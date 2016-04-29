#include "VariableStatement.h"

int VariableStatement::count_statement_id_ = 0;

void VariableStatement::variables_statements(){

  if(SyntaticalAnalizer::compareToken("var")){

    NEXT_TOKEN;
    variables_statements_list();

  }else if(SyntaticalAnalizer::lookahead_category("identificador")){//nao tem var e o prox e um ID
    //erro var
    SyntaticalAnalizer::error("expected 'var' instead '","12:VariableStatement.cpp::variables_statements()");
  }

}

void VariableStatement::variables_statements_list(){

  identifiers_list();

  if(SyntaticalAnalizer::compareToken(":")){

    NEXT_TOKEN;
    string t = SyntaticalAnalizer::get_currentToken();//pega o tipo das variaveis declaradas
    type();
    Stack::setType_ID(AMOUNT_IDs, t);//seta o tipo de todos os ids declarados

    reset_count_statement_ID();//zera o contador de IDs

    if(SyntaticalAnalizer::compareToken(";")){

      NEXT_TOKEN;
      if(SyntaticalAnalizer::compareCategory("identificador")){//se tiver mais declaracoes de variaveis
        variables_statements_list_2();
      }else if(SyntaticalAnalizer::compareToken("procedure") || SyntaticalAnalizer::compareToken("begin") || SyntaticalAnalizer::compareToken("function") ){//se for subprograma ou comando composto
        //nao faz nada, segue em frente
      }else{
        SyntaticalAnalizer::error("error","VariableStatement.cpp::variables_statements_list()");
      }

    }else{
      //erro variables_statements_list ';'
      SyntaticalAnalizer::error("expected ';' instead '","VariableStatement.cpp::variables_statements_list()");
    }

  }else{
    //erro variables_statements_list ':'
    SyntaticalAnalizer::error("expected ':' instead '","VariableStatement.cpp::variables_statements_list()");
  }

}

void VariableStatement::variables_statements_list_2(){

  identifiers_list();

  if(SyntaticalAnalizer::compareToken(":")){

    NEXT_TOKEN;
    string t = SyntaticalAnalizer::get_currentToken();//pega o tipo das variaveis declaradas
    type();
    Stack::setType_ID(AMOUNT_IDs, t);//seta o tipo dos ids

    reset_count_statement_ID();//zera o contador de IDs

    if(SyntaticalAnalizer::compareToken(";")){

      NEXT_TOKEN;
      if(SyntaticalAnalizer::compareCategory("identificador")){//se tiver mais declaracoes de variaveis
        variables_statements_list_2();
      }else if(SyntaticalAnalizer::compareToken("procedure") || SyntaticalAnalizer::compareToken("begin")){//se for subprograma ou comando composto
        //nao faz nada, segue em frente
      }else{
        SyntaticalAnalizer::error("error","VariableStatement.cpp::variables_statements_list_2()");
      }

    }else{
      //erro variables_statements_list_2 ';'
      SyntaticalAnalizer::error("expected ';' instead '","VariableStatement.cpp::variables_statements_list_2()");
    }

  }else{
    //erro variables_statements_list_2 ':'
    SyntaticalAnalizer::error("expected ':' instead '","64:VariableStatement.cpp::variables_statements_list_2()");
  }
}

void VariableStatement::identifiers_list(){

  if(SyntaticalAnalizer::compareCategory("identificador")){

    count_statement_ID();//conta a qtd de variaveis declaradas

    if(PUSH_ID){//guarda o id na pilha de escopo
      //id e unico
    }else{
      SyntaticalAnalizer::error("dubplicate id '","VariableStatement.cpp::identifiers_list()");
    }

    NEXT_TOKEN;
    identifiers_list_2();

  }else{//TEM QUE SER VAZIO AQUI PARA SUBPROGRAMAS

  }

}

void VariableStatement::identifiers_list_2(){

  if(SyntaticalAnalizer::compareToken(",")){

    NEXT_TOKEN;
    if(SyntaticalAnalizer::compareCategory("identificador")){

      count_statement_ID();//conta a qtd de variaveis declaradas

      if(PUSH_ID){//guarda o id
        //id e unico
      }else{
        SyntaticalAnalizer::error("dubplicate id '","VariableStatement.cpp::identifiers_list_2()");
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

  if(SyntaticalAnalizer::compareToken("integer") || SyntaticalAnalizer::compareToken("real") || SyntaticalAnalizer::compareToken("boolean")){
    NEXT_TOKEN;
  }else{
    // erro de tipo
    SyntaticalAnalizer::error("unknow type","VariableStatement.cpp::type()");
  }
}

void VariableStatement::check_scope(){

  if(ID_IS_IN_CURRENT_SCOPE){

  }else{

    if(ID_IS_IN_GLOBAL_SCOPE){

    }else{
      SyntaticalAnalizer::error("variable 'id' don't declared '"," in ");
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
