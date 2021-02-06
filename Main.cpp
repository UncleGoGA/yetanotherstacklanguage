#include <iostream>
#include <string>
#include <fstream>

#include "Exceptions.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpretator.h"

using namespace std;

#define complete_str std::tuple<size_t, std::string> //row one_string
#define lexed_str std::tuple<size_t, std::string, std::vector<std::string>> //row token argument(s)

//to refactoring

int main(int argc, char** argv)
{
	std::string file_name;
	if (argc == 1) //it means we need to interpretate it
	{
		cout << "What file you need to proceed?" << endl;
		cin >> file_name;

		//Parser::note;
		//Lexem::processed_note;

		try
		{
			Parser::Parse_(file_name);
		}
		catch (Main_exception::parse_exception except)
		{
			cout << except.Err << "\tat" << except.Row << endl;
			return 0;
		}
		try
		{
			Lexem::Lex_(Parser::note);
		}
		catch (Main_exception::tokenize_exception except)
		{
			cout << except.Err << "\tat" << except.Row << "\twith" << except.Token << endl;
		}
		//Need to complete interpret
		//At this moment we just show processed lexem

		for (auto it : Parser::note)
		{
			cout << get<1>(it);
		}
	}

	else if (argc == 2)
	{
		file_name = argv [1];

		try
		{
			Parser::Parse_(file_name);
		}
		catch (Main_exception::parse_exception except)
		{
			cout << except.Err << "\tat" << except.Row << endl;
			return 0;
		}
		try
		{
			Lexem::Lex_(Parser::note);
		}
		catch (Main_exception::tokenize_exception except)
		{
			cout << except.Err << "\tat" << except.Row << "\twith" << except.Token << endl;
		}
		//Need to complete interpret
		//At this moment we just show processed lexem

		for (auto it : Parser::note)
		{
			cout << get<1>(it) << '\n';
		}
	}

	else
	{
		cout << "Unrecognized command" << endl;
	}

	return 0;
}