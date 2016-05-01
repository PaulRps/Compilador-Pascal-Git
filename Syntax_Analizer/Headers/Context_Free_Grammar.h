#ifndef _Context_Free_Grammar_H__
#define _Context_Free_Grammar_H__

#include "Error.h"
#include "Syntax.h"
#include "MyStack.h"


class Context_Free_Grammar{

private:
  static int count_statement_id_;

public:
  static void variables_statements();
  static void variables_statements_list();
  static void variables_statements_list_2();
  static void identifiers_list();
  static void identifiers_list_2();
  static void type();
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

  //aux funcions
  static void count_statement_ID();
  static int getCount_statement_ID();
  static void reset_count_statement_ID();
  static bool check_track(void(*f) (void));

};

#endif
