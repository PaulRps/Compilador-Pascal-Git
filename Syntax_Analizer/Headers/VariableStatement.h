#ifndef __VariableStatement_H__
#define __VariableStatement_H__

#define NEXT_TOKEN SyntaticalAnalizer::nextToken()
#define PUSH_ID Stack::push(&(*SyntaticalAnalizer::getTb_Token()))
#define ID_IS_IN_CURRENT_SCOPE Stack::search_current_scope(&(*SyntaticalAnalizer::getTb_Token()))
#define ID_IS_IN_GLOBAL_SCOPE Stack::search_all_scope(&(*SyntaticalAnalizer::getTb_Token()))
#define AMOUNT_IDs getCount_statement_ID()


#include "SyntaticalAnalizer.h"
#include "Stack.h"

using namespace std;

class VariableStatement{

private:
  static int count_statement_id_;

public:
  static void variables_statements();
  static void variables_statements_list();
  static void variables_statements_list_2();
  static void identifiers_list();
  static void identifiers_list_2();
  static void type();
  static void check_scope();
  static void count_statement_ID();
  static int getCount_statement_ID();
  static void reset_count_statement_ID();
};

#endif
