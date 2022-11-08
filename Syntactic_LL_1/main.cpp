#include "Syntactic_LL_1.h"
#include <vector>

using namespace std;

#ifndef _TEST_

int main(void)
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
	cout << "Input note stream: " << endl;
	string input;
	cin >> input;
	vector<char> sentence;
	predictAnalysisTable table;
	for (int i = 0; i < input.length(); ++i)
		sentence.push_back(input[i]);
	sentence.push_back('$');
	table.analysis(sentence);
	return 0;
}

#endif