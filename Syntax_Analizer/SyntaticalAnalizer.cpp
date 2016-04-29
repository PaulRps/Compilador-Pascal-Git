
#include "SyntaticalAnalizer.h"

vector<Token>::iterator SyntaticalAnalizer::tb_token_;
vector<Token>::iterator SyntaticalAnalizer::endTb_token_;


bool SyntaticalAnalizer::compareToken(string t){
  return (tb_token_->getToken().compare(t) == 0) ? true:false;
}

bool SyntaticalAnalizer::compareCategory(string c){
  return (tb_token_->getCategory().compare(c) == 0) ? true:false;
}

vector<Token>::iterator SyntaticalAnalizer::getTb_Token(){
  return tb_token_;
}

string SyntaticalAnalizer::get_currentToken(){
  return tb_token_->getToken();
}

void SyntaticalAnalizer::setTb_Token(vector<Token>::iterator it){
  tb_token_ = it;
}

void SyntaticalAnalizer::init_tbTokens(){
  tb_token_ = Lexical::getTokens();
  endTb_token_ = Lexical::getEnd_Tokens();
}

bool SyntaticalAnalizer::lookahead_token(string t){
  if((tb_token_+1) == endTb_token_) return false;
  return ((tb_token_+1)->getToken()).compare(t) == 0 ? true:false;
}

bool SyntaticalAnalizer::lookahead_category(string c){
  if((tb_token_+1) == endTb_token_) return false;
  return ((tb_token_+1)->getCategory()).compare(c) == 0 ? true:false;
}

void SyntaticalAnalizer::program(){

  string code = File::dataInput("code.txt");//LER OS DADOS DE ENTRADA
  Lexical::start(code);//EXECUTA O Lexical SOBRE A ENTRADA LIDA E GERA A TABELA DE SIMBOLOS
  // Lexical::showTokens();//EXIBE OS TOKENS EXTRAIDOS DA
  SyntaticalAnalizer::init_tbTokens();
  Stack::init_stack();//cria a pilha

  if(compareToken("program")){

    nextToken();
    if(compareCategory("identificador")){

      getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao
      Stack::push(& (*getTb_Token()) );//guarda o id do programa
      nextToken();
      if(compareToken(";")){

        nextToken();
        VariableStatement::variables_statements();//declaracao de variaveis
        SubProgramsStatements::subPrograms_statements();//declaracao de subprogramas
        SubProgramsStatements::composite_command();//comando composto

        if(compareToken(".")){
          //fim
          //Stack::end_scope();
        }else{
          error("expected '.' in end program instead '", "SyntaticalAnalizer.cpp:program()");
        }

      }else{
        error("expected ';' after program id instead '", "SyntaticalAnalizer.cpp:program()");
      }

    }else{
      //erro identificador
      error("expected 'id' after program instead '","SyntaticalAnalizer.cpp::program()");
    }

  }else{
    //erro program
    error("expected 'program'  instead '","SyntaticalAnalizer.cpp::program()");
  }

}

void SyntaticalAnalizer::nextToken(){
  ++tb_token_;
}

void SyntaticalAnalizer::error(string m, string f){

  cout << "\nERROR::symbol don't expected in "<< f << "::" << tb_token_->getLine() << "  " << m << tb_token_->getToken() <<"' " << endl;

  throw runtime_error("error");
}
