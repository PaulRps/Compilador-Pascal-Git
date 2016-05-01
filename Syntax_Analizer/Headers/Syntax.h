#ifndef __Syntax_H__
#define __Syntax_H__

#include "Lexical.h"
#include "File.h"
#include "Context_Free_Grammar.h"
#include "MyStack.h"

// #include <string>
// #include <iostream>
// #include <vector>
#include <stdexcept>//excecao

using namespace std;

class Syntax{

private:
  static vector<Token>::iterator tb_token_;
  static vector<Token>::iterator endTb_token_;

public:
  static bool compareToken(string t);
  static bool compareCategory(string c);
  static vector<Token>::iterator getTb_Token();
  static string get_currentToken();
  static void setTb_Token(vector<Token>::iterator it);
  static void init_tbTokens();
  static bool lookahead_token(string t);
  static bool lookahead_category(string c);
  static void program();
  static void nextToken();
  static void check_scope();
  static void error(string m, string f);
};

#endif
