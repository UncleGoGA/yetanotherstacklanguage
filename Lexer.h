#ifndef _LEXER_H_
#define _LEXER_H_

#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include "Exceptions.h"

#define complete_str std::tuple<size_t, std::string> //row one_string
//need to use vector as data storage for arguments
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
			e_Concat, e_Size, e_Substr, e_Delsubstr, e_Empty, e_Cycle, e_If, e_Init, e_Create,
			e_Except, e_Type, e_Var, e_Mark,



			e_AllToken
		};

		enum arrow
		{
			e_Number,
			e_WholeStr,
			e_Lexem = 1,
			e_Argument,

			e_count
		};

		//refactoring
		static int tokenaze(std::string& word) //better to get all lexems by lower
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

			else if (word == "Comm" || word == "//" || word == "<--") //at this moment we can use C++ commentary style
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

			else if (word == ";")
				return e_Empty;

			else if (word == "for" || word == "while")
				return e_If;

			else if (word [0] == '+' || word [0] == '-' || word [0] == '*' || word [0] == '/' || word [0] == '%')
				return e_TOperation;

			else if (word [0] == '<' || word [0] == '>' || word [0] == '!' || word [0] == '=')
				return e_TCompare;

			else if (word == "exception")
				return e_Except;

			else if (word [0] >= 'A' && word [0] <= 'Z' || word [0] >= 'a' && word [0] <= 'z')
				return e_Var;

			else if (word == "type")
				return e_Type;

			else if (word == "let")
				return e_Init;

			else if (word [0] == '<' && word [1] == '<'
				&& word [word.size( ) - 1] == '>' && word [word.size( ) - 2] == '>')
				return e_Mark;

			return e_Err;
		}
	}

	std::string arg_parse(std::string argument)
	{
		argument.push_back(' ');
		std::string word;
		std::vector<std::string> words;

		for (auto it : argument)
		{
			if (isalpha(it) || isdigit(it))
				word.push_back(it);

			else if (isspace(it) && !word.empty( ))
			{
				words.push_back(word);
				word.clear( );
			}
		}

		word.clear( );

		for (auto it : words)
		{
			for (auto s_it = it.begin( ); s_it != it.end( ); ++s_it)
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
			if (get<Utils::e_WholeStr>(it) == "write" || get<Utils::e_WholeStr>(it) == "read" || get<Utils::e_WholeStr>(it) == "end"
				|| Utils::tokenaze(get<Utils::e_WholeStr>(it)) == Utils::e_Mark)
			{
				token = get<Utils::e_WholeStr>(it);
				processed_note.push_back({get<Utils::e_Number>(it), token, {}});

				token.clear( );
				continue;
			}

			str = get<Utils::e_WholeStr>(it);
			for (auto s_it = str.begin( ); s_it != str.end( ); ++s_it)
			{
				size_t pos = 0;
				if (isalpha(*s_it) || isdigit(*s_it) || *s_it == '/'/*refactoring*/ ||
					*s_it == '-' || *s_it == '<')
				{
					for (; *s_it != ' '; ++s_it, ++pos)
						token.push_back(*s_it);

					if (Utils::tokenaze(token) == Utils::e_Err)
					{
						Main_exception::tokenize_exception excpt("Unrecognized token", token, get<0>(it));
						throw excpt;
					}

					if (token == "<--" || token == "exception" || Utils::tokenaze(token) == Utils::e_Var)
					{
						string semi = get<Utils::e_WholeStr>(it);

						for (string::reverse_iterator r_it = semi.rbegin( ); r_it != semi.rend( ); ++r_it)
						{
							if (Utils::tokenaze(token) == Utils::e_Var)
							{
								if (*r_it == 'e' && *(r_it + 1) == 'p' && *(r_it + 2) == 'y' && *(r_it + 3) == 'T' &&
									(*(r_it + 4) == ':' || *(r_it + 5) == ':'))
								{
									processed_note.push_back({get<Utils::e_Number>(it), semi, {}});

									token.clear( );
									break;
								}
							}
							else
							{
								if (*r_it == '>' && *(r_it + 1) == '-' && *(r_it + 2) == '-' && *(r_it + 3) != '<')
								{
									processed_note.push_back({get<Utils::e_Number>(it), semi, {}});

									token.clear( );
									break;
								}
							}
						}
					}


					processed_note.push_back({get<Utils::e_Number>(it), token,
						arg_parse(get<Utils::e_WholeStr>(it).substr(pos, get<Utils::e_WholeStr>(it).size( )))});

					token.clear( );

					break;
				}
			}
		}
	}

}

#endif
