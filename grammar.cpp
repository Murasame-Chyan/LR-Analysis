#include "pch.h"
#include "grammar.h"

using namespace std;



bool LR_Grammar::parseGrammar(const CString& c_rule)
{
  // 按行分割规则
  vector<string> lines;
  CString token;
  string get_Line, rule = CT2A(c_rule);
  stringstream stream(rule);
  while (getline(stream, get_Line))
  {
    // 去掉可能的'\r'字符（Windows行结束符）
    if (!get_Line.empty() && get_Line.back() == '\r')   get_Line.pop_back();
    lines.push_back(get_Line);
  }

  // 解析每一行规则
  for (const auto& s_Line : lines)
  {
    CString c_line = CString(s_Line.c_str());

    int c_pos = c_line.Find(_T("->"));
    if (c_pos != -1)
    { // ******此处非必要性使用了CString类型，更改下会更好，减少类型转换可能错误******
      CString left = c_line.Left(c_pos).Trim();
      CString right = c_line.Mid(c_pos + 2).Trim();

      // 按'|'分割多个产生式, 按空格分割产生式内多个token 注意单数复数两个production区别 
      // 目标：填充ps，塞到grammar.second中
      vector<Production> ps;
      string production = CT2A(right);
      production = trim(production);
      while (!production.empty())
      {
        Production productions;
        int s_pos = production.find('|');
        if (s_pos != string::npos)     // '|'还存在时
        { // 用production语句给结构体Productions赋值
          productions.production = production.substr(0, s_pos);
          string temp = productions.production;
          productions = process_production(temp);
          // 得到'|'分割的后续产生式
          production = production.substr(s_pos + 1);
          ps.push_back(productions);
        }
        else                              // 只剩一个产生式
        {
          productions = process_production(production);
          ps.push_back(productions);
          production.clear();             // 跳出循环
        }
      }

      // 存储到grammar
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
  if (grammar.empty())  // 不可处理，跳出
    return false;

  bool changed = false; // first集被改变标志
  do {
    // 遍历文法中的每个非终结符的每个产生式
    for (const auto& entry : grammar)
    {
      const string& left = entry.first;
      const vector<Production>& productions = entry.second;

      for (const auto& production : productions)
      { // 1. 产生式空 尝试插入空符@ 有变化
        

      } // end production : productions
    } // end entry : grammar
  } while (changed);
  return true;
}

void LR_Grammar::grammar_clr_Val()
{
  grammar.clear();
}

// 自定义 trim 函数来去掉字符串首尾的空格
string trim(const string& str) {
  size_t start = str.find_first_not_of(" \t\n\r"); // 找到第一个非空白字符的位置
  if (start == string::npos) return ""; // 如果全是空白字符，返回空字符串
  size_t end = str.find_last_not_of(" \t\n\r"); // 找到最后一个非空白字符的位置
  return str.substr(start, end - start + 1); // 返回去掉首尾空白字符的字符串
}
// 处理 production 减少代码耦合
Production process_production(const string& production)
{
  Production productions;
  if (production.size() == 0)
    productions.production = "";  // 缺少对tokens的填充，是否有问题？

  productions.production = trim(production);
  string temp = productions.production;
  int s_pos_whitespace = temp.find(' ');
  do {  // 自身式子中按空格分隔出token，最后一个token后可能有空格也可能没有
    productions.tokens.push_back(temp.substr(0, s_pos_whitespace));
    temp = temp.substr(s_pos_whitespace + 1);
    s_pos_whitespace = temp.find(' ');
  } while (s_pos_whitespace != string::npos);
  if (!temp.empty())
    productions.tokens.push_back(temp);

  return productions;
}