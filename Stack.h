#ifndef __Stack_H__
#define __Stack_H__

#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include "Token.h"
#include <ctype.h>
#include <stdexcept>
#include "SyntaticalAnalizer.h"

using namespace std;

class Stack{
private:
  static int topStack;
  static int baseStack;
  static int baseStack_procedureID;
  static int count_begin_end;
  static vector<Token> scopes;//empilha as variaveis para checagem de escopo
  static vector<Token> stack_procedures_ID;//guarda os ids de procedures e funcoes
  //static vector<Token> pct;//empliha as variaveis e os valores para checagem de tipos
  static vector<string> stack_operators;//empilha os operadores para checagem de tipos

public:
static vector<Token> pct;

  static void init_stack();
  static bool push(Token *id);
  static bool push_proceduresID(Token *id);
  static bool pop();
  static bool pop_scope();
  static bool isEmpty();
  static void end_scope();
  static bool search_all_scope(Token *id);
  static bool search_all_scope_proceduresID(string id);
  static bool search_current_scope(Token *id);
  static bool search_current_scope_proceduresID(string id);
  static void count_begin();
  static void count_end();
  static bool check_end_scope();
  static void setType_ID(int qtd_id, string type);
  static bool push_in_pct(Token *id);
  static bool push_in_stackOperators(string op);
  static string compute_expression(vector<Token> expression);
  static string throw_error_type(Token *op1, Token *op2, Token *op);
  static string check_type(Token *op1, Token *op2, Token *op);
  static Token* search_and_get_ID(string id);
  static vector<Token> post_fix_order();
  static void show();
};

#endif
