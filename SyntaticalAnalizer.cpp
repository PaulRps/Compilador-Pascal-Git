
#include "SyntaticalAnalizer.h"

vector<Token>::iterator SyntaticalAnalizer::tb_token;
vector<Token>::iterator SyntaticalAnalizer::endTb_token;


bool SyntaticalAnalizer::compareToken(string t){
  return (tb_token->getToken().compare(t) == 0) ? true:false;
}

bool SyntaticalAnalizer::compareCategory(string c){
  return (tb_token->getCategory().compare(c) == 0) ? true:false;
}

vector<Token>::iterator SyntaticalAnalizer::getTb_Token(){
  return tb_token;
}

string SyntaticalAnalizer::get_currentToken(){
  return tb_token->getToken();
}

void SyntaticalAnalizer::setTb_Token(vector<Token>::iterator it){
  tb_token = it;
}

void SyntaticalAnalizer::init_tbTokens(){
  tb_token = AFD::getTokens();
  endTb_token = AFD::getEnd_Tokens();
}

bool SyntaticalAnalizer::lookahead_token(string t){
  if((tb_token+1) == endTb_token) return false;
  return ((tb_token+1)->getToken()).compare(t) == 0 ? true:false;
}

bool SyntaticalAnalizer::lookahead_category(string c){
  if((tb_token+1) == endTb_token) return false;
  return ((tb_token+1)->getCategory()).compare(c) == 0 ? true:false;
}

void SyntaticalAnalizer::program(){

  string code = File::dataInput("code.txt");//LER OS DADOS DE ENTRADA
  AFD::start(code);//EXECUTA O AFD SOBRE A ENTRADA LIDA E GERA A TABELA DE SIMBOLOS
  // AFD::showTokens();//EXIBE OS TOKENS EXTRAIDOS DA
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
  ++tb_token;
}

void SyntaticalAnalizer::error(string m, string f){

  cout << "\nERROR::symbol don't expected in "<< f << "::" << tb_token->getLine() << "  " << m << tb_token->getToken() <<"' " << endl;

  throw runtime_error("error");
}
