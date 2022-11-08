// Syntactic_LL_1.cpp: 定义应用程序的入口点。
//

#include "Syntactic_LL_1.h"

#ifdef _TEST_

int main()
{
	//set<string> E_temp = Generator()['A'];
	//for (set<string>::iterator it = E_temp.begin(); it != E_temp.end(); ++it)
	//	cout << "E" << cnt++ << ":" << *(it) << endl;
	char g[5] = { 'E', 'A', 'T', 'B', 'F' };

	predictAnalysisTable table;
	cout << table.get_generation('B', '$') << endl;


	set<char> E_first, A_first, T_first, B_first, F_first;

	set<char> E_follow, A_follow, T_follow, B_follow, F_follow;

	unordered_map<char, set<char>> first_set = {
	{'E', E_first},
	{'A', A_first},
	{'T', T_first},
	{'B', B_first},
	{'F', F_first}
	};

	unordered_map<char, set<char>> follow_set = {
		{'E', E_follow},
		{'A', A_follow},
		{'T', T_follow},
		{'B', B_follow},
		{'F', F_follow}
	};

	for (int i = 0; i < 5; ++i)
	{
		int cnt = 0;
		if (table.First(g[i], first_set[g[i]]))
			for (set<char>::iterator it = first_set[g[i]].begin(); it != first_set[g[i]].end(); ++it)
				cout << g[i] << cnt++ << ":" << (*it) << '\t';
		cout << endl;
	}

	cout << endl;

	for (int i = 0; i < 5; ++i)
	{
		int cnt = 0;
		if (table.Follow(g[i], follow_set[g[i]]))
			for (set<char>::iterator it = follow_set[g[i]].begin(); it != follow_set[g[i]].end(); ++it)
				cout << g[i] << cnt++ << ":" << (*it) << '\t';
		cout << endl;
	}

	vector<char> sentence = { 'd', '+', 'd', '*', 'd', '$'};
	cout << "Test sentence: d+(d*d)" << endl;
	table.analysis(sentence);

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


void predictAnalysisTable::analysis(vector<char> input)
{
	stk.push_back('$');
	stk.push_back('E');
	int cnt = 0;
	int process = 0;
	while (!stk.empty())
	{
		cout << "Current Stack: ";
		for (int i = 0; i < stk.size(); ++i)
		{
			cout << stk[i] << ' ';
		}
		cout << endl;

		if (stk.back() == input[cnt])
		{
			++cnt;
			stk.pop_back();
		}
		else
		{
			string generation = get_generation(stk.back(), input[cnt]);
			if (generation.length() == 0)
				cout << "Error Input" << endl;
			else if(generation == "#")
				stk.pop_back();
			else
			{
				stk.pop_back();
				for (string::reverse_iterator it = generation.rbegin(); it != generation.rend(); ++it)
					stk.push_back(*it);
			}
		}
	}
	if (stk.empty())
		cout << "Accept!" << endl;
}


predictAnalysisTable::predictAnalysisTable()
{
	char g[5] = { 'E', 'A', 'T', 'B', 'F' };

	set<char> E_first, A_first, T_first, B_first, F_first;

	set<char> E_follow, A_follow, T_follow, B_follow, F_follow;

	unordered_map<char, set<char>> first_set = {
		{'E', E_first},
		{'A', A_first},
		{'T', T_first},
		{'B', B_first},
		{'F', F_first}
	};

	unordered_map<char, set<char>> follow_set = {
		{'E', E_follow},
		{'A', A_follow},
		{'T', T_follow},
		{'B', B_follow},
		{'F', F_follow}
	};

	for (int i = 0 ; i < 5; ++i)
	{
		First(g[i], first_set[g[i]]);
		Follow(g[i], follow_set[g[i]]);
	}

	for (int i =0; i < 5; ++i)
	{
		for (set<char>::iterator it = first_set[g[i]].begin(); it != first_set[g[i]].end(); ++it)
		{
			string key;
			key.push_back(g[i]);
			key.push_back((*it));
			set<string> generation =Generator()[g[i]];
			for (set<string>::iterator it_g = generation.begin(); it_g != generation.end(); ++it_g)
			{
				int pos = 0;
				for (int cnt = 0; cnt <= pos; ++cnt)
				{
					if (is_terminator((*it_g)[cnt]))
					{
						if ((*it_g)[cnt] != (*it))
							break;
						pat[key] = *it_g;
					}
					else if (is_non_terminator((*it_g)[cnt]))
					{
						if (first_set[(*it_g)[cnt]].count((*it)))
							pat[key] = (* it_g);
						else if(first_set[(*it_g)[cnt]].count('#'))
							++pos;
					}
				}
			}
		}

		if (first_set[g[i]].count('#')) 
		{
			for (set<char>::iterator it = follow_set[g[i]].begin(); it != follow_set[g[i]].end(); ++it)
			{
				string key;
				key.push_back(g[i]);
				key.push_back((*it));
				pat[key] = '#';
			}
		}
	}
}

string predictAnalysisTable::get_generation(char nonTerminator, char Terminator)
{
	string key;
	key.push_back(nonTerminator); 
	key.push_back(Terminator);
	return pat[key];
}

bool predictAnalysisTable::First(char nonTerminator, set<char>& result)
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

bool predictAnalysisTable::Follow(char nonTerminator, set<char>& result)
{
	if (!is_non_terminator(nonTerminator))
		return NULL;

	char g[5] = { 'E', 'A', 'T', 'B', 'F' };
	if (nonTerminator == 'E')
		result.insert('$');
	for (int i = 0; i < 5; ++i)
	{
		set<string> generation = Generator()[g[i]];
		for (auto it = generation.begin(); it != generation.end(); ++it)//非终结符的每个产生式
		{
			//产生式中的每个字符
			for (int cnt = 0; cnt < (*it).length(); ++cnt)
			{
				if ((*it)[cnt] == nonTerminator)
				{
					bool is_end = false;

					while (!is_end)
					{
						if (cnt == (*it).length() - 1)//是生成式的最后一个字符
						{
							if (nonTerminator == g[i])//且生成式左侧不是当前待求非终结符
								break;
							set<char> pre_nonTerminator;
							Follow(g[i], pre_nonTerminator);
							merge(pre_nonTerminator, result);
							is_end = true;
						}
						else if (cnt >= (*it).length())
							is_end = true;
						else if (is_terminator((*it)[cnt + 1]))//下一位是终结符
						{
							result.insert((*it)[cnt + 1]);
							is_end = true;
						}
						else if (is_non_terminator((*it)[cnt+1]))//下一位是非终结符
						{
							set<char> next_first;
							First((*it)[cnt + 1], next_first);
							if (next_first.count('#'))//有空
							{
								next_first.erase('#');//看下一位
								++cnt;
							}
							else
								is_end = true;
							merge(next_first, result);
						}
					}
				}
			}
		}
	}


	return true;
}

void predictAnalysisTable::merge(set<char>& src, set<char>& des)
{
	for (set<char>::iterator it = src.begin(); it != src.end(); ++it)
		des.insert(*it);
}

