#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define left_count count_input(setty, iter.begin->value)
#define left_count_diff count_input(setty, lst_iter.begin->value)
#define right_count lst.count_input(lst.setty, lst_iter.begin->value)
#define right_count_diff lst.count_input(lst.setty, iter.begin->value)
#define ll long

/*
По итогу имеем некоторое соглашение по именам, но не по использованию функций
*/
namespace List
{
	std::string to_string(ll val)
	{
		ll copy = val;
		size_t size = 0;
		std::string res;

		for (; copy > 0; ++size, copy /= 10);

		for (; val > 0; copy += val % 10, copy *= 10, val /= 10);

		copy /= 10;

		for (; copy > 0; res.push_back((copy % 10) + 48), copy /= 10);

		return res;
	}

	struct set
	{
		ll value;
		set* next = nullptr;
	};

	class list
	{
		size_t cur_size;
		bool empty = true;
		size_t count_input(set* cur, ll value)
		{
			size_t count = 0;
			set* start = cur;

			while (start->next)
			{
				if (start->value == value)
				{
					count++;
				}
			}

			return count;
		}

		//просто пустое множество тоже нужно как то обозначать, поэтому данный подход вполне целесообразен
		//т.е у нас могут быть множества в однозначных числах(имею в виду, что любой элемент множества будет >= 0, а 9 просто может входить из - за СС)
		//так почему бы не закодировать пустое множество с помощью -1?
		void copy_travel(set* cur, set* n_cur)
		{ //исправлено
			if (n_cur)
			{
				if (empty)
				{
					cur = new set;
					empty = false;
				}
				if (!cur)
					cur = new set;
				cur->value = n_cur->value;
				cur = cur->next;
				copy_travel(cur, n_cur->next);
			}
		}

		void del_travel(set* next)
		{
			if (!empty)
			{
				if (next)
				{
					if (next->next)
						del_travel(next->next);
				}
				if (next)
					delete next;
				cur_size--;
				if (cur_size <= 0)
					empty = true;
				next = nullptr;
			}
		}

		set* setty;
	public:
		set* get_set( )
		{
			return setty;
		}
		bool is_empty( )
		{
			return empty;
		}
		class iterator
		{
			set* begin;
		public:
			friend class list;
			iterator( )
			{
				begin = nullptr;
			}
			iterator(set* settle)
			{
				begin = settle;
			}

			void reset_iter(set* settle)
			{
				begin = settle;
			}

			iterator operator=(const iterator& iter)
			{
				if (this == &iter)
				{
					return *this;
				}
				begin = iter.begin;
				return *this;
			}

			iterator& operator++( )
			{
				begin = begin->next;
				return *this;
			}

			iterator& operator+(const size_t inc)
			{
				iterator res = *this;

				for (size_t i = 0; i < inc; ++i)
				{
					res.begin = res.begin->next;
				}

				return res;
			}
			//вы имеете в виду проверку на владение одним объектом?
			bool operator!=(const iterator& obj) const
			{
				return (begin != obj.begin);
			}

			bool operator==(const iterator& obj) const
			{
				return (begin == obj.begin);
			}

			ll& operator*( )
			{
				return begin->value;
			}
		};

		list( )
		{
			cur_size = 0;
			setty = nullptr;
		}

		list(const list& obj)
		{
			copy_travel(setty, obj.setty);
		}

		~list( )
		{
			del_travel(setty);
		}

		void push_back(ll& val)
		{
			if (empty)
			{
				setty = new set;
				setty->value = val;
				empty = false;
				cur_size = 0;
				cur_size++;
				return;
			}
			set* start = setty;
			while (start->next) start = start->next;

			start->next = new set;
			start = start->next;
			start->value = val;
			cur_size++;
		}

		bool is(ll& val)
		{ //O(n)
			set* start = setty;
			for (size_t i = 0; i < len( ); ++i, start = start->next)
			{
				if (start->value == val)
				{
					return true;
				}
			}

			return false;
		}

		bool operator==(list& lst)
		{
			//для начала стоит проверить множества на мощность

			if (len( ) != lst.len( ))
				return false;

			iterator iter(setty), lst_iter(lst.setty);

			for (size_t i = 0; i < len( ); ++i, lst_iter.begin++, iter.begin++)
			{
				if (count_input(setty, lst_iter.begin->value) != count_input(lst.setty, lst_iter.begin->value) ||
					count_input(setty, iter.begin->value) != count_input(lst.setty, iter.begin->value))
					return false;
			}

			return true;
		}

		bool operator!=(list& lst)
		{
			//увы, здесь мощность ничего не скажет нам о множестве
			//точнее как вы и отмечали будут ложные срабатывания

			if (len( ) == lst.len( ))
			{

				iterator lst_iter(lst.setty), iter(setty);

				for (size_t i = 0; i < len( ); lst_iter.begin++, iter.begin++)
				{
					if (count_input(setty, lst_iter.begin->value) != count_input(lst.setty, lst_iter.begin->value) ||
						count_input(setty, iter.begin->value) != count_input(lst.setty, iter.begin->value))
						return true;
				}
				return false;

			}
			else
			{
				return true;
			}
		}

		void operator=(list lst)
		{
			if (*this == lst)
				return;

			copy_travel(setty, lst.setty);
		}


		size_t len( )
		{
			return cur_size;
		}

		list Diff(list& lst)
		{
			list res;

			iterator iter, lst_iter;
			iter.reset_iter(setty);
			lst_iter.reset_iter(lst.setty);

			if (lst.len( ) > len( ))
			{
				for (size_t i = 0; i < lst.len( ); ++i, lst_iter.begin++)
				{
					if (is(lst_iter.begin->value))
					{
						if (count_input(setty, lst_iter.begin->value) > lst.count_input(lst.setty, lst_iter.begin->value))
						{
							for (size_t i = 0; i < left_count_diff - right_count; ++i)
							{
								res.push_back(lst_iter.begin->value);
							}
						}
						else if (count_input(setty, lst_iter.begin->value) < lst.count_input(lst.setty, lst_iter.begin->value))
						{
							for (size_t i = 0; i < right_count - left_count_diff; ++i)
							{
								res.push_back(lst_iter.begin->value);
							}
						}
					}
				}
			}
			else
			{
				for (size_t k = 0; k < len( ); ++k, iter.begin++)
				{
					if (lst.is(iter.begin->value))
					{
						if (count_input(setty, iter.begin->value) > lst.count_input(lst.setty, iter.begin->value))
						{
							for (size_t i = 0; i < left_count - right_count_diff; ++i)
							{
								res.push_back(lst_iter.begin->value);
							}
						}
						else if (count_input(setty, lst_iter.begin->value) < lst.count_input(lst.setty, lst_iter.begin->value))
						{
							for (size_t i = 0; i < right_count_diff - left_count; ++i)
							{
								res.push_back(lst_iter.begin->value);
							}
						}
					}
				}
			}


			return res;
		}

		list Union(list& lst)
		{
			list res(*this);
			iterator iter, lst_iter;
			iter.reset_iter(res.setty);
			lst_iter.reset_iter(lst.setty);

			for (; lst_iter.begin != nullptr; lst_iter.begin++, iter.begin++)
			{
				if (count_input(res.setty, iter.begin->value) > lst.count_input(lst.setty, lst_iter.begin->value))
				{
					for (size_t i = 0; i < left_count - right_count; ++i)
					{
						res.push_back(iter.begin->value);
					}
				}
				else
				{
					for (size_t i = 0; i < right_count - left_count; ++i)
					{
						res.push_back(lst_iter.begin->value);
					}
				}
			}
			return res;
		}

		list Symm_Diff(list& lst)
		{
			list llhd, lrhd, res;
			iterator lhd, rhd;
			lhd.reset_iter(setty);
			rhd.reset_iter(lst.setty);

			llhd = Diff(lst);
			lrhd = lst.Diff(*this);

			res = llhd.Diff(lrhd);

			return res;
		}

