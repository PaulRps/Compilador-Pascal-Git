#ifndef __SyntaticalAnalizer_H__
#define __SyntaticalAnalizer_H__

#include "Lexical.h"
#include "File.h"
#include "VariableStatement.h"
#include "SubProgramsStatements.h"
#include "Stack.h"

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>//excecao

using namespace std;

class SyntaticalAnalizer{

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
  static void error(string m, string f);
};

#endif
