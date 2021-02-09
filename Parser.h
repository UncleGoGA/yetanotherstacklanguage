#ifndef _PARSER_H_
#define _PARSER_H_

//¬озможно стоит настроить параметры
//—уществующие разделители
//стоит ли пропускать пробелы и генерировать адекватные строки или нет
//стоит установить наличие флагов дл€ проверки на разрешенные символы
//это можно реализовать через enum class


//ѕочему не используютс€ классы дл€ представлени€ записи
//ѕотому что запись не €вл€етс€ настолько самодостаточным объектом.
#include <fstream>
#include <vector>
#include <tuple> // ортежи будут более оптимальным решением дл€ хранени€ записи
#include <string>
#include "Exceptions.h"

#define complete_str std::tuple<size_t, std::string>

namespace Parser
{
	using namespace std;

	vector<complete_str> note;	//вектор записей

	//возможно стоит добавить возможность парсить не только из txt
	//дл€ этого нужно будет написать функцию дл€ преобразовани€ или отдельную дл€ работы
	namespace Utils
	{
		enum Symbols //Ќеобходима€ проверка на разрешенные символы
		{
			e_Letter, e_Digit, e_Delim, e_NewLine, e_Operation, e_Compare, e_Error, e_Stop, e_Slash
		};

		static bool prepare_to_parse(const string& FILE)
		{
			ifstream stream(FILE);

			if (!stream.is_open( ))
			{
				stream.close( );
				return false;
			}

			stream.close( );
			return true;
		}

		static int transliterator(char ch)
		{
			if (isalpha(ch))
				return e_Letter;
			else if (isdigit(ch))
				return e_Digit;
			else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
				return e_Operation;
			else if (ch >= '<' && ch <= '>')
				return e_Compare;
			else if (ch == ' ')
				return e_Delim;
			else if (ch == '\n')
				return e_NewLine;
			else if (ch == '$')
				return e_Stop;
			else if (ch == '/')
				return e_Slash;

			return e_Error;
		}

		static bool transliterate_str(string& STR)
		{
			for (auto it : STR)
			{
				if (transliterator(it) == e_Error)
					return false;
			}

			return true;
		}
	}

	void Parse_(const string& FILE)
	{
		if (!Utils::prepare_to_parse(FILE))
		{
			Main_exception::parse_exception excp("Cant open file", 0);
			throw excp;
		}

		size_t Cur_Row = 1;
		string STR;
		string Pre_Str;

		ifstream stream(FILE);

		stream.seekg(0, std::ios::end);
		Pre_Str.reserve(stream.tellg( ));
		stream.seekg(0);

		Pre_Str.assign((std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>( ));

		//Need to currently parse last sentence
		//then we add null terminator to input string

		Pre_Str.push_back('\0');

		for (auto it : Pre_Str)//запускаем цикл по всей записи, в поисках строк
		{
			if (it == '\n' || it == '\0' || it == ',') //признак окончани€ строки, следовательно необходимо провести проверку прочитанной записи
			{
				if (!Utils::transliterate_str(STR))
				{
					Main_exception::parse_exception excp("Unrecognized symbols", Cur_Row);
					throw excp;
				}

				note.push_back({Cur_Row, STR}); //Since C++17
				Cur_Row++;
				STR.clear( );

				continue;
			}

			STR.push_back(it);
		}

		stream.close( );
	}
}


#endif
