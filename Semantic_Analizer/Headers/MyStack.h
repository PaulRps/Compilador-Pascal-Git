#ifndef __MyStack_H__
#define __MyStack_H__

// #include <string>
#include <vector>
//#include <iostream>
#include <stack>
// #include <ctype.h>
#include <stdexcept>

//#include "Syntax.h"
#include "Token.h"

using namespace std;

class MyStack{
private:
  static int topStack_;
  static int baseStack_;
  static int count_begin_end_;
  static vector<Token> scopes_;//empilha as variaveis para checagem de escopo
  static vector<Token> PcT_;//empliha as variaveis e os valores para checagem de tipos

public:

  static void init_MyStack();
  static bool push(Token *id);
  static bool pop();
  static bool pop_scope();
  static bool isEmpty();
  static bool PcT_isEmpty();
  static void PcT_clear();
  static void end_scope();
  static bool search_all_scope(Token *id);
  static bool search_current_scope(Token *id);
  static bool push_in_PcT(Token *id);

  static void count_begin();
  static void count_end();
  static bool check_end_scope();
  static void setType_ID(int qtd_id, string type);
  static string compute_expression(vector<Token> expression);
  static string throw_error_type(Token *op1, Token *op2, Token *op);
  static string check_type(Token *op1, Token *op2, Token *op);
  static Token* search_and_get_ID(string id);
  static vector<Token> post_fix_order();
};

#endif
