
#include "Syntax.h"

vector<Token>::iterator Syntax::tb_token_;
vector<Token>::iterator Syntax::endTb_token_;


bool Syntax::compareToken(string t){
  return (tb_token_->getToken().compare(t) == 0) ? true:false;
}

bool Syntax::compareCategory(string c){
  return (tb_token_->getCategory().compare(c) == 0) ? true:false;
}

vector<Token>::iterator Syntax::getTb_Token(){
  return tb_token_;
}

string Syntax::get_currentToken(){
  return tb_token_->getToken();
}

void Syntax::setTb_Token(vector<Token>::iterator it){
  tb_token_ = it;
}

void Syntax::init_tbTokens(){
  tb_token_ = Lexical::getTokens();
  endTb_token_ = Lexical::getEnd_Tokens();
}

bool Syntax::lookahead_token(string t){
  if((tb_token_+1) == endTb_token_) return false;
  return ((tb_token_+1)->getToken()).compare(t) == 0 ? true:false;
}

bool Syntax::lookahead_category(string c){
  if((tb_token_+1) == endTb_token_) return false;
  return ((tb_token_+1)->getCategory()).compare(c) == 0 ? true:false;
}

void Syntax::program(){

  string code = File::dataInput("C:\\Users\\paulo\\Documents\\GitHub\\Compilador-Pascal-Git\\Data_input\\code.txt");//LER OS DADOS DE ENTRADA
  Lexical::start(code);//EXECUTA O Lexical SOBRE A ENTRADA LIDA E GERA A TABELA DE SIMBOLOS
  // Lexical::showTokens();//EXIBE OS TOKENS EXTRAIDOS DA
  Syntax::init_tbTokens();
  MyStack::init_MyStack();//cria a pilha

  if(compareToken("program")){

    nextToken();

    if(compareCategory("identificador")){

      getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao
      MyStack::push(& (*getTb_Token()) );//guarda o id do programa
      nextToken();

      if(compareToken(";")){

        nextToken();
        Context_Free_Grammar::variables_statements();//declaracao de variaveis
        Context_Free_Grammar::subPrograms_statements();//declaracao de subprogramas
        Context_Free_Grammar::composite_command();//comando composto

        if(compareToken(".")){
          //fim
        }else{
          Error::error_message("expected '.' in end program instead '", "Syntax.cpp:program()");
        }

      }else{
        Error::error_message("expected ';' after program id instead '", "Syntax.cpp:program()");
      }

    }else{
      //erro identificador
      Error::error_message("expected 'id' after program instead '","Syntax.cpp::program()");
    }

  }else{
    //erro program
    Error::error_message("expected 'program'  instead '","Syntax.cpp::program()");
  }

}

void Syntax::nextToken(){
  ++tb_token_;
}

void Syntax::check_scope(){

  if( MyStack::search_current_scope(&(*getTb_Token())) ){

  }else{

    if( MyStack::search_all_scope(&(*getTb_Token())) ){

    }else{
      Error::error_message("variable 'id' don't declared '"," in ");
    }
  }
}

// void Syntax::error(string m, string f){
//
//   cout << "\nERROR::symbol don't expected in "<< f << "::" << tb_token_->getLine() << "  " << m << tb_token_->getToken() <<"' " << endl;
//
//   throw runtime_error("error");
// }
