#ifndef _Context_Free_Grammar_H__
#define _Context_Free_Grammar_H__

#include "Error.h"
#include "Syntax.h"
#include "Semantic.h"

class Context_Free_Grammar{

private:
    int count_statement_id_;    

public:
    Context_Free_Grammar();
    ~Context_Free_Grammar();
    void variables_statements();
    void variables_statements_list();
    void variables_statements_list_2();
    void identifiers_list();
    void identifiers_list_2();
    void type();
    void subPrograms_statements();
    void subPrograms_statements_2();
    void subProgram_Statement();
    void arguments();
    void parameters_list();
    void parameters_list_2();
    void composite_command();
    void optional_commands();
    void commands_list();
    void commands_list_2();
    void command();
    void command_2();
    void expression();
    void simple_expression();
    void simple_expression_2();
    void expression_2();
    void expressions_list();
    void expressions_list_2();
    void else_part();
    void relational_op();
    void term();
    void term_2();
    void factor();
    void factor_2();
    void signal();
    void additive_op();
    void multiplicative_op();
    void procedure_activation();
    void procedure_activation_2();
    void variable();

    //aux funcions
    inline void count_statement_ID();
    inline int getCount_statement_ID();
    inline void reset_count_statement_ID();
    bool check_track(/*void (Context_Free_Grammar::*func)()*/);

};

inline void Context_Free_Grammar::count_statement_ID(){
    ++count_statement_id_;
}

inline int Context_Free_Grammar::getCount_statement_ID(){
    return count_statement_id_;
}

inline void Context_Free_Grammar::reset_count_statement_ID(){
    count_statement_id_ = 0;
}


#endif
