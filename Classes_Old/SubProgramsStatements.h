#ifndef __SubProgramsStatements_H__
#define __SubProgramsStatements_H__

#include "Syntax.h"
#include "VariableStatement.h"
#include "MyStack.h"
#include <vector>

#define NEXT_TOKEN Syntax::nextToken()
#define PUSH_ID MyStack::push(&(*Syntax::getTb_Token()))
#define ID_IS_IN_CURRENT_SCOPE MyStack::search_current_scope(&(*Syntax::getTb_Token()))
#define ID_IS_IN_GLOBAL_SCOPE MyStack::search_all_scope(&(*Syntax::getTb_Token()))
#define CHECK_SCOPE VariableStatement::check_scope()
#define COUNT_BEGIN MyStack::count_begin()
#define COUNT_END MyStack::count_end()
#define CHECK_END_SCOPE MyStack::check_end_scope()
#define POP_SCOPE MyStack::pop_scope()
#define END_SCOPE MyStack::end_scope()

using namespace std;

class SubProgramsStatements{

public:
  static void subPrograms_statements();
  static void subPrograms_statements_2();
  static void subProgram_Statement();
  static void arguments();
  static void parameters_list();
  static void parameters_list_2();
  static void composite_command();
  static void optional_commands();
  static void commands_list();
  static void commands_list_2();
  static void command();
  static void command_2();
  static void expression();
  static void simple_expression();
  static void simple_expression_2();
  static void expression_2();
  static void expressions_list();
  static void expressions_list_2();
  static void else_part();
  static void relational_op();
  static void term();
  static void term_2();
  static void factor();
  static void factor_2();
  static void signal();
  static void additive_op();
  static void multiplicative_op();
  static void procedure_activation();
  static void procedure_activation_2();
  static void variable();
  static bool check_track(void(*f) (void));
};

#endif
