#ifndef _LEXER_H_
#define _LEXER_H_

#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include "Exceptions.h"

#define complete_str std::tuple<size_t, std::string> //row one_string
#define lexed_str std::tuple<size_t, std::string, std::string> //row token argument

namespace Lexem
{
	using namespace std;

	vector<lexed_str> processed_note; //вектор токенизированных строк

	namespace Utils
	{
		enum Tokens
		{
			e_Comment, e_Dollar, e_JI, e_Pop, e_Push, e_Jmp, e_Read, e_Write, e_list, e_End,
			e_Diff, e_Sym, e_Inter, e_Union, e_Err, e_End_mark, e_TCompare, e_TOperation, e_At,
			e_Concat, e_Size, e_Substr, e_Delsubstr

		};
		//refactoring
		static int tokenaze(std::string word) //better to get all lexems by lower
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

			else if (word == "Comm" || word == "//") //at this moment we can use only C++ commentary style
				return e_Comment;

			else if (word == "Mark")
				return e_End_mark;

			else if (word == "list")
				return e_list;

			else if (word == "end")
				return e_End;

			else if (word == "at" || word == "At") //Random access into str
				return e_At;

			else if (word == "concat" || word == "Concat") //Concat with other str
				return e_Concat;

			else if (word == "size" || word == "Size")
				return e_Size;

			else if (word == "substr" || word == "Substr")
				return e_Substr;

			else if (word == "delsubstr" || word == "Delsubstr")
				return e_Delsubstr;

			return e_Err;
		}
	}

	std::string arg_parse(std::string argument)
	{
		std::string word;
		std::vector<std::string> words;

		for (auto it : argument)
		{
			if (isalpha(it) || isdigit(it))
				word.push_back(it);

			else if (isspace(it) && !word.empty( ))
				words.push_back(word);
		}

		word.clear( );

		for (auto it : words)
		{
			for (auto s_it = it.begin(); s_it != it.end(); ++s_it)
			{
				word.push_back(*s_it);
			}
			word.push_back(' ');
		}

		return word;
	}

	void Lex_(vector<complete_str>& Compl_STR)
	{
		string token;
		string str;

		for (auto it : Compl_STR)
		{
			if (get<1>(it) == "write" || get<1>(it) == "read" || get<1>(it) == "end")
			{
				token = get<1>(it);
				processed_note.push_back({get<0>(it), token, {}});

				token.clear( );
				continue;
			}

			str = get<1>(it);
			for (auto s_it = str.begin(); s_it != str.end(); ++s_it)
			{
				size_t pos = 0;
				if (isalpha(*s_it) || isdigit(*s_it) || *s_it == '/'/*refactoring*/)
				{
					for (; *s_it != ' '; ++s_it, ++pos)
						token.push_back(*s_it);

					if (Utils::tokenaze(token) == Utils::e_Err)
					{
						Main_exception::tokenize_exception excpt("Unrecognized token", token, get<0>(it));
						throw excpt;
					}

					processed_note.push_back({get<0>(it), token, arg_parse(get<1>(it).substr(pos, get<1>(it).size()))});

					token.clear( );
					
					break;
				}
			}
		}
	}

}

#endif
