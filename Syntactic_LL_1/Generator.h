#ifndef GENERATOR_H
#define GENERATOR_H

#include <unordered_map>
#include <set>
#include <string>

using namespace std;

//写入生成式

bool is_terminator(char id);

bool is_non_terminator(char id);

unordered_map<char, set<string>> Generator();

#endif // !GENERTATOR_H




