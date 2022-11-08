#include "Generator.h"



set<char> terminator = {
	'+',	'-',	'*',	'/',	'(',	')',	'd'
};

set<char> nonTerminator = {
	'E',	/*'A',*/	'T',	/*'B',*/	'F'
};

//set<string> E = {
//	"TA"
//};
//
//set<string> A = {
//	"+TA",	"-TA",	"#"
//};
//
//set<string>	T = {
//	"FB"
//};
//
//set<string> B = {
//	"*FB",	"/FB",	"#"
//};
//
//set<string> F = {
//	"(E)",	"d"
//};
//
//unordered_map<char, set<string>> generator = {
//	{'E', E},
//	{'A', A},
//	{'T', T},
//	{'B', B},
//	{'F', F}
//};

set<string> E = {
	"E+T", "E-T", "T"
};


set<string>	T = {
	"T*F", "T/F", "F"
};

set<string> F = {
	"(E)",	"d"
};

unordered_map<char, set<string>> pre_generator = {
	{'E', E},
	{'T', T},
	{'F', F}
};

generator g;


bool is_terminator(char id)
{
	return terminator.count(id);
}

bool is_non_terminator(char id)
{
	return nonTerminator.count(id);
}

std::set<char>& get_non_terminator()
{
	return nonTerminator;
}

std::set<char>& get_terminator()
{
	return terminator;
}

unordered_map<char, set<string>> generator::get_grammar()
{
	return grammar;
}

bool generator::reduce_left_recursion(unordered_map<char, set<string>>&src, unordered_map<char, set<string>>&des)
{
	//对原语法的每个产生式进行处理
	for (auto it = src.begin(); it != src.end(); ++it)
	{
		set<string> generation = it->second;
		set<string> new_g = {"#"};
		set<string> pre_g;
		bool r_exist = false;
		//对每个非终结符的产生式
		for (auto it_g = generation.begin(); it_g != generation.end(); ++it_g)
		{
			int pos = 0;
			if (it->first == (*it_g)[pos])//存在左递归
			{
				string new_key = (*it_g).substr(1); 
				new_key.push_back(tolower(it->first));
				new_g.insert(new_key);
				r_exist = true;
			}
			else
				pre_g.insert((*it_g));//不含左递归则完全复制到该非终结符对应的集合中
			
		}
		if (r_exist)//存在左递归，则对加入到集合中的生成式进行处理
		{
			set<string> processed_g;
			for (auto it_g = pre_g.begin(); it_g != pre_g.end(); ++it_g)
			{
				string new_key = (*it_g);
				new_key.push_back(tolower(it->first));
				processed_g.insert(new_key);
			}
			des[it->first] = processed_g;
			des[tolower(it->first)] = new_g;
			nonTerminator.insert(tolower(it->first));
		}
		else
			des[it->first] = pre_g;
	}
	return true;
}

generator::generator()
{
	reduce_left_recursion(pre_generator, grammar);
}

unordered_map<char, set<string>> Generator()
{
	return g.get_grammar();
}
