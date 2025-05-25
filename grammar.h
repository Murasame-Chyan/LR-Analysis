#pragma once

#include "pch.h"
#include <map>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>

using namespace std;

struct Production {
	string production;			// 一整块产生式（含空格）
	vector<string> tokens;	// 组成产生式的tokens
};

class LR_Grammar {
public:
	map<string, vector<Production>> grammar;		// 文法分割 非终结符-多个产生式
	map<string, set<string>> first;							// first集
	map<string, set<string>> follow;						// follow集

public:
	void grammar_clr_Val();
	bool parseGrammar(const CString&);				// 文法分割
	bool parseFirst();												// First集处理
	bool parseFollow();												// Follow集处理
};

string trim(const string&);
Production process_production(const string&);