#include "pch.h"
#include "grammar.h"

using namespace std;



bool LR_Grammar::parseGrammar(const CString& c_rule)
{
  // ���зָ����
  vector<string> lines;
  CString token;
  string get_Line, rule = CT2A(c_rule);
  stringstream stream(rule);
  while (getline(stream, get_Line))
  {
    // ȥ�����ܵ�'\r'�ַ���Windows�н�������
    if (!get_Line.empty() && get_Line.back() == '\r')   get_Line.pop_back();
    lines.push_back(get_Line);
  }

  // ����ÿһ�й���
  for (const auto& s_Line : lines)
  {
    CString c_line = CString(s_Line.c_str());

    int c_pos = c_line.Find(_T("->"));
    if (c_pos != -1)
    { // ******�˴��Ǳ�Ҫ��ʹ����CString���ͣ������»���ã���������ת�����ܴ���******
      CString left = c_line.Left(c_pos).Trim();
      CString right = c_line.Mid(c_pos + 2).Trim();

      // ��'|'�ָ�������ʽ, ���ո�ָ����ʽ�ڶ��token ע�ⵥ����������production���� 
      // Ŀ�꣺���ps������grammar.second��
      vector<Production> ps;
      string production = CT2A(right);
      production = trim(production);
      while (!production.empty())
      {
        Production productions;
        int s_pos = production.find('|');
        if (s_pos != string::npos)     // '|'������ʱ
        { // ��production�����ṹ��Productions��ֵ
          productions.production = production.substr(0, s_pos);
          string temp = productions.production;
          productions = process_production(temp);
          // �õ�'|'�ָ�ĺ�������ʽ
          production = production.substr(s_pos + 1);
          ps.push_back(productions);
        }
        else                              // ֻʣһ������ʽ
        {
          productions = process_production(production);
          ps.push_back(productions);
          production.clear();             // ����ѭ��
        }
      }

      // �洢��grammar
      string s_left = CT2A(left);
      grammar[s_left] = ps;
    }
  }

  if (grammar.empty())
    return false;
  return true;
}

bool LR_Grammar::parseFirst()
{
  if (grammar.empty())  // ���ɴ�������
    return false;

  bool changed = false; // first�����ı��־
  do {
    // �����ķ��е�ÿ�����ս����ÿ������ʽ
    for (const auto& entry : grammar)
    {
      const string& left = entry.first;
      const vector<Production>& productions = entry.second;

      for (const auto& production : productions)
      { // 1. ����ʽ�� ���Բ���շ�@ �б仯
        

      } // end production : productions
    } // end entry : grammar
  } while (changed);
  return true;
}

void LR_Grammar::grammar_clr_Val()
{
  grammar.clear();
}

// �Զ��� trim ������ȥ���ַ�����β�Ŀո�
string trim(const string& str) {
  size_t start = str.find_first_not_of(" \t\n\r"); // �ҵ���һ���ǿհ��ַ���λ��
  if (start == string::npos) return ""; // ���ȫ�ǿհ��ַ������ؿ��ַ���
  size_t end = str.find_last_not_of(" \t\n\r"); // �ҵ����һ���ǿհ��ַ���λ��
  return str.substr(start, end - start + 1); // ����ȥ����β�հ��ַ����ַ���
}
// ���� production ���ٴ������
Production process_production(const string& production)
{
  Production productions;
  if (production.size() == 0)
    productions.production = "";  // ȱ�ٶ�tokens����䣬�Ƿ������⣿

  productions.production = trim(production);
  string temp = productions.production;
  int s_pos_whitespace = temp.find(' ');
  do {  // ����ʽ���а��ո�ָ���token�����һ��token������пո�Ҳ����û��
    productions.tokens.push_back(temp.substr(0, s_pos_whitespace));
    temp = temp.substr(s_pos_whitespace + 1);
    s_pos_whitespace = temp.find(' ');
  } while (s_pos_whitespace != string::npos);
  if (!temp.empty())
    productions.tokens.push_back(temp);

  return productions;
}