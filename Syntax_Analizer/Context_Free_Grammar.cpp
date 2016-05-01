#include "Context_Free_Grammar.h"

int Context_Free_Grammar::count_statement_id_ = 0;

void Context_Free_Grammar::variables_statements(){

  if(Syntax::compareToken("var")){

    Syntax::nextToken();
    variables_statements_list();

  }else if(Syntax::lookahead_category("identificador")){//nao tem var e o prox e um ID
    //erro var
    Error::error_message("expected 'var' instead '","12:Context_Free_Grammar.cpp::variables_statements()");
  }

}

void Context_Free_Grammar::variables_statements_list(){

  identifiers_list();

  if(Syntax::compareToken(":")){

    Syntax::nextToken();

    string t = Syntax::get_currentToken();//pega o tipo das variaveis declaradas

    type();

    MyStack::setType_ID(getCount_statement_ID(), t);//seta o tipo de todos os ids declarados

    reset_count_statement_ID();//zera o contador de IDs

    if(Syntax::compareToken(";")){

      Syntax::nextToken();

      if(Syntax::compareCategory("identificador")){//se tiver mais declaracoes de variaveis

        variables_statements_list_2();

      }else if(Syntax::compareToken("procedure") || Syntax::compareToken("begin") || Syntax::compareToken("function") ){//se for subprograma ou comando composto
        //nao faz nada, segue em frente
      }else{
        Error::error_message("error","Context_Free_Grammar.cpp::variables_statements_list()");
      }

    }else{
      //erro variables_statements_list ';'
      Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp::variables_statements_list()");
    }

  }else{
    //erro variables_statements_list ':'
    Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp::variables_statements_list()");
  }

}

void Context_Free_Grammar::variables_statements_list_2(){

  identifiers_list();

  if(Syntax::compareToken(":")){

    Syntax::nextToken();

    string t = Syntax::get_currentToken();//pega o tipo das variaveis declaradas

    type();

    MyStack::setType_ID(getCount_statement_ID(), t);//seta o tipo dos ids

    reset_count_statement_ID();//zera o contador de IDs

    if(Syntax::compareToken(";")){

      Syntax::nextToken();

      if(Syntax::compareCategory("identificador")){//se tiver mais declaracoes de variaveis

        variables_statements_list_2();

      }else if(Syntax::compareToken("procedure") || Syntax::compareToken("begin")){//se for subprograma ou comando composto
        //nao faz nada, segue em frente
      }else{
        Error::error_message("error","Context_Free_Grammar.cpp::variables_statements_list_2()");
      }

    }else{
      //erro variables_statements_list_2 ';'
      Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp::variables_statements_list_2()");
    }

  }else{
    //erro variables_statements_list_2 ':'
    Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp::variables_statements_list_2()");
  }
}

void Context_Free_Grammar::identifiers_list(){

  if(Syntax::compareCategory("identificador")){

    count_statement_ID();//conta a qtd de variaveis declaradas

    if(MyStack::push(&(*Syntax::getTb_Token()))){//guarda o id na pilha de escopo
      //id e unico
    }else{
      Error::error_message("dubplicate id '","Context_Free_Grammar.cpp::identifiers_list()");
    }

    Syntax::nextToken();
    identifiers_list_2();

  }else{//TEM QUE SER VAZIO AQUI PARA SUBPROGRAMAS

  }

}

void Context_Free_Grammar::identifiers_list_2(){

  if(Syntax::compareToken(",")){

    Syntax::nextToken();

    if(Syntax::compareCategory("identificador")){

      count_statement_ID();//conta a qtd de variaveis declaradas

      if(MyStack::push(&(*Syntax::getTb_Token()))){//guarda o id
        //id e unico
      }else{
        Error::error_message("dubplicate id '","Context_Free_Grammar.cpp::identifiers_list_2()");
      }

      Syntax::nextToken();
      identifiers_list_2();

    }else{
      //vazio
    }

  }else{
    //vazio
  }

}

