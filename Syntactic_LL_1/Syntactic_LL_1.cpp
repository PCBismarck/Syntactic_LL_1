// Syntactic_LL_1.cpp: 定义应用程序的入口点。
//

#include "Syntactic_LL_1.h"
#include "Generator.h"

void merge(set<char> &des, set<char> &src);

#ifdef _TEST_

int main()
{
	//set<string> E_temp = Generator()['A'];
	//for (set<string>::iterator it = E_temp.begin(); it != E_temp.end(); ++it)
	//	cout << "E" << cnt++ << ":" << *(it) << endl;
	
	char g[5] = { 'E', 'A', 'T', 'B', 'F' };
	
	set<char> E_first;
	set<char> A_first;
	set<char> T_first;
	set<char> B_first;
	set<char> F_first;

	unordered_map<char, set<char>> first_set = {
		{'E', E_first},
		{'A', A_first},
		{'T', T_first},
		{'B', B_first},
		{'F', F_first}
	};
	for (int i = 0; i < 5; ++i)
	{
		int cnt = 0;
		if (First(g[i], first_set[g[i]]))
			for (set<char>::iterator it = first_set[g[i]].begin(); it != first_set[g[i]].end(); ++it)
				cout << g[i] << cnt++ << ":" << (*it) << '\t';
		cout << endl;
	}


	return 0;
}

#endif

#ifndef _TEST_

int main(void)
{
	cout << "No test!" << endl;
	return 0;
}

#endif

bool First(char nonTerminator, set<char>& result)
{
	if (!is_non_terminator(nonTerminator))
		return false;
	set<string> generation = Generator()[nonTerminator];
	for (set<string>::iterator it = generation.begin(); it != generation.end(); ++it)
	{
		int pos = 0;
		for (int cnt = 0; cnt <= pos; ++cnt)
		{
			if (is_terminator((*it)[cnt]))
				result.insert((*it)[cnt]);
			else if (is_non_terminator((*it)[cnt]))
			{
				set<char> next_nonTerminator;
				if (First((*it)[cnt], next_nonTerminator))
				{
					if (next_nonTerminator.count('#'))
					{
						++pos;
						next_nonTerminator.erase('#');
					}
					merge(next_nonTerminator, result);
				}
			}
			else if ((*it)[cnt] == '#')
				result.insert('#');
		}
	}
	return true;
}

bool Follow(char nonTerminator, set<char>& result)
{
	if (!is_non_terminator(nonTerminator))
		return NULL;
	set<string> generation = Generator()[nonTerminator];
}

void merge(set<char> &src, set<char> &des)
{
	for (set<char>::iterator it = src.begin(); it != src.end(); ++it)
		des.insert(*it);
}