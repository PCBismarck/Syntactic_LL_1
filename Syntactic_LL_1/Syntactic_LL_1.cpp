//

#include "Syntactic_LL_1.h"

#ifdef _TEST_

int main()
{
	unordered_map<char, set<string>> test = Generator();
	cout << "New Grammar:" << endl;
	for (auto it = test.begin(); it != test.end(); ++it)
	{
		cout << it->first << "->";
		int flag = 0;
		for (auto it_g = it->second.begin(); it_g != it->second.end(); ++it_g)
		{
			if (flag)
				cout << "|";
			else
				flag = 1;
			cout << *it_g;
		}
		cout << endl;
	}

	predictAnalysisTable table;

	vector<char> sentence = { 'd', '+', '(', 'd', '*', 'd', ')', '$'};
	cout << "Test sentence: d+(d*d)" << endl;
	table.analysis(sentence);
	return 0;
}

#endif



void predictAnalysisTable::analysis(vector<char> input)
{
	stk.push_back('$');
	stk.push_back('E');
	int cnt = 0;
	int process = 1;
	while (!stk.empty())
	{

		cout << process++ << ".\t";
		for (int i = 0; i < input.size(); ++i)
		{
			if (i == cnt)
				cout << " ->";
			cout << input[i];
		}
		cout   << "\tStack:\t";
		for (int i = 0; i < stk.size(); ++i)
		{
			cout << stk[i] /*<< ' '*/;
		}

		if (stk.back() == input[cnt])
		{
			cout << "\t\tpop " << input[cnt] << endl;
			++cnt;
			stk.pop_back();
		}
		else
		{
			string generation = get_generation(stk.back(), input[cnt]);
			if (generation.length() == 0)
			{
				cout << "Error Input" << endl;
				break;
			}
			else if (generation == "#")
			{
				cout << "\t\tpop " << stk.back() << endl;
				stk.pop_back();
			}
			else
			{
				cout << "\t\t" << stk.back() << "->" << generation << endl;
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
	vector<char> g;
	for (auto it = get_non_terminator().begin(); it != get_non_terminator().end(); ++it)
		g.push_back(*it);
	
	unordered_map<char, set<char>> first_set, follow_set;
	for (int i = 0; i < g.size(); ++i)
	{
		set<char> temp_set;
		First(g[i], temp_set);
		first_set[g[i]] = temp_set;
		temp_set.clear();
		Follow(g[i], temp_set);
		follow_set[g[i]] = temp_set;
	}


	for (int i =0; i < g.size(); ++i)
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

	vector<char> g;
	for (auto it = get_non_terminator().begin(); it != get_non_terminator().end(); ++it)
		g.push_back(*it);
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

