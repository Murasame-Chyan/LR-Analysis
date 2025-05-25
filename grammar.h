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
	string production;			// һ�������ʽ�����ո�
	vector<string> tokens;	// ��ɲ���ʽ��tokens
};

class LR_Grammar {
public:
	map<string, vector<Production>> grammar;		// �ķ��ָ� ���ս��-�������ʽ
	map<string, set<string>> first;							// first��
	map<string, set<string>> follow;						// follow��

public:
	void grammar_clr_Val();
	bool parseGrammar(const CString&);				// �ķ��ָ�
	bool parseFirst();												// First������
	bool parseFollow();												// Follow������
};

string trim(const string&);
Production process_production(const string&);