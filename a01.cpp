
#include <iostream>
#include <stdio.h>
// optimistically hoping this will eventually assist an ai proof writer

#include <fstream>
#include <string>
#include <map>

//
// negate statementss
//  

void load_known(std::map<std::string, std::string>* known_negations)
{
	std::fstream known_negations_text;
	known_negations_text.open("known_negations_text.txt", std::ios::in);

	// variables for loop
	std::string line;
	std::string key, value;

	if (known_negations_text.is_open())
	{
		while (std::getline(known_negations_text, line))
		{
			if (!(line.front() == '#'))
			{
				// key set to line pre "."
				key = (line.substr(0, line.find_first_of(".")));

				// value set t line post "."
				value = (line.substr(line.find_first_of(".") + 1, line.back()));

				// add pair to map
				known_negations->insert(std::pair<std::string, std::string>(key, value));
			}
		}
		known_negations_text.close();
	}
}

std::string first_command(std::string line)
{
	switch (line.front())
	{
	case '\\':
		return line.substr(0, line.substr(1, line.back()).find_first_of(" \\") + 1);
		break;
	default:
		return std::string(1, line.front());
		break;
	}
}

std::string negate(std::string statement)
{
	// variables
	std::string negated_string = "";
	std::string command;
	std::map<std::string, std::string> known_negations;

	// init
	load_known(&known_negations);

	while (statement.length() > 0)
	{
		// get first command
		command = first_command(statement);

		if (known_negations.find(command) == known_negations.end()) 
		{
			// command does not negate
			negated_string.append(command);

			// remove first command from statement
			statement.erase(0, command.length());
		}
		else 
		{
			// command negates
			negated_string.append(known_negations[command]);

			// remove extra space if "\"
			switch (statement.front())
			{
			case '\\':
				// remove first command from statement
				statement.erase(0, command.length() + 1);
				break;
			default:
				// remove first command from statement
				statement.erase(0, command.length());
				break;
			}
		}
	}

	// return success
	return negated_string;
}

//
// assertion test
// 

bool assert(bool passed)
{
	switch (passed)
	{
	case 0:
		printf("test failed\n");
		return false;
		break;
	default:
		printf("test passed\n");
		return true;
		break;
	}
}

//
// main
//

int main()
{
	assert(negate("\\forall n\\in\\mathbb{Z},\\; n=2") == "\\exists n\\in\\mathbb{Z},\\; n\\neq 2");
	assert(negate("\\forall n\\in\\mathbb{N},\\;\\exists y\\in\\mathbb{Z},\\; y^2\\geq n") == "\\exists n\\in\\mathbb{N},\\;\\forall y\\in\\mathbb{Z},\\; y^2<n");

	std::cout	<< "\n(a) " << negate("\\forall n\\in\\mathbb{N},\\;\\exists y\\in\\mathbb{Z},\\; y^2\\geq n") 
				<< "\n(b) " << negate("\\exists x\\in\mathbb{Z},\\;\\forall y\\in\\mathbb{Z},\\; (xy>0)\\land (x+y<0)")
				<< "\n(c) " << negate("\\exists x\\in\\mathbb{R},\\;\\forall n\\in\\mathbb{N},\\; (x<n)\\lor (x^2=n)")
				<< "\n(d) " << negate("\\forall a,b,c\\in\\mathbb{Z},\\; a^2+b^2=c^2");

	std::cout << std::endl;
}