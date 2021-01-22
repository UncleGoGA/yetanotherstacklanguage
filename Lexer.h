#ifndef _LEXER_H_
#define _LEXER_H_

#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include "Exceptions.h"

#define complete_str std::tuple<size_t, std::string>
#define lexed_str std::tuple<size_t, std::string, std::string, std::string> //row string token argument

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

	void Lex_(vector<complete_str>& Compl_STR)
	{
		stringstream word;
		string argument;

		for (auto it : Compl_STR)
		{
			// формируем токен и его аргумент
			// в данный момент проверяем только правильность токенов
			// аргументы нас не волнуют
			for (auto s_it : get<1>(it)) //обращаемся ко второму полю кортежа, т.е к строке
			{
				//возможно стоит использовать stringstream для форматирования
				//потому что ловить пробелы - боль.
			}
		}
	}

}

#endif
