#include "SubProgramsStatements.h"

void SubProgramsStatements::subPrograms_statements(){
  subPrograms_statements_2();
}

void SubProgramsStatements::subPrograms_statements_2(){

  if(SyntaticalAnalizer::compareToken("procedure") || SyntaticalAnalizer::compareToken("function")){//se tiver mais subprogramas

    subProgram_Statement();

    if(SyntaticalAnalizer::compareToken(";") ){

      NEXT_TOKEN;
      subPrograms_statements_2();

    }else{
      //erro ';'
      SyntaticalAnalizer::error("expected ';' instead '","SubProgramsStatements.cpp:subProgram_Statement_2()");
    }

  }else{
    //vazio
  }


}

void SubProgramsStatements::subProgram_Statement(){

  if(SyntaticalAnalizer::compareToken("procedure")){

    NEXT_TOKEN;

    if(SyntaticalAnalizer::compareCategory("identificador")){

      //CHECK_SCOPE;//ta dando erro aqui, na checagem de escopo

      SyntaticalAnalizer::getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao
      if(PUSH_ID){//guarda o id do programa na pilha de ids de procedures e verifica se ela ja esta no escopo atual
        //id e unico
      }else{
        SyntaticalAnalizer::error("dubplicate id '","SubProgramsStatements.cpp::subProgram_Statement()");
      }

      END_SCOPE;//encerra o escopo anterior e comeca um novo

      NEXT_TOKEN;
      arguments();

      if(SyntaticalAnalizer::compareToken(";")){

        NEXT_TOKEN;
        if(SyntaticalAnalizer::compareToken("var"))//se tiver declaracao de variavel
          VariableStatement::variables_statements();

        if(SyntaticalAnalizer::compareToken("procedure"))//se tiver declaracao de procedure
          subPrograms_statements();

        composite_command();

      }else{
        //erro ';'
        SyntaticalAnalizer::error("expected ';' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
      }

    }else{
      //erro 'id'
      SyntaticalAnalizer::error("expected procedure 'id' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
    }

  }else if(SyntaticalAnalizer::compareToken("function")){

    NEXT_TOKEN;

    if(SyntaticalAnalizer::compareCategory("identificador")){

      //CHECK_SCOPE;//ta dando erro aqui, na checagem de escopo

      SyntaticalAnalizer::getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao
      if(PUSH_ID){//guarda o id do programa na pilha de ids de procedures e verifica se ela ja esta no escopo atual
        //id e unico
      }else{
        SyntaticalAnalizer::error("dubplicate id '","SubProgramsStatements.cpp::subProgram_Statement()");
      }

      END_SCOPE;//encerra o escopo anterior e comeca um novo

      NEXT_TOKEN;
      arguments();

      if(SyntaticalAnalizer::compareToken(":")){

        NEXT_TOKEN;

        VariableStatement::type();

        if(SyntaticalAnalizer::compareToken(";")){

          NEXT_TOKEN;
          if(SyntaticalAnalizer::compareToken("var"))//se tiver declaracao de variavel
            VariableStatement::variables_statements();

          if(SyntaticalAnalizer::compareToken("procedure") || SyntaticalAnalizer::compareToken("function"))//se tiver declaracao de procedure
            subPrograms_statements();

          composite_command();

        }else{
          //erro ';'
          SyntaticalAnalizer::error("expected ';' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
        }


      }else{
        //erro ':'
        SyntaticalAnalizer::error("expected ':' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
      }

    }else {
      //erro 'id'
      SyntaticalAnalizer::error("expected function 'id' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
    }

  }else{
    //erro 'procedure'
    SyntaticalAnalizer::error("expected 'procedure' or 'function' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
  }

}
void SubProgramsStatements::arguments(){

  if( SyntaticalAnalizer::compareToken( "(" ) ){

    NEXT_TOKEN;
    parameters_list();
    if( SyntaticalAnalizer::compareToken( ")" ) ){
      NEXT_TOKEN;
    }else{
      //erro ')'
      SyntaticalAnalizer::error("expected ')' instead '","SubProgramsStatements.cpp:arguments()");
    }
  }else if(SyntaticalAnalizer::lookahead_category("identificador")){//se o prox for um id, provavel que seja um erro
    //erro '('
    SyntaticalAnalizer::error("expected '(' instead '","SubProgramsStatements.cpp:arguments()");
  }
}

void SubProgramsStatements::parameters_list(){

  VariableStatement::identifiers_list();

  if(SyntaticalAnalizer::compareToken(":")){

    NEXT_TOKEN;

    string t = SyntaticalAnalizer::get_currentToken();//pega o tipo das variaveis declaradas

    VariableStatement::type();

    Stack::setType_ID(VariableStatement::getCount_statement_ID(), t);//seta o tipo de todos os ids declarados
    VariableStatement::reset_count_statement_ID();//zera o contador de IDs

    parameters_list_2();

  }else{
    //erro ':'
    SyntaticalAnalizer::error("expected ':' instead '","SubProgramsStatements.cpp:parameters_list()");
  }

}

void SubProgramsStatements::parameters_list_2(){

  if(SyntaticalAnalizer::compareToken(";")){

    NEXT_TOKEN;
    //gambiarra
    if(SyntaticalAnalizer::compareCategory("identificador")){//se tiver mais declaracao de variavel
      VariableStatement::identifiers_list();
    }else{
      return;//e o caso do vazio
    }

    if(SyntaticalAnalizer::compareToken(":")){

      NEXT_TOKEN;

      string t = SyntaticalAnalizer::get_currentToken();//pega o tipo das variaveis declaradas

      VariableStatement::type();

      Stack::setType_ID(VariableStatement::getCount_statement_ID(), t);//seta o tipo de todos os ids declarados
      VariableStatement::reset_count_statement_ID();//zera o contador de IDs

      parameters_list_2();

    }else{
      //erro ':'
      SyntaticalAnalizer::error("expected ':' instead '","SubProgramsStatements.cpp:parameters_list_2()");
    }

  }else if(SyntaticalAnalizer::compareCategory("identificador")){//se nao colocar ';' e tiver mais identificadores
    //erro ';'
    SyntaticalAnalizer::error("expected ';' instead '","SubProgramsStatements.cpp:parameters_list_2()");
  }else{
    //vazio
  }

}

void SubProgramsStatements::composite_command(){

  if(SyntaticalAnalizer::compareToken("begin")){

    COUNT_BEGIN;
    NEXT_TOKEN;
    optional_commands();

    if(SyntaticalAnalizer::compareToken("end")){
      if(!Stack::pct.empty()){
        Stack::compute_expression( Stack::post_fix_order() );
        Stack::pct.clear();
      }

      NEXT_TOKEN;
      COUNT_END;
      if(CHECK_END_SCOPE){
        POP_SCOPE;
      }else{
        SyntaticalAnalizer::error("don't close scope '","SubProgramsStatements.cpp:composite_command()");
      }

      //nao faz nada
    }else{
      //erro 'end'
      SyntaticalAnalizer::error("expected 'end' instead '","SubProgramsStatements.cpp:composite_command()");
    }

  }
}

void SubProgramsStatements::optional_commands(){
  commands_list();
  //falta ver o caso do vazio
}

void SubProgramsStatements::commands_list(){
  command();
  commands_list_2();
}

void SubProgramsStatements::commands_list_2(){

  if(SyntaticalAnalizer::compareToken(";")){

    vector<Token> a = Stack::post_fix_order();
    string s = Stack::compute_expression( a );
    Stack::pct.clear();

    NEXT_TOKEN;
    command();
    commands_list_2();

  }else{//falta ver o vazio
    //erro ';'
    //SyntaticalAnalizer::error("expected ';' ","SubProgramsStatements.cpp:commands_list_2()");
  }
}

void SubProgramsStatements::command(){

  if(SyntaticalAnalizer::lookahead_token(":=")){//se o prox simbolo for ':=', e o case variable
    //esta funcao segue a descricao da GLC dada, testei de forma diferente para diferenciar o nao-determinismo

    //pega token para comparar tipos :=
    variable();

    Stack::push_in_pct(&(*(SyntaticalAnalizer::getTb_Token() -1)));//emplinha o id
    Stack::push_in_pct(&(*(SyntaticalAnalizer::getTb_Token())));//emplinha o :=

    NEXT_TOKEN;//ja testou o ':=' acima, passa para prox
    expression();


  }else if(SyntaticalAnalizer::compareCategory("identificador")){

    procedure_activation();

  }else if(SyntaticalAnalizer::compareToken("begin")){

    composite_command();

  }else if(SyntaticalAnalizer::compareToken("if")){

    NEXT_TOKEN;
    expression();

    Stack::compute_expression( Stack::post_fix_order() );//avalia a expressao do if
    Stack::pct.clear();//limpa pct

  if(SyntaticalAnalizer::compareToken("then")){

    NEXT_TOKEN;
    command();
    else_part();

  }else{
    //erro 'then'
    SyntaticalAnalizer::error("expected 'then' instead '","SubProgramsStatements.cpp:command()");
  }

  }else if(SyntaticalAnalizer::compareToken("while")){

    NEXT_TOKEN;
    expression();

    Stack::compute_expression( Stack::post_fix_order() );//avalia a expressao do while
    Stack::pct.clear();//limpa pct

    if(SyntaticalAnalizer::compareToken("do")){

      NEXT_TOKEN;
      command();

    }else{
      //erro 'do'
      SyntaticalAnalizer::error("expected 'do' instead '","SubProgramsStatements.cpp:command()");
    }

  }

}

void SubProgramsStatements::command_2(){

  if(SyntaticalAnalizer::compareToken(":=")){

    NEXT_TOKEN;
    expression();
    //checar tipos
  }else{
    procedure_activation_2();
  }
}

void SubProgramsStatements::expression(){

  simple_expression();
  expression_2();

}

void SubProgramsStatements::expression_2(){

  if(SyntaticalAnalizer::compareCategory("operador-relacional")){

    relational_op();

    Stack::push_in_pct(&(*(SyntaticalAnalizer::getTb_Token() -1)));

    simple_expression();

  }else{
    // vazio
  }

}

void SubProgramsStatements::expressions_list(){

  expression();
  expressions_list_2();
}

void SubProgramsStatements::expressions_list_2(){

  if(SyntaticalAnalizer::compareToken(",")){

    NEXT_TOKEN;
    expression();
    expressions_list_2();

  }else{
    //tem que ver o erro e o vazio
    //SyntaticalAnalizer::error("expected ','","SubProgramsStatements.cpp:expressions_list_2()");
  }
}

void SubProgramsStatements::else_part(){

  if(SyntaticalAnalizer::compareToken("else")){

    NEXT_TOKEN;
    command();

  }else{//falta ver o vazio
    //SyntaticalAnalizer::error("expected 'else' ","SubProgramsStatements.cpp:else_part()");
  }

}

void SubProgramsStatements::simple_expression(){

  if(SyntaticalAnalizer::compareToken("+") || SyntaticalAnalizer::compareToken("-")){

    SyntaticalAnalizer::getTb_Token()->setCategory("operador-unario");
    Stack::push_in_pct( &(*SyntaticalAnalizer::getTb_Token()) );

    NEXT_TOKEN;

    term();

    simple_expression_2();

  }else if(check_track(term)){

    simple_expression_2();

  }else{
    SyntaticalAnalizer::error("expected simple_expression instead '","SubProgramsStatements.cpp:simple_expression()");
  }

}

void SubProgramsStatements::simple_expression_2(){

  if(SyntaticalAnalizer::compareCategory("operador-aditivo")){

    additive_op();

    Stack::push_in_pct(&(*(SyntaticalAnalizer::getTb_Token() -1)));

    term();
    simple_expression_2();

  }else{
    //vazio
  }

}

void SubProgramsStatements::relational_op(){

  if(SyntaticalAnalizer::compareCategory("operador-relacional")){

    NEXT_TOKEN;

  }else{
    //erro unknow relational_op
    //SyntaticalAnalizer::error("unknow relational_op ","SubProgramsStatements.cpp:relational_op()");
  }
}

void SubProgramsStatements::term(){
  factor();
  term_2();
}

void SubProgramsStatements::term_2(){

  if(SyntaticalAnalizer::compareCategory("operador-multiplicativo")){

    multiplicative_op();

    Stack::push_in_pct(&(*(SyntaticalAnalizer::getTb_Token() -1)));

    factor();
    term_2();

  }else{
    //vazio
  }
}

void SubProgramsStatements::factor(){

  if(SyntaticalAnalizer::compareCategory("identificador")){

    CHECK_SCOPE;

    Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca na pct

    NEXT_TOKEN;
    factor_2();

  }else if(SyntaticalAnalizer::compareCategory("inteiro")){

    Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca os numeros inteiros na pct
    NEXT_TOKEN;

  }else if(SyntaticalAnalizer::compareCategory("real")){

    Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca os numeros reais na pct
    NEXT_TOKEN;

  }else if(SyntaticalAnalizer::compareCategory("boolean")){

    Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca os booleans na pct
    NEXT_TOKEN;

  // }else if(SyntaticalAnalizer::compareCategory("false")){
  //
  //   NEXT_TOKEN;

  }else if(SyntaticalAnalizer::compareToken("(")){

    Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca na pct

    NEXT_TOKEN;
    expression();

    if(SyntaticalAnalizer::compareToken(")")){

      Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca na pct
      NEXT_TOKEN;

    }else{
      //erro ')'
      SyntaticalAnalizer::error("expected ')' instead '","SubProgramsStatements.cpp:factor()");
    }

  }else if(SyntaticalAnalizer::compareToken("not")){

    Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca na pct

    NEXT_TOKEN;
    factor();

  }

}

void SubProgramsStatements::factor_2(){

  if(SyntaticalAnalizer::compareToken("(")){

    Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca na pct

    NEXT_TOKEN;
    expressions_list();

    if(SyntaticalAnalizer::compareToken(")")){

      Stack::push_in_pct(&(*SyntaticalAnalizer::getTb_Token()));//coloca na pct

      NEXT_TOKEN;

    }else{
      //erro ')'
      SyntaticalAnalizer::error("expected ')' instead '","SubProgramsStatements.cpp:factor_2()");
    }

  }else{//tem que ver o vazio
    //SyntaticalAnalizer::error("expected '('","SubProgramsStatements.cpp:factor_2()");
  }
}

void SubProgramsStatements::additive_op(){

  if(SyntaticalAnalizer::compareCategory("operador-aditivo")){
    NEXT_TOKEN;
  }else{
    //SyntaticalAnalizer::error("unknow additive_op ","SubProgramsStatements.cpp:additive_op()");
  }
}

void SubProgramsStatements::multiplicative_op(){

  if(SyntaticalAnalizer::compareCategory("operador-multiplicativo")){
    NEXT_TOKEN;
  }else{
    //SyntaticalAnalizer::error("unknow multiplicative_op ","SubProgramsStatements.cpp:multiplicative_op()");
  }
}

void SubProgramsStatements::procedure_activation(){

  if(SyntaticalAnalizer::compareCategory("identificador")){

    NEXT_TOKEN;
    procedure_activation_2();
  }
}

void SubProgramsStatements::procedure_activation_2(){

  if(SyntaticalAnalizer::compareToken("(")){

    NEXT_TOKEN;
    expressions_list();
    if(SyntaticalAnalizer::compareToken(")")){

      NEXT_TOKEN;

    }else{
      SyntaticalAnalizer::error("expected ')' instead '","SubProgramsStatements.cpp:procedure_activation_2()");
    }

  }else{
    //vazio
  }
}

void SubProgramsStatements::variable(){

  if(SyntaticalAnalizer::compareCategory("identificador")){

    CHECK_SCOPE;

    //ta retornando null
    // Token *t = Stack::search_and_get_ID(SyntaticalAnalizer::get_currentToken());//pega o token com o seu tipo para pôr na pct
    // Stack::push_in_pct(t);//push variavel na pct

    NEXT_TOKEN;

  }else{
    SyntaticalAnalizer::error("expected variable 'id' instead '","SubProgramsStatements.cpp:variable()");
  }
}

bool SubProgramsStatements::check_track(void(*f) (void)){

  vector<Token>::iterator aux = SyntaticalAnalizer::getTb_Token();
  try{
    f();
  }catch(runtime_error &e){
    SyntaticalAnalizer::setTb_Token(aux);
    return false;
  }
  return true;
}
