#include "SubProgramsStatements.h"

void SubProgramsStatements::subPrograms_statements(){
  subPrograms_statements_2();
}

void SubProgramsStatements::subPrograms_statements_2(){

  if(Syntax::compareToken("procedure") || Syntax::compareToken("function")){//se tiver mais subprogramas

    subProgram_Statement();

    if(Syntax::compareToken(";") ){

      NEXT_TOKEN;
      subPrograms_statements_2();

    }else{
      //erro ';'
      Syntax::error("expected ';' instead '","SubProgramsStatements.cpp:subProgram_Statement_2()");
    }

  }else{
    //vazio
  }


}

void SubProgramsStatements::subProgram_Statement(){

  if(Syntax::compareToken("procedure")){

    NEXT_TOKEN;

    if(Syntax::compareCategory("identificador")){

      Syntax::getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao
      if(PUSH_ID){//guarda o id do programa na pilha de ids de procedures e verifica se ela ja esta no escopo atual
        //id e unico
      }else{
        Syntax::error("dubplicate id '","SubProgramsStatements.cpp::subProgram_Statement()");
      }

      END_SCOPE;//encerra o escopo anterior e comeca um novo

      NEXT_TOKEN;
      arguments();

      if(Syntax::compareToken(";")){

        NEXT_TOKEN;
        if(Syntax::compareToken("var"))//se tiver declaracao de variavel
          VariableStatement::variables_statements();

        if(Syntax::compareToken("procedure"))//se tiver declaracao de procedure
          subPrograms_statements();

        composite_command();

      }else{
        //erro ';'
        Syntax::error("expected ';' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
      }

    }else{
      //erro 'id'
      Syntax::error("expected procedure 'id' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
    }

  }else if(Syntax::compareToken("function")){

    NEXT_TOKEN;

    if(Syntax::compareCategory("identificador")){

      Syntax::getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao
      if(PUSH_ID){//guarda o id do programa na pilha de ids de procedures e verifica se ela ja esta no escopo atual
        //id e unico
      }else{
        Syntax::error("dubplicate id '","SubProgramsStatements.cpp::subProgram_Statement()");
      }

      END_SCOPE;//encerra o escopo anterior e comeca um novo

      NEXT_TOKEN;
      arguments();

      if(Syntax::compareToken(":")){

        NEXT_TOKEN;

        VariableStatement::type();

        if(Syntax::compareToken(";")){

          NEXT_TOKEN;
          if(Syntax::compareToken("var"))//se tiver declaracao de variavel
            VariableStatement::variables_statements();

          if(Syntax::compareToken("procedure") || Syntax::compareToken("function"))//se tiver declaracao de procedure
            subPrograms_statements();

          composite_command();

        }else{
          //erro ';'
          Syntax::error("expected ';' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
        }


      }else{
        //erro ':'
        Syntax::error("expected ':' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
      }

    }else {
      //erro 'id'
      Syntax::error("expected function 'id' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
    }

  }else{
    //erro 'procedure'
    Syntax::error("expected 'procedure' or 'function' instead '","SubProgramsStatements.cpp:subProgram_Statement()");
  }

}

void SubProgramsStatements::arguments(){

  if( Syntax::compareToken( "(" ) ){

    NEXT_TOKEN;
    parameters_list();
    if( Syntax::compareToken( ")" ) ){
      NEXT_TOKEN;
    }else{
      //erro ')'
      Syntax::error("expected ')' instead '","SubProgramsStatements.cpp:arguments()");
    }
  }else if(Syntax::lookahead_category("identificador")){//se o prox for um id, provavel que seja um erro
    //erro '('
    Syntax::error("expected '(' instead '","SubProgramsStatements.cpp:arguments()");
  }
}

void SubProgramsStatements::parameters_list(){

  VariableStatement::identifiers_list();

  if(Syntax::compareToken(":")){

    NEXT_TOKEN;

    string t = Syntax::get_currentToken();//pega o tipo das variaveis declaradas

    VariableStatement::type();

    MyStack::setType_ID(VariableStatement::getCount_statement_ID(), t);//seta o tipo de todos os ids declarados
    VariableStatement::reset_count_statement_ID();//zera o contador de IDs

    parameters_list_2();

  }else{
    //erro ':'
    Syntax::error("expected ':' instead '","SubProgramsStatements.cpp:parameters_list()");
  }

}

void SubProgramsStatements::parameters_list_2(){

  if(Syntax::compareToken(";")){

    NEXT_TOKEN;

    if(Syntax::compareCategory("identificador")){//se tiver mais declaracao de variavel
      VariableStatement::identifiers_list();
    }else{
      return;//e o caso do vazio
    }

    if(Syntax::compareToken(":")){

      NEXT_TOKEN;

      string t = Syntax::get_currentToken();//pega o tipo das variaveis declaradas

      VariableStatement::type();

      MyStack::setType_ID(VariableStatement::getCount_statement_ID(), t);//seta o tipo de todos os ids declarados
      VariableStatement::reset_count_statement_ID();//zera o contador de IDs

      parameters_list_2();

    }else{
      //erro ':'
      Syntax::error("expected ':' instead '","SubProgramsStatements.cpp:parameters_list_2()");
    }

  }else if(Syntax::compareCategory("identificador")){//se nao colocar ';' e tiver mais identificadores
    //erro ';'
    Syntax::error("expected ';' instead '","SubProgramsStatements.cpp:parameters_list_2()");
  }else{
    //vazio
  }

}

void SubProgramsStatements::composite_command(){

  if(Syntax::compareToken("begin")){

    COUNT_BEGIN;
    NEXT_TOKEN;
    optional_commands();

    if(Syntax::compareToken("end")){
      if(!MyStack::PcT_isEmpty()){
        MyStack::compute_expression( MyStack::post_fix_order() );
        MyStack::PcT_clear();
      }

      NEXT_TOKEN;
      COUNT_END;
      if(CHECK_END_SCOPE){
        POP_SCOPE;
      }else{
        Syntax::error("don't close scope '","SubProgramsStatements.cpp:composite_command()");
      }

      //nao faz nada
    }else{
      //erro 'end'
      Syntax::error("expected 'end' instead '","SubProgramsStatements.cpp:composite_command()");
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

  if(Syntax::compareToken(";")){

    vector<Token> a = MyStack::post_fix_order();
    string s = MyStack::compute_expression( a );
    MyStack::PcT_clear();

    NEXT_TOKEN;
    command();
    commands_list_2();

  }else{//falta ver o vazio
    //erro ';'
    //Syntax::error("expected ';' ","SubProgramsStatements.cpp:commands_list_2()");
  }
}

void SubProgramsStatements::command(){

  if(Syntax::lookahead_token(":=")){//se o prox simbolo for ':=', e o case variable
    //esta funcao segue a descricao da GLC dada, testei de forma diferente para diferenciar o nao-determinismo

    //pega token para comparar tipos :=
    variable();

    MyStack::push_in_PcT(&(*(Syntax::getTb_Token() -1)));//emplinha o id
    MyStack::push_in_PcT(&(*(Syntax::getTb_Token())));//emplinha o :=

    NEXT_TOKEN;//ja testou o ':=' acima, passa para prox
    expression();


  }else if(Syntax::compareCategory("identificador")){

    procedure_activation();

  }else if(Syntax::compareToken("begin")){

    composite_command();

  }else if(Syntax::compareToken("if")){

    NEXT_TOKEN;
    expression();

    MyStack::compute_expression( MyStack::post_fix_order() );//avalia a expressao do if
    MyStack::PcT_clear();//limpa pct

  if(Syntax::compareToken("then")){

    NEXT_TOKEN;
    command();
    else_part();

  }else{
    //erro 'then'
    Syntax::error("expected 'then' instead '","SubProgramsStatements.cpp:command()");
  }

  }else if(Syntax::compareToken("while")){

    NEXT_TOKEN;
    expression();

    MyStack::compute_expression( MyStack::post_fix_order() );//avalia a expressao do while
    MyStack::PcT_clear();//limpa pct

    if(Syntax::compareToken("do")){

      NEXT_TOKEN;
      command();

    }else{
      //erro 'do'
      Syntax::error("expected 'do' instead '","SubProgramsStatements.cpp:command()");
    }

  }else if(Syntax::compareToken("for")){

    NEXT_TOKEN;

    if(Syntax::compareCategory("identificador")){

      NEXT_TOKEN;

      if(Syntax::compareToken(":=")){

        NEXT_TOKEN;

        if(Syntax::getTb_Token()->getType().compare("integer") == 0){

          NEXT_TOKEN;

          if(Syntax::compareToken("to")){

            NEXT_TOKEN;

            if(Syntax::getTb_Token()->getType().compare("integer") == 0){

              NEXT_TOKEN;

              if(Syntax::compareToken("do")){

                NEXT_TOKEN;

                command();
                //composite_command();

              }else{
                //erro 'do' do for
                Syntax::error("expected 'do' instead '","SubProgramsStatements.cpp:command()");
              }

            }else{
              //erro tipo for
              Syntax::error("expected 'integer' instead '","SubProgramsStatements.cpp:command()");
            }

          }else{
            //erro for
            Syntax::error("expected 'to' instead '","SubProgramsStatements.cpp:command()");
          }

        }else{
          //erro de tipo
          Syntax::error("expected 'integer' instead '","SubProgramsStatements.cpp:command()");
        }

      }else{
        //erro for :=
        Syntax::error("expected ':=' instead '","SubProgramsStatements.cpp:command()");
      }

    }else{
      //erro id for
      Syntax::error("expected 'id' of 'for' instead '","SubProgramsStatements.cpp:command()");
    }


  }

}

void SubProgramsStatements::command_2(){

  if(Syntax::compareToken(":=")){

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

  if(Syntax::compareCategory("operador-relacional")){

    relational_op();

    MyStack::push_in_PcT(&(*(Syntax::getTb_Token() -1)));

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

  if(Syntax::compareToken(",")){

    NEXT_TOKEN;
    expression();
    expressions_list_2();

  }else{
    //tem que ver o erro e o vazio
    //Syntax::error("expected ','","SubProgramsStatements.cpp:expressions_list_2()");
  }
}

void SubProgramsStatements::else_part(){

  if(Syntax::compareToken("else")){

    NEXT_TOKEN;
    command();

  }else{//falta ver o vazio
    //Syntax::error("expected 'else' ","SubProgramsStatements.cpp:else_part()");
  }

}

void SubProgramsStatements::simple_expression(){

  if(Syntax::compareToken("+") || Syntax::compareToken("-")){

    Syntax::getTb_Token()->setCategory("operador-unario");
    MyStack::push_in_PcT( &(*Syntax::getTb_Token()) );

    NEXT_TOKEN;

    term();

    simple_expression_2();

  }else if(check_track(term)){

    simple_expression_2();

  }else{
    Syntax::error("expected simple_expression instead '","SubProgramsStatements.cpp:simple_expression()");
  }

}

void SubProgramsStatements::simple_expression_2(){

  if(Syntax::compareCategory("operador-aditivo")){

    additive_op();

    MyStack::push_in_PcT(&(*(Syntax::getTb_Token() -1)));

    term();
    simple_expression_2();

  }else{
    //vazio
  }

}

void SubProgramsStatements::relational_op(){

  if(Syntax::compareCategory("operador-relacional")){

    NEXT_TOKEN;

  }else{
    //erro unknow relational_op
    //Syntax::error("unknow relational_op ","SubProgramsStatements.cpp:relational_op()");
  }
}

void SubProgramsStatements::term(){
  factor();
  term_2();
}

void SubProgramsStatements::term_2(){

  if(Syntax::compareCategory("operador-multiplicativo")){

    multiplicative_op();

    MyStack::push_in_PcT(&(*(Syntax::getTb_Token() -1)));

    factor();
    term_2();

  }else{
    //vazio
  }
}

void SubProgramsStatements::factor(){

  if(Syntax::compareCategory("identificador")){

    CHECK_SCOPE;

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

    NEXT_TOKEN;
    factor_2();

  }else if(Syntax::compareCategory("inteiro")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca os numeros inteiros na pct
    NEXT_TOKEN;

  }else if(Syntax::compareCategory("real")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca os numeros reais na pct
    NEXT_TOKEN;

  }else if(Syntax::compareCategory("boolean")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca os booleans na pct
    NEXT_TOKEN;

  // }else if(Syntax::compareCategory("false")){
  //
  //   NEXT_TOKEN;

  }else if(Syntax::compareToken("(")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

    NEXT_TOKEN;
    expression();

    if(Syntax::compareToken(")")){

      MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct
      NEXT_TOKEN;

    }else{
      //erro ')'
      Syntax::error("expected ')' instead '","SubProgramsStatements.cpp:factor()");
    }

  }else if(Syntax::compareToken("not")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

    NEXT_TOKEN;
    factor();

  }

}

void SubProgramsStatements::factor_2(){

  if(Syntax::compareToken("(")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

    NEXT_TOKEN;
    expressions_list();

    if(Syntax::compareToken(")")){

      MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

      NEXT_TOKEN;

    }else{
      //erro ')'
      Syntax::error("expected ')' instead '","SubProgramsStatements.cpp:factor_2()");
    }

  }else{//tem que ver o vazio
    //Syntax::error("expected '('","SubProgramsStatements.cpp:factor_2()");
  }
}

void SubProgramsStatements::additive_op(){

  if(Syntax::compareCategory("operador-aditivo")){
    NEXT_TOKEN;
  }else{
    //Syntax::error("unknow additive_op ","SubProgramsStatements.cpp:additive_op()");
  }
}

void SubProgramsStatements::multiplicative_op(){

  if(Syntax::compareCategory("operador-multiplicativo")){
    NEXT_TOKEN;
  }else{
    //Syntax::error("unknow multiplicative_op ","SubProgramsStatements.cpp:multiplicative_op()");
  }
}

void SubProgramsStatements::procedure_activation(){

  if(Syntax::compareCategory("identificador")){

    NEXT_TOKEN;
    procedure_activation_2();
  }
}

void SubProgramsStatements::procedure_activation_2(){

  if(Syntax::compareToken("(")){

    NEXT_TOKEN;
    expressions_list();
    if(Syntax::compareToken(")")){

      NEXT_TOKEN;

    }else{
      Syntax::error("expected ')' instead '","SubProgramsStatements.cpp:procedure_activation_2()");
    }

  }else{
    //vazio
  }
}

void SubProgramsStatements::variable(){

  if(Syntax::compareCategory("identificador")){

    CHECK_SCOPE;

    NEXT_TOKEN;

  }else{
    Syntax::error("expected variable 'id' instead '","SubProgramsStatements.cpp:variable()");
  }
}

bool SubProgramsStatements::check_track(void(*f) (void)){

  vector<Token>::iterator aux = Syntax::getTb_Token();
  try{
    f();
  }catch(runtime_error &e){
    Syntax::setTb_Token(aux);
    return false;
  }
  return true;
}
