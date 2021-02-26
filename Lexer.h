#ifndef _LEXER_H_
#define _LEXER_H_

#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <limits.h>
#include "Exceptions.h"

#define complete_str std::tuple<size_t, std::string> //row one_string

namespace Lexem
{
	using namespace std;

	namespace Utils
	{
		enum Tokens
		{
			e_Comment, e_Dollar, e_JI, e_Pop, e_Push, e_Jmp, e_Read, e_Write, e_list, e_End,
			e_Diff, e_Sym, e_Inter, e_Union, e_Err, e_End_mark, e_TCompare, e_TOperation, e_At,
			e_Concat, e_Size, e_Substr, e_Delsubstr, e_Empty, e_Cycle, e_If, e_Init, e_Create,
			e_Except, e_Type, e_Var, e_Mark, e_Const, e_ODD,

			e_Unread,



			e_AllToken
		};

		//row - res tokenaze
		const char cur_token [34] [11]
		{
			{"Comment"}, {"Dollar"}, {"JI"}, {"Pop"}, {"Push"},
			{"Jmp"}, {"Read"}, {"Write"}, {"list"}, {"End"},
			{"Diff"}, {"Sym"}, {"Inter"}, {"Union"}, {"Err"},
			{"End_mark"}, {"TCompare"}, {"TOperation"}, {"At"}, {"Concat"},
			{"Size"}, {"Substr"}, {"Delsubstr"}, {"Empty"}, {"Cycle"},
			{"If"},{"Init"},{"Create"},{"Except"},{"Type"},
			{"Var"}, {"Mark"}, {"Const"}, {"ODD"}
		};

		enum arrow
		{
			e_Number,
			e_WholeStr,

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

			else if (word == "Comm" || word == "//" || word == "<--" || word == "-->")
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

			else if (word == ":")
				return e_ODD;

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

			else if (std::min(std::max(std::stoi(word), INT_MIN), INT_MAX) != INT_MIN ||
				std::min(std::max(std::stoi(word), INT_MIN), INT_MAX) != INT_MAX)
				return e_Const;

			return e_Err;
		}
	}

	struct T_word
	{
		std::string token;
		std::string word;
	};

#define lexed_str std::tuple<size_t, std::vector<T_word>>

	std::vector<lexed_str> processed_str;

	//end doesnt include
	std::string substr(std::string::iterator start, std::string::iterator end)
	{
		std::string wrd;

		for (; start != end; ++start)
			wrd.push_back(*start);

		return wrd;
	}

	T_word Lexer(std::string wrd)
	{
		if (Utils::tokenaze(wrd) == Utils::e_Err)
		{
			Main_exception::tokenize_exception expt("Wrong token", wrd, 0);

			throw expt;
		}

		T_word res;

		res.token = Utils::cur_token [Utils::tokenaze(wrd)];
		res.word = wrd;

		return res;
	}

	void Lex_(std::vector<complete_str>& STR)
	{
		std::string initial_str;

		for (auto it : STR)
		{
			initial_str = get<Utils::e_WholeStr>(it);
			initial_str.push_back(' ');

			std::vector<T_word> T_words;
			std::string::iterator i_end;

			for (std::string::iterator i_it = initial_str.begin(); i_it != initial_str.end(); ++i_it)
			{
				i_end = std::find(i_it, initial_str.end( ), ' ');

				try
				{
					T_words.push_back(Lexer(substr(i_it, i_end)));
				}
				catch (Main_exception::tokenize_exception expt)
				{
					Main_exception::tokenize_exception expt_out(expt.Err, expt.Token, get<Utils::e_Number>(it));

					throw expt_out;
				}

				if (i_end == initial_str.end( ))
					break;
				i_it = i_end;
			}

			if (T_words [0].token == "Comment")
			{
				for (auto t_it : T_words)
					t_it.token = "Comment";
			}

			processed_str.push_back({std::get<Utils::e_Number>(it), T_words});
		}
	}


}

#endif