		list Intersaction(list& lst)
		{
			list res;
			iterator iter(setty)/*просто показал несколько вариантов инициализации*/,
				lst_iter(lst.setty);

			if (len( ) > lst.len( ))
			{
				if (is(lst_iter.begin->value))
				{
					for (; iter.begin != nullptr; iter.begin++)
					{
						if (left_count > right_count_diff)
						{
							for (size_t i = 0; i < left_count - right_count_diff; ++i)
							{
								res.push_back(iter.begin->value);
							}
						}
					}
				}
			}
			else
			{
				if (lst.is(iter.begin->value))
				{
					for (; lst_iter.begin != nullptr; lst_iter.begin++)
					{
						if (left_count_diff < right_count)
						{
							for (size_t i = 0; i < right_count - left_count_diff; ++i)
							{
								res.push_back(lst_iter.begin->value);
							}
						}
					}
				}
			}

			return res;
		}

		void insert_first_fail( )
		{ //исправлено пару раз
			iterator iter(setty);

			ll lower_bound = setty->value;
			ll upper_bound = setty->value;
			for (; iter.begin != nullptr; iter.begin++)
			{
				if (iter.begin->value >= upper_bound)
					upper_bound = iter.begin->value;
				if (iter.begin->value < lower_bound)
					lower_bound = iter.begin->value;
			}

			for (ll i = 0; i < lower_bound; ++i)
			{ //ищем минимальный элемент от lower_bound до 0
//если нашли, то здорово, если нет, то будем пытаться вставить элемент от lower_bound до upper_bound
//если и в этом случае не нашли, то вставляем upper_bound + 1
				if (!is(i))
				{
					push_back(i);
					return;
				}
			}
			for (; lower_bound < upper_bound; ++lower_bound)
			{
				if (!is(lower_bound))
				{
					push_back(lower_bound);
					return;
				}
			}

			upper_bound++;
			push_back(upper_bound);
			return;

		}

	};

	std::istream& operator>>(std::istream& is, list lst)
	{
		char* start = new char [INT_MAX / 2];

		is >> start;

		size_t len = is.tellg( );

		char* new_one = new char [len];
		ll value = 0;
		for (size_t i = 0; i < len; ++i)
		{
			new_one [i] = start [i];
		}

		for (size_t i = 2; i < len - 1; ++i)
		{
			if (isdigit(new_one [i]))
			{
				value += new_one [i] - 48;
				value *= 10;
			}
			if (new_one [i] == '/')
			{
				value /= 10;
				lst.push_back(value);
			}
		}

		return is;
	}

	std::ostream& operator<<(std::ostream& os, list lst)
	{
		os << "//";
		set* start = lst.get_set( );
		while (start)
		{
			os << start->value << "/";
			start = start->next;
		}
		os << "/";

		return os;
	}
}

//Будем использовать дерево для работы над множествами
//Внутри стека могут быть использованы какие угодно операции
//Но как только все числа были вытолкнуты в какую то переменную
//Мы заносим имя этой переменной в дерево и работаем уже с ним
namespace Tree
{
	struct tree_pair
	{
		std::string name;
		List::list value;
	};

	struct except:public std::exception
	{
		std::string variable_name;
	};

	class BinTree
	{
		bool empty;
		tree_pair cur_pair;
		BinTree* left, * right;

		void copy_tree(BinTree* root, BinTree* AnotherTree)
		{
			if (AnotherTree)
			{
				cur_pair.name = AnotherTree->cur_pair.name;
				cur_pair.value = AnotherTree->cur_pair.value;
				if (AnotherTree->left)
				{
					if (!left)
						left = new BinTree;
					copy_tree(left, AnotherTree->left);
				}
				if (AnotherTree->right)
				{
					if (!right)
						right = new BinTree;
					copy_tree(right, AnotherTree->right);
				}
			}
		}

		void del_tree(BinTree* root)
		{
			if (root)
			{
				if (!root->empty)
				{
					BinTree* left = nullptr, * right = nullptr;
					if (root->left)
						left = root->left;
					if (root->right)
						right = root->right;

					if (root)
						delete root;
					root = nullptr;

					del_tree(left);
					del_tree(right);
				}
			}

		}

	public:
		BinTree( )
		{
			left = right = nullptr;
			empty = true;
		}
		BinTree(BinTree* AnotherTree)
		{
			cur_pair.name = AnotherTree->cur_pair.name;
			cur_pair.value = AnotherTree->cur_pair.value;
			{
				if (AnotherTree->left)
				{
					left = new BinTree;
					copy_tree(left, AnotherTree->left);
				}
				if (AnotherTree->right)
				{
					right = new BinTree;
					copy_tree(right, AnotherTree->right);
				}
			}
		}
		~BinTree( )
		{
			del_tree(left);
			del_tree(right);
		}

		void reset(BinTree& AnotherTree)
		{
			del_tree(left);
			del_tree(right);

			cur_pair.name = AnotherTree.cur_pair.name;
			cur_pair.value = AnotherTree.cur_pair.value;

			copy_tree(left, AnotherTree.left);
			copy_tree(right, AnotherTree.right);
		}

		tree_pair* find(std::string name)
		{
			BinTree* root;

			if (name > cur_pair.name)
				root = right;
			else if (name < cur_pair.name)
				root = left;
			else
				return &cur_pair;

			while (root != nullptr)
			{
				if (name > root->cur_pair.name)
					root = root->right;
				else if (name < root->cur_pair.name)
					root = root->left;
				else
					return &root->cur_pair;
			}

			return nullptr;
		}
		//true - income_value
		//false - !income_value
		void push_back(std::string name, ll income_value, bool flag)
		{
			if (cur_pair.name.empty( ))
			{
				cur_pair.name = name;
				cur_pair.value.push_back(income_value);
			}
			else
			{
				if (find(name))
				{
					if (flag)
					{
						find(name)->value.push_back(income_value);
						empty = false;
					}
				}
				else
				{
					BinTree* root;

					if (name > cur_pair.name)
					{
						if (right)
							root = right;
						{
							right = new BinTree;
							right->cur_pair.name = name;
							root = right;
							root->empty = false;
						}
					}
					else
					{
						if (left)
							root = left;
						else
						{
							left = new BinTree;
							left->cur_pair.name = name;
							root = left;
							root->empty = false;
						}
					}

					while (true)
					{
						if (name > root->cur_pair.name)
						{
							if (!root->right)
							{
								root->right = new BinTree;
								root = root->right;
								root->empty = false;

								root->cur_pair.name = name;
								if (flag)
									root->cur_pair.value.push_back(income_value);
								return;
							}
							else
							{
								root = root->right;
							}
						}
						else if (name < root->cur_pair.name)
						{
							if (!root->left)
							{
								root->left = new BinTree;
								root = root->left;
								root->empty = false;

								root->cur_pair.name = name;
								if (flag)
									root->cur_pair.value.push_back(income_value);
								return;
							}
							else
							{
								root = root->left;
							}
						}
						else
						{
							if (flag)
							{
								root->cur_pair.value.push_back(income_value);
								root->empty = false;
							}
							return;
						}
					}
				}
			}
		}
	};



}

//Будем использовать некоторое соглашение в именах
//Таким образом мы сможем всегда быстро отыскать места использования

enum Symbols
{
	/*Letter, Digit, Operation, Compare, Razdelit, Cr, Point, Error, End, Skobochka, Skobochka2*/
	e_Letter, e_Digit, e_Delim, e_NewLine, e_Operation, e_Compare, e_Error, e_Stop, e_Slash
};

//на мой взгляд правильнее всего использовать Trie Tree / Radix Tree для определения ошибок
enum Tokens
{
	e_Comment, e_Dollar, e_JI, e_Pop, e_Push, e_Jmp, e_Read, e_Write, e_list, e_End,
	e_Diff, e_Sym, e_Inter, e_Union, e_Err, e_End_mark, e_TCompare, e_TOperation

};

int transliterator(char ch)
{
	if (isalpha(ch))
		return e_Letter;
	if (isdigit(ch))
		return e_Digit;
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
		return e_Operation;
	if (ch >= '<' && ch <= '>')
		return e_Compare;
	if (ch == ' ')
		return e_Delim;
	if (ch == '\n')
		return e_NewLine;
	if (ch == '$')
		return e_Stop;
	if (ch == '/')
		return e_Slash;

	return e_Error;
}