void Context_Free_Grammar::type(){

  if(Syntax::compareToken("integer") || Syntax::compareToken("real") || Syntax::compareToken("boolean")){
    Syntax::nextToken();
  }else{
    // erro de tipo
    Error::error_message("unknow type","Context_Free_Grammar.cpp::type()");
  }
}

void Context_Free_Grammar::subPrograms_statements(){
  subPrograms_statements_2();
}

void Context_Free_Grammar::subPrograms_statements_2(){

  if(Syntax::compareToken("procedure") || Syntax::compareToken("function")){//se tiver mais subprogramas

    subProgram_Statement();

    if(Syntax::compareToken(";") ){

      Syntax::nextToken();
      subPrograms_statements_2();

    }else{
      //erro ';'
      Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp:subProgram_Statement_2()");
    }

  }else{
    //vazio
  }

}

void Context_Free_Grammar::subProgram_Statement(){

  if(Syntax::compareToken("procedure")){

    Syntax::nextToken();

    if(Syntax::compareCategory("identificador")){

      Syntax::getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao

      if(MyStack::push(&(*Syntax::getTb_Token()))){//guarda o id do programa na pilha de ids de procedures e verifica se ela ja esta no escopo atual
        //id e unico
      }else{
        Error::error_message("dubplicate id '","Context_Free_Grammar.cpp::subProgram_Statement()");
      }

      MyStack::end_scope();//encerra o escopo anterior e comeca um novo

      Syntax::nextToken();
      arguments();

      if(Syntax::compareToken(";")){

        Syntax::nextToken();
        if(Syntax::compareToken("var"))//se tiver declaracao de variavel

          variables_statements();

        if(Syntax::compareToken("procedure"))//se tiver declaracao de procedure

          subPrograms_statements();

        composite_command();

      }else{
        //erro ';'
        Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
      }

    }else{
      //erro 'id'
      Error::error_message("expected procedure 'id' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
    }

  }else if(Syntax::compareToken("function")){

    Syntax::nextToken();

    if(Syntax::compareCategory("identificador")){

      Syntax::getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao

      if(MyStack::push(&(*Syntax::getTb_Token()))){//guarda o id do programa na pilha de ids de procedures e verifica se ela ja esta no escopo atual
        //id e unico
      }else{
        Error::error_message("dubplicate id '","Context_Free_Grammar.cpp::subProgram_Statement()");
      }

      MyStack::end_scope();//encerra o escopo anterior e comeca um novo

      Syntax::nextToken();
      arguments();

      if(Syntax::compareToken(":")){

        Syntax::nextToken();

        type();

        if(Syntax::compareToken(";")){

          Syntax::nextToken();
          if(Syntax::compareToken("var"))//se tiver declaracao de variavel

            variables_statements();

          if(Syntax::compareToken("procedure") || Syntax::compareToken("function"))//se tiver declaracao de procedure

            subPrograms_statements();

          composite_command();

        }else{
          //erro ';'
          Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
        }


      }else{
        //erro ':'
        Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
      }

    }else {
      //erro 'id'
      Error::error_message("expected function 'id' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
    }

  }else{
    //erro 'procedure'
    Error::error_message("expected 'procedure' or 'function' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
  }

}

void Context_Free_Grammar::arguments(){

  if( Syntax::compareToken( "(" ) ){

    Syntax::nextToken();
    parameters_list();

    if( Syntax::compareToken( ")" ) ){
      Syntax::nextToken();
    }else{
      //erro ')'
      Error::error_message("expected ')' instead '","Context_Free_Grammar.cpp:arguments()");
    }

  }else if(Syntax::lookahead_category("identificador")){//se o prox for um id, provavel que seja um erro
    //erro '('
    Error::error_message("expected '(' instead '","Context_Free_Grammar.cpp:arguments()");
  }
}

void Context_Free_Grammar::parameters_list(){

  identifiers_list();

  if(Syntax::compareToken(":")){

    Syntax::nextToken();

    string t = Syntax::get_currentToken();//pega o tipo das variaveis declaradas

    type();

    MyStack::setType_ID(getCount_statement_ID(), t);//seta o tipo de todos os ids declarados

    reset_count_statement_ID();//zera o contador de IDs

    parameters_list_2();

  }else{
    //erro ':'
    Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp:parameters_list()");
  }

}

void Context_Free_Grammar::parameters_list_2(){

  if(Syntax::compareToken(";")){

    Syntax::nextToken();

    if(Syntax::compareCategory("identificador")){//se tiver mais declaracao de variavel

      identifiers_list();

    }else{
      return;//e o caso do vazio
    }

    if(Syntax::compareToken(":")){

      Syntax::nextToken();

      string t = Syntax::get_currentToken();//pega o tipo das variaveis declaradas

      type();

      MyStack::setType_ID(getCount_statement_ID(), t);//seta o tipo de todos os ids declarados

      reset_count_statement_ID();//zera o contador de IDs

      parameters_list_2();

    }else{
      //erro ':'
      Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp:parameters_list_2()");
    }

  }else if(Syntax::compareCategory("identificador")){//se nao colocar ';' e tiver mais identificadores
    //erro ';'
    Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp:parameters_list_2()");
  }else{
    //vazio
  }

}

void Context_Free_Grammar::composite_command(){

  if(Syntax::compareToken("begin")){

    MyStack::count_begin();
    Syntax::nextToken();
    optional_commands();

    if(Syntax::compareToken("end")){

      if(!MyStack::PcT_isEmpty()){

        MyStack::compute_expression( MyStack::post_fix_order() );
        MyStack::PcT_clear();

      }

      Syntax::nextToken();
      MyStack::count_end();
      if(MyStack::check_end_scope()){
        MyStack::pop_scope();
      }else{
        Error::error_message("don't close scope '","Context_Free_Grammar.cpp:composite_command()");
      }

      //nao faz nada
    }else{
      //erro 'end'
      Error::error_message("expected 'end' instead '","Context_Free_Grammar.cpp:composite_command()");
    }

  }
}

void Context_Free_Grammar::optional_commands(){

  commands_list();

}

void Context_Free_Grammar::commands_list(){

  command();
  commands_list_2();

}

void Context_Free_Grammar::commands_list_2(){

  if(Syntax::compareToken(";")){

    vector<Token> a = MyStack::post_fix_order();
    string s = MyStack::compute_expression( a );
    MyStack::PcT_clear();

    Syntax::nextToken();
    command();
    commands_list_2();

  }else{
    //vazio
  }
}

void Context_Free_Grammar::command(){

  if(Syntax::lookahead_token(":=")){//se o prox simbolo for ':=', e o case variable

    variable();

    MyStack::push_in_PcT(&(*(Syntax::getTb_Token() -1)));//emplinha o id
    MyStack::push_in_PcT(&(*(Syntax::getTb_Token())));//emplinha o :=

    Syntax::nextToken();//ja testou o ':=' acima, passa para prox
    expression();

  }else if(Syntax::compareCategory("identificador")){

    procedure_activation();

  }else if(Syntax::compareToken("begin")){

    composite_command();

  }else if(Syntax::compareToken("if")){

    Syntax::nextToken();
    expression();

    MyStack::compute_expression( MyStack::post_fix_order() );//avalia a expressao do if
    MyStack::PcT_clear();//limpa pct

  if(Syntax::compareToken("then")){

    Syntax::nextToken();
    command();
    else_part();

  }else{
    //erro 'then'
    Error::error_message("expected 'then' instead '","Context_Free_Grammar.cpp:command()");
  }

  }else if(Syntax::compareToken("while")){

    Syntax::nextToken();
    expression();

    MyStack::compute_expression( MyStack::post_fix_order() );//avalia a expressao do while
    MyStack::PcT_clear();//limpa pct

    if(Syntax::compareToken("do")){

      Syntax::nextToken();
      command();

    }else{
      //erro 'do'
      Error::error_message("expected 'do' instead '","Context_Free_Grammar.cpp:command()");
    }

  }else if(Syntax::compareToken("for")){

    Syntax::nextToken();

    if(Syntax::compareCategory("identificador")){

      Syntax::nextToken();

      if(Syntax::compareToken(":=")){

        Syntax::nextToken();

        if(Syntax::getTb_Token()->getType().compare("integer") == 0){

          Syntax::nextToken();

          if(Syntax::compareToken("to")){

            Syntax::nextToken();

            if(Syntax::getTb_Token()->getType().compare("integer") == 0){

              Syntax::nextToken();

              if(Syntax::compareToken("do")){

                Syntax::nextToken();

                command();

              }else{
                //erro 'do' do for
                Error::error_message("expected 'do' instead '","Context_Free_Grammar.cpp:command()");
              }

            }else{
              //erro tipo for
              Error::error_message("expected 'integer' instead '","Context_Free_Grammar.cpp:command()");
            }

          }else{
            //erro for
            Error::error_message("expected 'to' instead '","Context_Free_Grammar.cpp:command()");
          }

        }else{
          //erro de tipo
          Error::error_message("expected 'integer' instead '","Context_Free_Grammar.cpp:command()");
        }

      }else{
        //erro for :=
        Error::error_message("expected ':=' instead '","Context_Free_Grammar.cpp:command()");
      }

    }else{
      //erro id for
      Error::error_message("expected 'id' of 'for' instead '","Context_Free_Grammar.cpp:command()");
    }


  }

}

void Context_Free_Grammar::command_2(){

  if(Syntax::compareToken(":=")){

    Syntax::nextToken();
    expression();
    //checar tipos
  }else{
    procedure_activation_2();
  }
}

void Context_Free_Grammar::expression(){

  simple_expression();
  expression_2();

}

void Context_Free_Grammar::expression_2(){

  if(Syntax::compareCategory("operador-relacional")){

    relational_op();

    MyStack::push_in_PcT(&(*(Syntax::getTb_Token() -1)));

    simple_expression();

  }else{
    // vazio
  }

}

void Context_Free_Grammar::expressions_list(){

  expression();
  expressions_list_2();
}

void Context_Free_Grammar::expressions_list_2(){

  if(Syntax::compareToken(",")){

    Syntax::nextToken();
    expression();
    expressions_list_2();

  }else{
    //vazio
  }
}

void Context_Free_Grammar::else_part(){

  if(Syntax::compareToken("else")){

    Syntax::nextToken();
    command();

  }else{
    //vazio
  }

}

void Context_Free_Grammar::simple_expression(){

  if(Syntax::compareToken("+") || Syntax::compareToken("-")){

    Syntax::getTb_Token()->setCategory("operador-unario");
    MyStack::push_in_PcT( &(*Syntax::getTb_Token()) );

    Syntax::nextToken();

    term();

    simple_expression_2();

  }else if(check_track(term)){

    simple_expression_2();

  }else{
    Error::error_message("expected simple_expression instead '","Context_Free_Grammar.cpp:simple_expression()");
  }

}

void Context_Free_Grammar::simple_expression_2(){

  if(Syntax::compareCategory("operador-aditivo")){

    additive_op();

    MyStack::push_in_PcT(&(*(Syntax::getTb_Token() -1)));

    term();
    simple_expression_2();

  }else{
    //vazio
  }

}

void Context_Free_Grammar::relational_op(){

  if(Syntax::compareCategory("operador-relacional")){

    Syntax::nextToken();

  }else{
    //erro unknow relational_op
    //Syntax::error("unknow relational_op ","Context_Free_Grammar.cpp:relational_op()");
  }

}

void Context_Free_Grammar::term(){
  factor();
  term_2();
}

void Context_Free_Grammar::term_2(){

  if(Syntax::compareCategory("operador-multiplicativo")){

    multiplicative_op();

    MyStack::push_in_PcT(&(*(Syntax::getTb_Token() -1)));

    factor();
    term_2();

  }else{
    //vazio
  }
}

void Context_Free_Grammar::factor(){

  if(Syntax::compareCategory("identificador")){

    Syntax::check_scope();

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

    Syntax::nextToken();
    factor_2();

  }else if(Syntax::compareCategory("inteiro")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca os numeros inteiros na pct
    Syntax::nextToken();

  }else if(Syntax::compareCategory("real")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca os numeros reais na pct
    Syntax::nextToken();

  }else if(Syntax::compareCategory("boolean")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca os booleans na pct
    Syntax::nextToken();

  }else if(Syntax::compareToken("(")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

    Syntax::nextToken();
    expression();

    if(Syntax::compareToken(")")){

      MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct
      Syntax::nextToken();

    }else{
      //erro ')'
      Error::error_message("expected ')' instead '","Context_Free_Grammar.cpp:factor()");
    }

  }else if(Syntax::compareToken("not")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

    Syntax::nextToken();
    factor();

  }

}

void Context_Free_Grammar::factor_2(){

  if(Syntax::compareToken("(")){

    MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

    Syntax::nextToken();
    expressions_list();

    if(Syntax::compareToken(")")){

      MyStack::push_in_PcT(&(*Syntax::getTb_Token()));//coloca na pct

      Syntax::nextToken();

    }else{
      //erro ')'
      Error::error_message("expected ')' instead '","Context_Free_Grammar.cpp:factor_2()");
    }

  }else{
    //Vazio
  }
}

void Context_Free_Grammar::additive_op(){

  if(Syntax::compareCategory("operador-aditivo")){
    Syntax::nextToken();
  }else{
    //Syntax::error("unknow additive_op ","Context_Free_Grammar.cpp:additive_op()");
  }
}

void Context_Free_Grammar::multiplicative_op(){

  if(Syntax::compareCategory("operador-multiplicativo")){
    Syntax::nextToken();
  }else{
    //Syntax::error("unknow multiplicative_op ","Context_Free_Grammar.cpp:multiplicative_op()");
  }
}

void Context_Free_Grammar::procedure_activation(){

  if(Syntax::compareCategory("identificador")){

    Syntax::nextToken();
    procedure_activation_2();
  }
}

void Context_Free_Grammar::procedure_activation_2(){

  if(Syntax::compareToken("(")){

    Syntax::nextToken();
    expressions_list();

    if(Syntax::compareToken(")")){

      Syntax::nextToken();

    }else{
      Error::error_message("expected ')' instead '","Context_Free_Grammar.cpp:procedure_activation_2()");
    }

  }else{
    //vazio
  }
}

void Context_Free_Grammar::variable(){

  if(Syntax::compareCategory("identificador")){

    Syntax::check_scope();

    Syntax::nextToken();

  }else{
    Error::error_message("expected variable 'id' instead '","Context_Free_Grammar.cpp:variable()");
  }
}


//*********** Aux Functions *******************
void Context_Free_Grammar::count_statement_ID(){
  ++count_statement_id_;
}

int Context_Free_Grammar::getCount_statement_ID(){
  return count_statement_id_;
}

void Context_Free_Grammar::reset_count_statement_ID(){
  count_statement_id_ = 0;
}

bool Context_Free_Grammar::check_track(void(*f) (void)){

  vector<Token>::iterator aux = Syntax::getTb_Token();
  try{
    f();
  }catch(runtime_error &e){
    Syntax::setTb_Token(aux);
    return false;
  }
  return true;
}
