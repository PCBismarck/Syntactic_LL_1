#include "Generator.h"



set<char> terminator = {
	'+',	'-',	'*',	'/',	'(',	')',	'd'
};

set<char> nonTerminator = {
	'E',	'A',	'T',	'B',	'F'
};

set<string> E = {
	"TA"
};

set<string> A = {
	"+TA",	"-TA",	"#"
};

set<string>	T = {
	"FB"
};

set<string> B = {
	"*FB",	"/FB",	"#"
};

set<string> F = {
	"(E)",	"d"
};

unordered_map<char, set<string>> generator = {
	{'E', E},
	{'A', A},
	{'T', T},
	{'B', B},
	{'F', F}
};

bool is_terminator(char id)
{
	return terminator.count(id);
}

bool is_non_terminator(char id)
{
	return nonTerminator.count(id);
}

unordered_map<char, set<string>> Generator()
{
	return generator;
}