#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <exception>
#include <string>

namespace Main_exception
{
	using namespace std;

	struct parse_exception:public exception
	{
		string Err;
		size_t Row;

		parse_exception(string Error, size_t Op_Row): Err(Error), Row(Op_Row){};
	};

	struct tokenize_exception:public exception
	{
		string Err;
		string Token;
		size_t Row;

		tokenize_exception(string Error, string Tok, size_t Op_Row): Err(Error), Token(Tok), Row(Op_Row){};
	};

	struct interpret_exception:public exception
	{
		string Err;
		string Token;
		string Argument;
		size_t Row;

		interpret_exception(string Error, string Tok, string Arg, size_t Op_Row): 
			Err(Error), Token(Tok), Argument(Arg), Row(Op_Row){};
	};
}

#endif