//сначала прогоняем по транслитератору в поисках неразрешенных символов
//затем поставим токены и типы по всем найденным словам
struct produced_words
{
	std::string word;
	std::string value;
};
struct marked
{
	std::string name;
	size_t row;
};
struct variable
{
	std::string name;
	ll value;
};

std::vector<variable> variables;
std::vector<produced_words> words;
std::stack<ll> values;
std::vector<marked> marks;
bool option = false;

size_t end_zero_count(ll val)
{
	size_t res = 0;
	for (; val > 0; val /= 10)
	{
		if (val % 10 != 0)
			break;
		if (val % 10 == 0)
			res++;
	}

	return res;
}

int tokenaze(std::string word)
{
	/*Tokens: e_Comment, e_Dollar, e_JI, e_Pop, e_Push, e_Jmp, e_Read, e_Write,
	e_Diff, e_Sym, e_Inter, e_Union, e_Err, e_End_mark, e_TCompare, e_TOperation*/

	if (word == "ji")
		return e_JI;

	if (word == "pop")
		return e_Pop;

	if (word == "push")
		return e_Push;

	if (word == "jmp")
		return e_Jmp;

	if (word == "read")
		return e_Read;

	if (word == "write")
		return e_Write;

	if (word == "diff")
		return e_Diff;

	if (word == "sym")
		return e_Sym;

	if (word == "inter")
		return e_Inter;

	if (word == "union")
		return e_Union;

	if (word[0] == '+' || word[0] == '-' || word[0] == '*' || word[0] == '/' || word[0] == '%')
		return e_TOperation;

	if (word[0] == '<' || word[0] == '>' || word[0] == '!' || word[0] == '=')
		return e_TCompare;

	if (word == "Comm")
		return e_Comment;

	if (word == "Mark")
		return e_End_mark;

	if (word == "list")
		return e_list;

	if (word == "end")
		return e_End;

	return e_Err;
}

produced_words make_word(std::string word, bool flag, std::string particular)
{
	produced_words prod;

	if (flag)
	{
		size_t size = 0;
		for (; transliterator(word [size]) != e_Delim; ++size);

		prod.word = word.substr(0, size);
		prod.value = word.substr(size, word.size( ));
	}
	else
	{
		prod.word = particular;
		prod.value = word;
	}

	return prod;
}

template<typename T>
bool find(std::vector<T> marks, std::string src)
{
	for (auto it : marks)
	{
		if (it.name == src)
			return true;
	}

	return false;
}

template<typename T>
T& find_via(std::vector<T>& marks, std::string src)
{
	//for (auto it : marks)
	//{
	//	if (it.name == src)
	//		return it;
	//}

	for (size_t i = 0; i < marks.size( ); ++i)
	{
		if (marks [i].name == src)
		{
			return marks [i];
		}
	}

}

struct main_except: public std::exception
{
	std::string name;
	int row;
};

