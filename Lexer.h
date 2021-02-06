#ifndef _LEXER_H_
#define _LEXER_H_

#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include "Exceptions.h"

#define complete_str std::tuple<size_t, std::string> //row one_string
#define lexed_str std::tuple<size_t, std::string, std::vector<std::string>> //row token argument(s)

namespace Lexem
{
	using namespace std;

	vector<lexed_str> processed_note; //вектор токенизированных строк

	namespace Utils
	{
		static enum Tokens
		{
			e_Comment, e_Dollar, e_JI, e_Pop, e_Push, e_Jmp, e_Read, e_Write, e_list, e_End,
			e_Diff, e_Sym, e_Inter, e_Union, e_Err, e_End_mark, e_TCompare, e_TOperation

		};

		static int tokenaze(std::string word)
		{
			if (word == "ji")
				return e_JI;

			else if (word == "pop")
				return e_Pop;

			else if (word == "push")
				return e_Push;

			else if (word == "jmp")
				return e_Jmp;

			else if (word == "read")
				return e_Read;

			else if (word == "write")
				return e_Write;

			else if (word == "diff")
				return e_Diff;

			else if (word == "sym")
				return e_Sym;

			else if (word == "inter")
				return e_Inter;

			else if (word == "union")
				return e_Union;

			else if (word [0] == '+' || word [0] == '-' || word [0] == '*' || word [0] == '/' || word [0] == '%')
				return e_TOperation;

			else if (word [0] == '<' || word [0] == '>' || word [0] == '!' || word [0] == '=')
				return e_TCompare;

			else if (word == "Comm")
				return e_Comment;

			else if (word == "Mark")
				return e_End_mark;

			else if (word == "list")
				return e_list;

			else if (word == "end")
				return e_End;

			return e_Err;
		}
	}

	std::vector<std::string> arg_parse(std::string argument)
	{
		std::vector<std::string> result;
		std::string word;

		for (auto it : argument)
		{
			if (isalpha(it) || isdigit(it))
			{
				word.push_back(it);
			}
			else if (isspace(it) && !word.empty( ))
			{
				result.push_back(word);
				word.clear( );
			}
		}

		return result;
	}

	void Lex_(vector<complete_str>& Compl_STR)
	{
		string token;
		string argument;

		for (auto it : Compl_STR)
		{
			for (auto s_it : get<1>(it))
			{
				if (isalpha(s_it) || isdigit(s_it))
				{
					auto fst = s_it;
					for (; fst != ' '; ++fst); //can be better

					token = std::string(s_it, fst);//check it

					if (Utils::tokenaze(token) == Utils::e_Err)
					{
						Main_exception::tokenize_exception excpt("Unrecognized token", token, get<0>(it));
						throw excpt;
					}

					argument = std::string(fst, get<1>(it).find('\n')); //check it
					//auto str = std::string(input.begin( ) + input.find(' '), input.end( ));
					processed_note.push_back({get<0>(it), token, arg_parse(argument)});
				}
			}
		}
	}

}

#endif
