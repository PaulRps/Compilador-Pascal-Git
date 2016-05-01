#include "VariableStatement.h"

int VariableStatement::count_statement_id_ = 0;

void VariableStatement::variables_statements(){

  if(Syntax::compareToken("var")){

    NEXT_TOKEN;
    variables_statements_list();

  }else if(Syntax::lookahead_category("identificador")){//nao tem var e o prox e um ID
    //erro var
    Syntax::error("expected 'var' instead '","12:VariableStatement.cpp::variables_statements()");
  }

}

void VariableStatement::variables_statements_list(){

  identifiers_list();

  if(Syntax::compareToken(":")){

    NEXT_TOKEN;
    string t = Syntax::get_currentToken();//pega o tipo das variaveis declaradas
    type();
    MyStack::setType_ID(AMOUNT_IDs, t);//seta o tipo de todos os ids declarados

    reset_count_statement_ID();//zera o contador de IDs

    if(Syntax::compareToken(";")){

      NEXT_TOKEN;
      if(Syntax::compareCategory("identificador")){//se tiver mais declaracoes de variaveis
        variables_statements_list_2();
      }else if(Syntax::compareToken("procedure") || Syntax::compareToken("begin") || Syntax::compareToken("function") ){//se for subprograma ou comando composto
        //nao faz nada, segue em frente
      }else{
        Syntax::error("error","VariableStatement.cpp::variables_statements_list()");
      }

    }else{
      //erro variables_statements_list ';'
      Syntax::error("expected ';' instead '","VariableStatement.cpp::variables_statements_list()");
    }

  }else{
    //erro variables_statements_list ':'
    Syntax::error("expected ':' instead '","VariableStatement.cpp::variables_statements_list()");
  }

}

void VariableStatement::variables_statements_list_2(){

  identifiers_list();

  if(Syntax::compareToken(":")){

    NEXT_TOKEN;
    string t = Syntax::get_currentToken();//pega o tipo das variaveis declaradas
    type();
    MyStack::setType_ID(AMOUNT_IDs, t);//seta o tipo dos ids

    reset_count_statement_ID();//zera o contador de IDs

    if(Syntax::compareToken(";")){

      NEXT_TOKEN;
      if(Syntax::compareCategory("identificador")){//se tiver mais declaracoes de variaveis
        variables_statements_list_2();
      }else if(Syntax::compareToken("procedure") || Syntax::compareToken("begin")){//se for subprograma ou comando composto
        //nao faz nada, segue em frente
      }else{
        Syntax::error("error","VariableStatement.cpp::variables_statements_list_2()");
      }

    }else{
      //erro variables_statements_list_2 ';'
      Syntax::error("expected ';' instead '","VariableStatement.cpp::variables_statements_list_2()");
    }

  }else{
    //erro variables_statements_list_2 ':'
    Syntax::error("expected ':' instead '","64:VariableStatement.cpp::variables_statements_list_2()");
  }
}

void VariableStatement::identifiers_list(){

  if(Syntax::compareCategory("identificador")){

    count_statement_ID();//conta a qtd de variaveis declaradas

    if(PUSH_ID){//guarda o id na pilha de escopo
      //id e unico
    }else{
      Syntax::error("dubplicate id '","VariableStatement.cpp::identifiers_list()");
    }

    NEXT_TOKEN;
    identifiers_list_2();

  }else{//TEM QUE SER VAZIO AQUI PARA SUBPROGRAMAS

  }

}

void VariableStatement::identifiers_list_2(){

  if(Syntax::compareToken(",")){

    NEXT_TOKEN;
    if(Syntax::compareCategory("identificador")){

      count_statement_ID();//conta a qtd de variaveis declaradas

      if(PUSH_ID){//guarda o id
        //id e unico
      }else{
        Syntax::error("dubplicate id '","VariableStatement.cpp::identifiers_list_2()");
      }

      NEXT_TOKEN;
      identifiers_list_2();

    }else{
      //vazio
    }

  }else{
    //vazio
  }

}

void VariableStatement::type(){

  if(Syntax::compareToken("integer") || Syntax::compareToken("real") || Syntax::compareToken("boolean")){
    NEXT_TOKEN;
  }else{
    // erro de tipo
    Syntax::error("unknow type","VariableStatement.cpp::type()");
  }
}

void VariableStatement::check_scope(){

  if(ID_IS_IN_CURRENT_SCOPE){

  }else{

    if(ID_IS_IN_GLOBAL_SCOPE){

    }else{
      Syntax::error("variable 'id' don't declared '"," in ");
    }
  }
}

void VariableStatement::count_statement_ID(){
  ++count_statement_id_;
}

int VariableStatement::getCount_statement_ID(){
  return count_statement_id_;
}

void VariableStatement::reset_count_statement_ID(){
  count_statement_id_ = 0;
}