void trans(std::string str, Tree::BinTree Tree)
{
	/*Letters: e_Letter, e_Digit, e_Delim, e_NewLine, e_Operation, e_Compare, e_Error, e_Stop, e_Slash*/

	for (size_t i = 0, row = 0; i < str.size( ); ++i) //Проверяем на разрешенные символы
	{
		if (str [i] == '\n')
			++row;
		if (transliterator(str [i]) == e_Error)
		{
			main_except except;
			except.row = row;
			except.name = "Unspecified symbol " + str [i];

			throw except;
		}
	}

	for (size_t i = 0, row = 0, numchar = 0; i < str.size( ) || transliterator(str [i]) != e_Stop; ++i, ++numchar)
	{
		if (str [i] == '\n')
			++row;

		if (transliterator(str [i]) == e_NewLine || transliterator(str [i]) == e_Delim || str[i] == '\0')
		{
			std::string wrd;
			if (numchar != i)
			{
				i--;
				numchar--;
				for (size_t n = i - numchar; n < i + 1; ++n)
				{
					wrd.push_back(str [n]);
				}
			}
			else
			{
				for (size_t n = i - numchar; n < i; ++n)
				{
					wrd.push_back(str [n]);
				}
			}
			/*std::string wrd = str.substr(i - numchar, i);*/

			if (tokenaze(wrd) == e_Err)
			{
				main_except except;
				except.row = row;
				except.name = "Unspecified token " + str [i];

				throw except;
			}
			else if (tokenaze(wrd) == e_End)
			{
				words.push_back(make_word(std::string(), false, wrd));

				break;
			}

			{
				if (tokenaze(wrd) == e_Comment)
				{
					//может быть что угодно, после комментария, но до \n
					//поэтому можем не проверять
					wrd.push_back(' ');
					i++;
					for (;; ++i)
					{
						if (str [i] == '\n')
						{

							row++;
							break;
						}
						//value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					words.push_back(make_word(wrd, true, std::string( )));
					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Dollar)
				{
					words.push_back(make_word(wrd, false, "End - "));
					numchar = -1;
				}

				else if (tokenaze(wrd) == e_JI)
				{
					//допилить одноместный вариант

					//трехместная операция
					//проверяем value_check, конкретно нас интересует следующее
					//value_check должен выглядеть так
					//ji 5 >
					//ji 5 x < y
					//в первом случае проверяем стек, есть ли там числа 
					//если есть, переходим на 5 строку
					//если нет, то просто идем дальше
					//во втором случае, разрешены только операции = и !
					//если какие то другие операции, шлем исключение
					//если true, то переходим, иначе идем дальше
					wrd.push_back(' ');
					std::string value_check;
					++i;
					for (;; ++i)
					{
						if (str [i] == '\n')
						{
							row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}
					
					if (value_check [0] == ' ')
						value_check.erase(value_check.begin( ));

					std::string semi; //я знаю что память использована неэффективно и можно было написать оптимальнее
					std::string position; //также используется для запоминания операции
					std::string first_name;
					std::string second_name;
					size_t pos = 0;
					size_t new_row = 1;

					bool is_set = true;

					bool mark = true;
					bool lhs = true;
					bool oper = true;
					bool rhs = true;

					if (value_check [0] == ' ')
						value_check.erase(value_check.begin( ));

					for (auto it : value_check)
					{
						//если там 
						if (it == ' ' || it == '\n')
						{
							if (mark)
							{
								if (transliterator(semi [0]) == e_Letter)
								{
									if (find<marked>(marks, semi))
									{
										for (; ; ++pos)
										{
											if (new_row == find_via<marked>(marks, semi).row)
												break;
											if (str [pos] == '\n')
											{
												++new_row;
											}
										}
										--pos;
									}
								}
								else
								{
									if (atoi(semi.c_str( )) <= 0)
									{
										main_except except;
										except.row = row;
										except.name = "Position must be > 0 " + str [i];

										throw except;
									}

									position = semi;

									for (; ; ++pos)
									{
										if (new_row == atoi(position.c_str( )))
											break;
										if (str [pos] == '\n')
										{
											++new_row;
										}
									}
									--pos;
								}

								semi.clear( );
								mark = false;
								continue;
							}

							else if (lhs)
							{
								//значит мы должны работать над множествами
								if (transliterator(semi [0]) == e_Letter)
								{
									if (find<variable>(variables, semi))
									{
										is_set = false;
									}
									else if (!Tree.find(semi))
									{
										main_except except;
										except.row = row;
										except.name = "Unreachable variable " + wrd;

										throw except;
									}
									first_name = semi;
								}

								//значит мы должны работать над числами в стеке
								else if (transliterator(semi [0]) == e_Compare || transliterator(semi [0]) == e_Operation)
								{
									if (values.empty( ))
									{
										main_except except;
										except.row = row;
										except.name = "Unreachable operation, stack is empty " + wrd;

										throw except;
									}
									else if (values.size( ) < 2)
									{
										main_except except;
										except.row = row;
										except.name = "Unreachable operation, stack have less than 2 values " + wrd;

										throw except;
									}
									else
									{
										//еще больший ужас.
										//мы же не хотим портить стек
										if (semi [0] == '=')
										{
											std::stack<ll> predicted = values;

											//меееее как плохо то
											ll fst = predicted.top( );
											predicted.pop( );
											ll snd = predicted.top( );
											predicted.pop( );

											if (fst == snd)
											{
												semi = "true";
												i = pos;
												row = new_row;
												break;
											}
											else
											{
												semi = "false";
												break;
											}
										}
										else if (semi [0] == '!')
										{
											std::stack<ll> predicted = values;

											//меееее как плохо то
											ll fst = predicted.top( );
											predicted.pop( );
											ll snd = predicted.top( );
											predicted.pop( );

											if (fst == snd)
											{
												semi = "true";
												i = pos;
												row = new_row;
												break;
											}
											else
											{
												semi = "false";
												break;
											}
										}
										else if (semi [0] == '<')
										{
											std::stack<ll> predicted = values;

											//меееее как плохо то
											ll fst = predicted.top( );
											predicted.pop( );
											ll snd = predicted.top( );
											predicted.pop( );

											if (fst == snd)
											{
												semi = "true";
												i = pos;
												row = new_row;
												break;
											}
											else
											{
												semi = "false";
												break;
											}
										}
										else if (semi [0] == '>')
										{
											std::stack<ll> predicted = values;

											//меееее как плохо то
											ll fst = predicted.top( );
											predicted.pop( );
											ll snd = predicted.top( );
											predicted.pop( );

											if (fst == snd)
											{
												semi = "true";
												i = pos;
												row = new_row;
												break;
											}
											else
											{
												semi = "false";
												break;
											}
										}
										else
										{
											main_except except;
											except.row = row;
											except.name = "Undefined operation " + semi;

											throw except;
										}
									}
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Undefined symbol " + semi;

									throw except;
								}
								semi.clear( );
								lhs = false;
								continue;
							}

							//значит мы работаем над множествами
							else if (oper)
							{
								if (transliterator(semi [0]) == e_Compare)
								{
									if (is_set)
									{
										if (semi == "!" || semi == "=")
											position = semi;
										else
										{
											main_except except;
											except.row = row;
											except.name = "Undefined operation " + semi;

											throw except;
										}
									}
									else
									{
										if (semi == "!" || semi == "=" || semi == "<" || semi == ">")
										{
											position = semi;
										}
										else
										{
											main_except except;
											except.row = row;
											except.name = "Undefined operation " + semi;

											throw except;
										}
									}
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Undefined operation " + semi;

									throw except;
								}

								semi.clear( );
								oper = false;
								continue;
							}

							else if (rhs)
							{
								if (transliterator(semi [0]) == e_Letter)
								{
									if (is_set)
									{
										if (!Tree.find(semi))
										{
											main_except except;
											except.row = row;
											except.name = "Unreachable variable " + wrd;

											throw except;
										}

										second_name = semi;

										Tree::tree_pair* first, * second;
										first = Tree.find(first_name);
										second = Tree.find(second_name);

										if (position == "!")
										{
											semi = first->value.operator!=(second->value) ? "true" : "false";
											break;
										}
										else
										{
											semi = first->value.operator==(second->value) ? "true" : "false";
											break;
										}
									}
									else
									{
										second_name = semi;

										if (position == "!")
										{
											semi = find_via<variable>(variables, first_name).value !=
												find_via<variable>(variables, second_name).value ? "true" : "false";
											break;
										}
										else if (position == "=")
										{
											semi = find_via<variable>(variables, first_name).value ==
												find_via<variable>(variables, second_name).value ? "true" : "false";
											break;
										}
										else if (position == ">")
										{
											semi = find_via<variable>(variables, first_name).value >
												find_via<variable>(variables, second_name).value ? "true" : "false";
											break;
										}
										else if (position == "<")
										{
											semi = find_via<variable>(variables, first_name).value <
												find_via<variable>(variables, second_name).value ? "true" : "false";
											break;
										}
									}
								}
								rhs = false;
								continue;
							}

							else
							{
								main_except except;
								except.row = row;
								except.name = "Undefined symbol " + semi;

								throw except;
							}
						}
						semi.push_back(it);
					}

					words.push_back(make_word(wrd, true, std::string( )));
					if (semi == "true")
					{
						i = pos;
						row = new_row;
					}
					else if (transliterator(semi [0]) == e_Digit)
					{
						if (option)
						{
							option = false;
							size_t pos = 0;
							size_t new_row = 1;
							for (; ; ++pos)
							{
								if (new_row == atoi(semi.c_str()))
									break;
								if (str [pos] == '\n')
								{
									++new_row;
								}
							}
							--pos;
							i = pos;
							row = new_row;
						}
					}

					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Pop)
				{
					wrd.push_back(' ');
					std::string value_check;
					std::string list_token;
					bool is_list = false;
					++i;
					for (;; ++i)
					{
						if (str [i] == ' ')
							is_list = true;
						if (str [i] == '\n')
						{
							row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					if (value_check [0] == ' ')
					{
						value_check.erase(value_check.begin( ));
					}

					if (is_list)
					{
						for (size_t i = 0; i < value_check.size( ); ++i)
						{
							if (value_check [i] == ' ')
							{
								for (size_t j = i + 1; j < value_check.size( ); ++j)
									list_token.push_back(value_check [j]);

								break;
							}
						}
					}

					if (tokenaze(list_token) != e_list)
					{
						is_list = false;
					}

					if (is_list)
					{
						std::string name;

						for (auto it : value_check)
						{
							if (it == ' ')
							{
								break;
							}

							name.push_back(it);
						}

						size_t whole_size = values.size( );
						for (size_t i = 0; i < whole_size; ++i)
						{
							Tree.push_back(name, values.top( ), true);
							values.pop( );
						}
					}
					else
					{
						std::string name;

						for (auto it : value_check)
						{
							if (it == ' ')
								break;

							name.push_back(it);
						}

						if (find<variable>(variables, name))
						{
							find_via<variable>(variables, name).value = values.top( );
							values.pop( );
						}
						else
						{
							variables.push_back({name, values.top( )});
							values.pop( );
						}
					}

					words.push_back(make_word(wrd, true, std::string( )));
					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Push)
				{
					wrd.push_back(' ');
					std::string value_check;
					i++;
					for (;; ++i)
					{
						if (str [i] == '\n')
						{
							row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					if (value_check [0] == ' ')
					{
						value_check.erase(value_check.begin( ));
					}

					for (auto it : value_check)
					{
						if (transliterator(it) != e_Digit && transliterator(it) != e_Letter)
						{
							main_except except;
							except.row = row;
							except.name = "Undeclared value " + wrd;

							throw except;
						}
					}

					bool is_variable = false;

					for (auto it : value_check)
					{
						if (transliterator(it) == e_Letter)
							is_variable = true;
					}

					//e_list
					//пускай у нас будет модификатор, который позволит провернуть это

					if (is_variable)
					{
						//Tree.push_back(value_check, ll( ), false);
						//разобраться с приколом
						//variables.push_back({value_check, INT_MIN});

						if (find<variable>(variables, value_check))
						{
							values.push(find_via<variable>(variables, value_check).value);
						}
						else
						{
							main_except except;
							except.row = row;
							except.name = "Unreachable variable " + wrd;

							throw except;
						}
					}
					else
					{
						values.push(atoi(value_check.c_str( )));
					}

					words.push_back(make_word(wrd, true, std::string( )));

					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Jmp)
				{
					wrd.push_back(' ');
					std::string value_check;
					i++;
					for (;; ++i)
					{
						if (str [i] == '\n')
						{
							row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					if (value_check [0] == ' ')
						value_check.erase(value_check.begin( ));

					if (find<marked>(marks, value_check))
					{
						size_t pos = 0;
						size_t new_row = 0;
						for (; ; ++pos)
						{
							if (new_row == find_via<marked>(marks, value_check).row)
								break;
							if (str [pos] == '\n')
							{
								++new_row;
							}
						}
						--pos;
						i = pos;
						row = new_row;
					}
					else
					{
						size_t pos = 0;
						size_t new_row = 1;
						for (; ; ++pos)
						{
							if (new_row == atoi(value_check.c_str( )))
								break;
							if (str [pos] == '\n')
							{
								++new_row;
							}
						}
						--pos;
						i = pos;
						row = new_row;
					}


					words.push_back(make_word(wrd, true, std::string( )));

					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Read)
				{
					std::cout << "All letters would be translated to digits" << std::endl;

					std::string semi_values;

					std::cin >> semi_values;
					ll semi_stack = 0;

					if (semi_values.size( ) == 1)
					{
						values.push(atoi(semi_values.c_str( )));
					}
					else
					{

						for (size_t i = 0; i < semi_values.size( ); ++i)
						{
							if (semi_values [i] == ' ')
							{
								semi_stack /= 10;
								values.push(semi_stack);
							}
							semi_stack += semi_values [i] - 48;
							semi_stack *= 10;
						}
					}

					words.push_back(make_word(wrd, false, "Read - "));
					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Write)
				{
					wrd.push_back(' ');
					std::string value_check;
					if (str [i + 1] == '\n')
					{
						//std::stack<ll> semi = values;

						/*size_t real_size = semi.size( );

						for (size_t i = 0; i < real_size; ++i)
						{
							for (auto it : List::to_string(semi.top( )))
							{
								value_check.push_back(it);
							}
							if (i < real_size - 1)
								value_check.push_back(',');
							semi.pop( );
						}*/

						if (values.top( ) != 0)
						{
							value_check = List::to_string(values.top( ));

							for (size_t i = 0; i < end_zero_count(values.top( )); ++i)
							{
								value_check.push_back('0');
							}
						}
						else
						{
							value_check.push_back('0');
						}

						words.push_back(make_word("", false, wrd));
						words.push_back(make_word("", false, value_check));
						++i;
						numchar = -1;
					}
					else
					{
						i += 2;
						for (;; ++i)
						{
							if (str [i] == ' ' || str [i] == '\n')
							{
								if (str [i] == '\n')
									row++;
								break;
							}
							value_check.push_back(str [i]);
							wrd.push_back(str [i]);
						}

						if (value_check [0] == ' ')
						{
							value_check.erase(value_check.begin( ));
						}

						if (find<variable>(variables, value_check))
						{
							words.push_back(make_word(wrd, true, std::string( )));

							words.push_back(make_word(List::to_string(find_via<variable>(variables, value_check).value) + "0", false, value_check + ": "));

							numchar = -1;
						}

						else if (!Tree.find(value_check))
						{
							if (find<variable>(variables, value_check))
							{
								words.push_back(make_word(
									List::to_string(find_via<variable>(variables, value_check).value), false, value_check + ": "));

								numchar = -1;
							}
							else
							{
								main_except except;
								except.row = row;
								except.name = "Unreachable variable " + wrd;

								throw except;
							}
						}
						else
						{
							words.push_back(make_word(wrd, true, std::string( )));

							Tree::tree_pair* cur_pair = Tree.find(value_check);

							std::string list_values;

							for (List::set* it = cur_pair->value.get_set( ); it != nullptr; it = it->next)
							{
								std::string semi = List::to_string(it->value);
								for (auto it : semi)
								{
									list_values.push_back(it);
								}
								list_values.push_back(' ');
							}

							words.push_back(make_word(list_values, false, value_check + ": "));
							numchar = -1;
						}
					}
				}

				else if (tokenaze(wrd) == e_End_mark)
				{
					//Одноместная операция
					wrd.push_back(' ');
					std::string value_check;
					if (i != numchar)
						i += 2;
					else
						i++;
					for (;; ++i)
					{
						if (str [i] == ' ' || str [i] == '\n')
						{
							if (str [i] == '\n')
								row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					marks.push_back({value_check, row});

					words.push_back(make_word(wrd, true, std::string( )));

					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Diff)
				{
					//трехместная операция
					//т.е diff x n k
					//делаем diff между x и n
					//сохраняем в k
					//k не будем выводить
					bool fst = true;
					bool snd = true;
					bool end = false;

					std::string fst_variable;
					std::string snd_variable;
					std::string thd_variable;

					wrd.push_back(' ');
					std::string value_check;
					size_t counter = 0;
					if (i != numchar)
						i += 2;
					else
						i++;
					for (;; ++i)
					{
						if (str [i] == ' ')
							counter++;
						if (str [i] == '\n')
						{
							if (str [i] == '\n')
								row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					if (counter > 2)
					{
						main_except except;
						except.row = row;
						except.name = "Unrecognized variable " + wrd;

						throw except;
					}

					for (auto it : value_check)
					{
						if (it == ' ')
						{
							if (fst)
							{
								fst_variable = thd_variable;
								thd_variable.clear( );
								fst = false;
								continue;
							}
							if (snd)
							{
								snd_variable = thd_variable;
								thd_variable.clear( );
								snd = false;
								continue;
							}
						}

						thd_variable.push_back(it);
					}

					if (Tree.find(fst_variable))
					{
						if (Tree.find(snd_variable))
						{
							List::list semi;
							if (!Tree.find(fst_variable)->value.Diff(Tree.find(snd_variable)->value).is_empty( ))
								semi = Tree.find(fst_variable)->value.Diff(Tree.find(snd_variable)->value);
							else
							{
								Tree.push_back(thd_variable, 0, false);
								end = true;
							}

							if (!end)
							{

								std::stack<ll> new_one;
								List::set* it = semi.get_set( );

								for (size_t i = 0; i < semi.len( ); ++i, it = it->next)
								{
									new_one.push(it->value);
								}

								size_t size = new_one.size( );

								if (size > 0)
								{
									for (size_t i = 0; i < size; ++i)
									{
										Tree.push_back(thd_variable, new_one.top( ), true);
										new_one.pop( );
									}
									words.push_back(make_word(wrd, true, std::string( )));
								}
								else
								{
									words.push_back(make_word(wrd, true, std::string( )));
									Tree.push_back(thd_variable, 0, false);
									//words.push_back(make_word(" is empty, can`t save it.", false, thd_variable));
								}
							}
						}

						else
						{
							main_except except;
							except.row = row;
							except.name = "Unrecognized variable " + wrd;

							throw except;
						}
					}
					else
					{
						main_except except;
						except.row = row;
						except.name = "Unrecognized variable " + wrd;

						throw except;
					}

					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Sym)
				{
					bool fst = true;
					bool snd = true;
					bool end = false;

					std::string fst_variable;
					std::string snd_variable;
					std::string thd_variable;

					wrd.push_back(' ');
					std::string value_check;
					size_t counter = 0;
					if (i != numchar)
						i += 2;
					else
						i++;
					for (;; ++i)
					{
						if (str [i] == ' ')
							counter++;
						if (str [i] == '\n')
						{
							if (str [i] == '\n')
								row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					if (counter > 2)
					{
						main_except except;
						except.row = row;
						except.name = "Unrecognized variable " + wrd;

						throw except;
					}

					for (auto it : value_check)
					{
						if (it == ' ')
						{
							if (fst)
							{
								fst_variable = thd_variable;
								thd_variable.clear( );
								fst = false;
								continue;
							}
							if (snd)
							{
								snd_variable = thd_variable;
								thd_variable.clear( );
								snd = false;
								continue;
							}
						}

						thd_variable.push_back(it);
					}

					if (Tree.find(fst_variable))
					{
						if (Tree.find(snd_variable))
						{
							List::list semi;
							if (!Tree.find(fst_variable)->value.Symm_Diff(Tree.find(snd_variable)->value).is_empty( ))
								semi = Tree.find(fst_variable)->value.Symm_Diff(Tree.find(snd_variable)->value);
							else
							{
								Tree.push_back(thd_variable, 0, false);
								end = true;
							}

							if (!end)
							{

								std::stack<ll> new_one;
								List::set* it = semi.get_set( );

								for (size_t i = 0; i < semi.len( ); ++i, it = it->next)
								{
									new_one.push(it->value);
								}

								size_t size = new_one.size( );

								if (size > 0)
								{
									for (size_t i = 0; i < size; ++i)
									{
										Tree.push_back(thd_variable, new_one.top( ), true);
										new_one.pop( );
									}
									words.push_back(make_word(wrd, true, std::string( )));
								}
								else
								{
									words.push_back(make_word(wrd, true, std::string( )));
									Tree.push_back(thd_variable, 0, false);
									//words.push_back(make_word(" is empty, can`t save it.", false, thd_variable));
								}
							}
						}

						else
						{
							main_except except;
							except.row = row;
							except.name = "Unrecognized variable " + wrd;

							throw except;
						}
					}
					else
					{
						main_except except;
						except.row = row;
						except.name = "Unrecognized variable " + wrd;

						throw except;
					}

					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Inter)
				{
					bool fst = true;
					bool snd = true;
					bool end = false;

					std::string fst_variable;
					std::string snd_variable;
					std::string thd_variable;

					wrd.push_back(' ');
					std::string value_check;
					size_t counter = 0;
					if (i != numchar)
						i += 2;
					else
						i++;
					for (;; ++i)
					{
						if (str [i] == ' ')
							counter++;
						if (str [i] == '\n')
						{
							if (str [i] == '\n')
								row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					if (counter > 2)
					{
						main_except except;
						except.row = row;
						except.name = "Unrecognized variable " + wrd;

						throw except;
					}

					for (auto it : value_check)
					{
						if (it == ' ')
						{
							if (fst)
							{
								fst_variable = thd_variable;
								thd_variable.clear( );
								fst = false;
								continue;
							}
							if (snd)
							{
								snd_variable = thd_variable;
								thd_variable.clear( );
								snd = false;
								continue;
							}
						}

						thd_variable.push_back(it);
					}

					if (Tree.find(fst_variable))
					{
						if (Tree.find(snd_variable))
						{
							List::list semi;
							if (!Tree.find(fst_variable)->value.Intersaction(Tree.find(snd_variable)->value).is_empty( ))
								semi = Tree.find(fst_variable)->value.Intersaction(Tree.find(snd_variable)->value);
							else
							{
								Tree.push_back(thd_variable, 0, false);
								end = true;
							}

							if (!end)
							{

								std::stack<ll> new_one;
								List::set* it = semi.get_set( );

								for (size_t i = 0; i < semi.len( ); ++i, it = it->next)
								{
									new_one.push(it->value);
								}

								size_t size = new_one.size( );

								if (size > 0)
								{
									for (size_t i = 0; i < size; ++i)
									{
										Tree.push_back(thd_variable, new_one.top( ), true);
										new_one.pop( );
									}
									words.push_back(make_word(wrd, true, std::string( )));
								}
								else
								{
									words.push_back(make_word(wrd, true, std::string( )));
									Tree.push_back(thd_variable, 0, false);
									//words.push_back(make_word(" is empty, can`t save it.", false, thd_variable));
								}
							}
						}

						else
						{
							main_except except;
							except.row = row;
							except.name = "Unrecognized variable " + wrd;

							throw except;
						}
					}
					else
					{
						main_except except;
						except.row = row;
						except.name = "Unrecognized variable " + wrd;

						throw except;
					}

					numchar = -1;
				}

				else if (tokenaze(wrd) == e_Union)
				{
					bool fst = true;
					bool snd = true;
					bool end = false;

					std::string fst_variable;
					std::string snd_variable;
					std::string thd_variable;

					wrd.push_back(' ');
					std::string value_check;
					size_t counter = 0;
					if (i != numchar)
						i += 2;
					else
						i++;
					for (;; ++i)
					{
						if (str [i] == ' ')
							counter++;
						if (str [i] == '\n')
						{
							if (str [i] == '\n')
								row++;
							break;
						}
						value_check.push_back(str [i]);
						wrd.push_back(str [i]);
					}

					if (counter > 2)
					{
						main_except except;
						except.row = row;
						except.name = "Unrecognized variable " + wrd;

						throw except;
					}

					for (auto it : value_check)
					{
						if (it == ' ')
						{
							if (fst)
							{
								fst_variable = thd_variable;
								thd_variable.clear( );
								fst = false;
								continue;
							}
							if (snd)
							{
								snd_variable = thd_variable;
								thd_variable.clear( );
								snd = false;
								continue;
							}
						}

						thd_variable.push_back(it);
					}

					if (Tree.find(fst_variable))
					{
						if (Tree.find(snd_variable))
						{
							List::list semi;
							if (!Tree.find(fst_variable)->value.Union(Tree.find(snd_variable)->value).is_empty( ))
								semi = Tree.find(fst_variable)->value.Union(Tree.find(snd_variable)->value);
							else
							{
								Tree.push_back(thd_variable, 0, false);
								end = true;
							}

							if (!end)
							{

								std::stack<ll> new_one;
								List::set* it = semi.get_set( );

								for (size_t i = 0; i < semi.len( ); ++i, it = it->next)
								{
									new_one.push(it->value);
								}

								size_t size = new_one.size( );

								if (size > 0)
								{
									for (size_t i = 0; i < size; ++i)
									{
										Tree.push_back(thd_variable, new_one.top( ), true);
										new_one.pop( );
									}
									words.push_back(make_word(wrd, true, std::string( )));
								}
								else
								{
									words.push_back(make_word(wrd, true, std::string( )));
									Tree.push_back(thd_variable, 0, false);
									//words.push_back(make_word(" is empty, can`t save it.", false, thd_variable));
								}
							}
						}

						else
						{
							main_except except;
							except.row = row;
							except.name = "Unrecognized variable " + wrd;

							throw except;
						}
					}
					else
					{
						main_except except;
						except.row = row;
						except.name = "Unrecognized variable " + wrd;

						throw except;
					}

					numchar = -1;
				}

				else if (tokenaze(wrd) == e_TCompare)
				{
					//немного пофиксить


					/*	if (word == "<" && word == ">" || word == "!" || word == "=")
					return e_TCompare;*/

					//Есть два случая
					//<\n - значит мы работаем над стеком
					//<_n_a\n - значит мы работаем над множествами
					size_t counter = 0;
					i++;
					for (;; ++i)
					{
						if (str [i] == '\n')
						{
							if (str [i] == ' ')
								++counter;
							++row;
							break;
						}
						wrd.push_back(str [i]);
					}

					//проверим размер и по нему косвенно узнаем над чем мы работаем

					if (wrd.size( ) == 1)
					{
						//значит мы работаем над числами
						if (transliterator(wrd [0]) == e_Compare)
						{
							std::stack<ll> copy = values;

							ll fst = copy.top( );
							copy.pop( );
							ll snd = copy.top( );
							copy.pop( );

							if (wrd == "=")
							{
								words.push_back(make_word(std::string(), false, wrd));
								if (snd == fst)
								{
									words.push_back(make_word(std::string( ), false, "true"));
									option = true;
								}
								else
								{
									words.push_back(make_word(std::string( ), false, "false"));
								}
							}
							else if (wrd == "!")
							{
								words.push_back(make_word(std::string( ), false, wrd));
								if (snd != fst)
								{
									option = true;
									words.push_back(make_word(std::string( ), false, "true"));
								}
								else
								{
									words.push_back(make_word(std::string( ), false, "false"));
								}
							}
							else if (wrd == "<")
							{
								words.push_back(make_word(std::string( ), false, wrd));
								if (snd < fst)
								{
									option = true;
									words.push_back(make_word(std::string( ), false, "true"));
								}
								else
								{
									words.push_back(make_word(std::string( ), false, "false"));
								}
							}
							else if (wrd == ">")
							{
								words.push_back(make_word(std::string( ), false, wrd));
								if (snd > fst)
								{
									option = true;
									words.push_back(make_word(std::string( ), false, "true"));
								}
								else
								{
									words.push_back(make_word(std::string( ), false, "false"));
								}
							}

						}
						else
						{
							main_except except;
							except.row = row;
							except.name = "Undefined operation " + wrd;

							throw except;
						}



					}
					else //значит мы работаем над множествами или переменными
					{//чтобы решить проблему однозначности, так как вычисления могут происходить как над
						//множествами, так и над переменными, будем считать, что изначально работаем над переменными


						bool fst = true;
						bool snd = true;

						std::string fst_variable;
						std::string snd_variable;
						std::string thd_variable;

						if (counter > 2)
						{
							main_except except;
							except.row = row;
							except.name = "Unrecognized variable " + wrd;

							throw except;
						}

						for (auto it : wrd)
						{
							if (it == ' ')
							{
								if (fst)
								{
									fst_variable = thd_variable;
									thd_variable.clear( );
									fst = false;
									continue;
								}
								if (snd)
								{
									snd_variable = thd_variable;
									thd_variable.clear( );
									snd = false;
									continue;
								}
							}

							thd_variable.push_back(it);
						}

						if (fst_variable == "=")
						{
							words.push_back(make_word(wrd, true, std::string( )));
							if (find<variable>(variables, snd_variable))
							{
								if (find<variable>(variables, thd_variable))
								{
									if (find_via<variable>(variables, snd_variable).value ==
										find_via<variable>(variables, snd_variable).value)
									{
										words.push_back(make_word(std::string( ), false, "true"));
									}
									else
									{
										words.push_back(make_word(std::string( ), false, "false"));
									}
								}
								else if (Tree.find(thd_variable))
								{
									main_except except;
									except.row = row;
									except.name = "Undeclared variable types " + wrd;

									throw except;
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Undefined variable  " + wrd;

									throw except;
								}
							}

							else if (Tree.find(snd_variable))
							{
								if (Tree.find(thd_variable))
								{
									if (Tree.find(snd_variable)->value.operator==(Tree.find(thd_variable)->value))
									{
										words.push_back(make_word(std::string( ), false, "true"));
									}
									else
									{
										words.push_back(make_word(std::string( ), false, "false"));
									}
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Undefined variable " + wrd;

									throw except;
								}
							}
							else
							{
								main_except except;
								except.row = row;
								except.name = "Undefined variable " + wrd;

								throw except;
							}
						}
						else if (fst_variable == "!")
						{
							words.push_back(make_word(wrd, true, std::string( )));
							if (find<variable>(variables, snd_variable))
							{
								if (find<variable>(variables, thd_variable))
								{
									if (find_via<variable>(variables, snd_variable).value !=
										find_via<variable>(variables, snd_variable).value)
									{
										words.push_back(make_word(std::string( ), false, "true"));
									}
									else
									{
										words.push_back(make_word(std::string( ), false, "false"));
									}
								}
								else if (Tree.find(thd_variable))
								{
									main_except except;
									except.row = row;
									except.name = "Undeclared variable types " + wrd;

									throw except;
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Undefined variable  " + wrd;

									throw except;
								}
							}

							else if (Tree.find(snd_variable))
							{
								if (Tree.find(thd_variable))
								{
									if (Tree.find(snd_variable)->value.operator!=(Tree.find(thd_variable)->value))
									{
										words.push_back(make_word(std::string( ), false, "true"));
									}
									else
									{
										words.push_back(make_word(std::string( ), false, "false"));
									}
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Undefined variable " + wrd;

									throw except;
								}
							}
							else
							{
								main_except except;
								except.row = row;
								except.name = "Undefined variable " + wrd;

								throw except;
							}
						}
						else if (fst_variable == "<")
						{
							words.push_back(make_word(wrd, true, std::string( )));
							if (find<variable>(variables, snd_variable))
							{
								if (find<variable>(variables, thd_variable))
								{
									if (find_via<variable>(variables, snd_variable).value <
										find_via<variable>(variables, snd_variable).value)
									{
										words.push_back(make_word(std::string( ), false, "true"));
									}
									else
									{
										words.push_back(make_word(std::string( ), false, "false"));
									}
								}
								else if (Tree.find(thd_variable))
								{
									main_except except;
									except.row = row;
									except.name = "Undeclared variable types " + wrd;

									throw except;
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Undefined variable  " + wrd;

									throw except;
								}
							}
							else if (Tree.find(thd_variable))
							{
								main_except except;
								except.row = row;
								except.name = "Undeclared variable types " + wrd;

								throw except;
							}
							else
							{
								main_except except;
								except.row = row;
								except.name = "Undefined variable  " + wrd;

								throw except;
							}
						}
						else if (fst_variable == ">")
						{
							words.push_back(make_word(wrd, true, std::string( )));
							if (find<variable>(variables, snd_variable))
							{
								if (find<variable>(variables, thd_variable))
								{
									if (find_via<variable>(variables, snd_variable).value >
										find_via<variable>(variables, snd_variable).value)
									{
										words.push_back(make_word(std::string( ), false, "true"));
									}
									else
									{
										words.push_back(make_word(std::string( ), false, "false"));
									}
								}
								else if (Tree.find(thd_variable))
								{
									main_except except;
									except.row = row;
									except.name = "Undeclared variable types " + wrd;

									throw except;
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Undefined variable  " + wrd;

									throw except;
								}
							}
							else if (Tree.find(thd_variable))
							{
								main_except except;
								except.row = row;
								except.name = "Undeclared variable types " + wrd;

								throw except;
							}
							else
							{
								main_except except;
								except.row = row;
								except.name = "Undefined variable  " + wrd;

								throw except;
							}
						}

						else
						{
							main_except except;
							except.row = row;
							except.name = "Undefined variable " + wrd;

							throw except;
						}


					}
					numchar = -1;
				}

				else if (tokenaze(wrd) == e_TOperation)
				{
					/*if (word == "+" || word == "-" || word == "*" || word == "/" || word == "%")
					return e_TOperation;*/

					//данные операции разрешены только над числами в стеке
					i++;
					for (;; ++i)
					{
						if (str [i] == '\n')
						{
							row++;
							break;
						}
						wrd.push_back(str [i]);
					}

					if (wrd.size( ) > 1)
					{
						//допилить для работы над переменными

						std::string op;
						std::string value_check;
						size_t counter = 0;

						for (size_t i = 0; i < wrd.size( ); ++i)
						{
							if (wrd [i] == ' ')
							{
								++i;
								for (; i < wrd.size( ); ++i)
								{
									if (wrd [i] == ' ')
										counter++;
									value_check.push_back(wrd [i]);
								}

								break;
							}
							op.push_back(wrd [i]);
						}

						//мы получили аргументы оператора
						//теперь мы должны распарсить их

						bool fst = true;
						bool snd = true;
						bool end = false;

						std::string fst_variable;
						std::string snd_variable;
						std::string thd_variable;

						if (counter > 2)
						{
							main_except except;
							except.row = row;
							except.name = "Unrecognized variable " + wrd;

							throw except;
						}

						for (auto it : value_check)
						{
							if (it == ' ')
							{
								if (fst)
								{
									fst_variable = thd_variable;
									thd_variable.clear( );
									fst = false;
									continue;
								}
								if (snd)
								{
									snd_variable = thd_variable;
									thd_variable.clear( );
									snd = false;
									continue;
								}
							}

							thd_variable.push_back(it);
						}

						//распарсили выражение
						//теперь нужно проверить нахождение первых двух переменных в векторе переменных 
						//если есть - работаем
						//иначе кидаем исключение

						//почему только двух? по причине того, что последняя переменная изменит свое значение
						//если она есть, иначе мы создадим новую
						//если thd_variable пустой, значит нужно результаты вычислений положить в стек

						//std::vector<variable> variables;
						//std::stacK<ll> values;

						if (find<variable>(variables, fst_variable))
						{
							if (find<variable>(variables, snd_variable))
							{
								words.push_back(make_word(std::string( ), false, wrd)); // по хорошему нужно было сразу предусмотреть логику вывода ошибок
								//чтобы например сначала печаталась строка, затем если что выдавалась ошибка и пользователь мог отреагировать
								if (op == "+")
								{
									if (thd_variable.empty( ))
									{
										values.push(find_via<variable>(variables, fst_variable).value +
											find_via<variable>(variables, snd_variable).value);
									}
									else
									{
										find_via<variable>(variables, thd_variable).value =
											(find_via<variable>(variables, fst_variable).value +
												find_via<variable>(variables, snd_variable).value);

									}
								}
								else if (op == "-")
								{
									if (thd_variable.empty( ))
									{
										values.push(find_via<variable>(variables, fst_variable).value -
											find_via<variable>(variables, snd_variable).value);
									}
									else
									{
										find_via<variable>(variables, thd_variable).value =
											(find_via<variable>(variables, fst_variable).value -
												find_via<variable>(variables, snd_variable).value);
									}
								}
								else if (op == "*")
								{
									if (thd_variable.empty( ))
									{
										values.push(find_via<variable>(variables, fst_variable).value *
											find_via<variable>(variables, snd_variable).value);
									}
									else
									{
										find_via<variable>(variables, thd_variable).value =
											(find_via<variable>(variables, fst_variable).value *
												find_via<variable>(variables, snd_variable).value);
									}
								}
								else if (op == "/")
								{
									if (find_via<variable>(variables, fst_variable).value == 0 ||
										find_via<variable>(variables, snd_variable).value == 0)
									{
										main_except except;
										except.row = row;
										except.name = "One of variables is equal zero " + wrd;

										throw except;
									}
									if (thd_variable.empty( ))
									{
										values.push(find_via<variable>(variables, fst_variable).value /
											find_via<variable>(variables, snd_variable).value);
									}
									else
									{
										find_via<variable>(variables, thd_variable).value =
											(find_via<variable>(variables, fst_variable).value /
												find_via<variable>(variables, snd_variable).value);
									}
								}
								else if (op == "%")
								{
									if (find_via<variable>(variables, fst_variable).value == 0 ||
										find_via<variable>(variables, snd_variable).value == 0)
									{
										main_except except;
										except.row = row;
										except.name = "One of variables is equal zero " + wrd;

										throw except;
									}
									if (thd_variable.empty( ))
									{
										values.push(find_via<variable>(variables, fst_variable).value %
											find_via<variable>(variables, snd_variable).value);
									}
									else
									{
										find_via<variable>(variables, thd_variable).value =
											(find_via<variable>(variables, fst_variable).value %
												find_via<variable>(variables, snd_variable).value);
									}
								}
								else //знаю что не оптимально
								{
									main_except except;
									except.row = row;
									except.name = "Unrecognized operation " + wrd;

									throw except;
								}
							}
							else if (transliterator(snd_variable [0]) == e_Digit)
							{
								words.push_back(make_word(std::string( ), false, wrd));
								bool is_digit = true;

								for (auto it : snd_variable)
								{
									if (transliterator(it) != e_Digit)
										is_digit = false;
								}

								if (is_digit)
								{
									if (op == "+")
									{
										if (thd_variable.empty( ))
										{
											values.push(find_via<variable>(variables, fst_variable).value + atoi(snd_variable.c_str( )));
										}
										else
										{
											find_via<variable>(variables, thd_variable).value =
												find_via<variable>(variables, fst_variable).value + atoi(snd_variable.c_str( ));
										}
									}
									else if (op == "-")
									{
										if (thd_variable.empty( ))
										{
											values.push(find_via<variable>(variables, fst_variable).value - atoi(snd_variable.c_str( )));
										}
										else
										{
											find_via<variable>(variables, thd_variable).value =
												find_via<variable>(variables, fst_variable).value - atoi(snd_variable.c_str( ));
										}
									}
									else if (op == "/")
									{
										if (find_via<variable>(variables, fst_variable).value == 0 ||
											find_via<variable>(variables, snd_variable).value == 0)
										{
											main_except except;
											except.row = row;
											except.name = "One of variables is equal zero " + wrd;

											throw except;
										}
										if (thd_variable.empty( ))
										{
											values.push(find_via<variable>(variables, fst_variable).value / atoi(snd_variable.c_str( )));
										}
										else
										{
											find_via<variable>(variables, thd_variable).value =
												find_via<variable>(variables, fst_variable).value / atoi(snd_variable.c_str( ));
										}
									}
									else if (op == "*")
									{
										if (thd_variable.empty( ))
										{
											values.push(find_via<variable>(variables, fst_variable).value * atoi(snd_variable.c_str( )));
										}
										else
										{
											find_via<variable>(variables, thd_variable).value =
												find_via<variable>(variables, fst_variable).value * atoi(snd_variable.c_str( ));
										}
									}
									else if (op == "%")
									{
										if (find_via<variable>(variables, fst_variable).value == 0 ||
											find_via<variable>(variables, snd_variable).value == 0)
										{
											main_except except;
											except.row = row;
											except.name = "One of variables is equal zero " + wrd;

											throw except;
										}

										if (thd_variable.empty( ))
										{
											values.push(find_via<variable>(variables, fst_variable).value % atoi(snd_variable.c_str( )));
										}
										else
										{
											find_via<variable>(variables, thd_variable).value =
												find_via<variable>(variables, fst_variable).value % atoi(snd_variable.c_str( ));
										}
									}
									else
									{
										main_except except;
										except.row = row;
										except.name = "Unrecognized operation " + wrd;

										throw except;
									}
								}
								else
								{
									main_except except;
									except.row = row;
									except.name = "Unreachable operation, second operand is undefined " + wrd;

									throw except;
								}
							}
							else
							{
								main_except except;
								except.row = row;
								except.name = "Unreachable operation, second operand is missing " + wrd;

								throw except;
							}
						}
						else
						{
							main_except except;
							except.row = row;
							except.name = "Unreachable operation, first operand is missing " + wrd;

							throw except;
						}
					}

					else
					{
						if (values.empty( ))
						{
							main_except except;
							except.row = row;
							except.name = "Unreachable operation, stack is empty " + wrd;

							throw except;
						}
						if (values.size( ) < 2)
						{
							main_except except;
							except.row = row;
							except.name = "Unreachable operation, stack have less than 2 values " + wrd;

							throw except;
						}

						if (transliterator(wrd [0]) == e_Operation)
						{

							ll fst = values.top( );
							values.pop( );
							ll snd = values.top( );
							values.pop( );

							if (wrd == "+")
							{
								words.push_back(make_word(std::string( ), false, wrd));
								fst += snd;
								values.push(fst);
							}
							else if (wrd == "-")
							{
								words.push_back(make_word(std::string( ), false, wrd));
								fst -= snd;
								values.push(fst);
							}
							else if (wrd == "%")
							{
								if (fst * snd == 0)
								{
									main_except except;
									except.row = row;
									except.name = "One of operand equals zero " + wrd;

									throw except;
								}
								words.push_back(make_word(std::string( ), false, wrd));
								fst %= snd;
								values.push(fst);
							}
							else if (wrd == "*")
							{
								words.push_back(make_word(std::string( ), false, wrd));
								fst *= snd;
								values.push(fst);
							}
							else if (wrd == "/")
							{
								if (fst * snd == 0)
								{
									main_except except;
									except.row = row;
									except.name = "One of operand equals zero " + wrd;

									throw except;
								}
								words.push_back(make_word(std::string( ), false, wrd));
								fst /= snd;
								values.push(fst);
							}

						}
						else
						{
							main_except except;
							except.row = row;
							except.name = "Undefined operation " + wrd;

							throw except;
						}
					}
					numchar = -1;
				}
			}

		}

		else if (transliterator(str [i]) == e_Stop)
		{
			words.push_back(make_word("$", false, "End mark - "));
			break;
		}
	}


}

int main( )
{
	std::ifstream stream("Test.txt");

	if (!stream.is_open( ))
	{
		std::cout << "Can`t open file" << std::endl;
		exit(EXIT_FAILURE);
	}

	Tree::BinTree Tree;
	std::string produce;

	stream.seekg(0, std::ios::end);
	produce.reserve(stream.tellg( ));
	stream.seekg(0);

	produce.assign((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>( ));

	try
	{
		trans(produce, Tree);
	}
	catch (main_except except)
	{
		std::cout << "Failure: " << except.name << "\nAt: " << except.row << " row" << std::endl;
		//exit(EXIT_FAILURE);
	}

	stream.close( );

	for (auto it : words)
	{
		std::cout << it.word << it.value << std::endl;
	}




	return 0;
}