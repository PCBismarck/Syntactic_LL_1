// Syntactic_LL_1.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#ifndef SYNTACTIC_LL__H
#define SYNTACTIC_LL__H

#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <string>
#include "Generator.h"
#include <iterator>

//#define _TEST_

using namespace std;

class predictAnalysisTable
{
public:
	predictAnalysisTable();
	string get_generation(char nonTerminator, char Terminator);

	bool First(char nonTerminator, set<char>& result);

	bool Follow(char nonTerminator, set<char>& result);

	void analysis(vector<char> input);


private:
	void merge(set<char>& src, set<char>& des);

private:
	unordered_map<string, string> pat;
	vector<char> stk;
};




#endif //SYNTACTIC_LL__H


// TODO: 在此处引用程序需要的其他标头。
