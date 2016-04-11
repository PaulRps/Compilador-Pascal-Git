#include "Token.h"

/*--------------------------------- CABECALHO ----------------------------------
UNIVERSIDADE FEDERAL DA PARAIBA - UFPB
DATA: 25/02/2016
ALUNO: PAULO RICARDO PEREIRA DA SILVA - 11218638
TRABALHO DA DISCIPLINA DE COMPILADORES I - ANALISADOR LEXICO DA LINGUAGEM PASCAL

ESTA CLASSE IMPLEMENTA A ESTRUTURA QUE ARMAZENA AS INFORMACOES SOBRE OS SIMBOLOS
(SIMBOLOS) COMO CATEGORIA, NUMERO DA LINHA QUE APARECEU E O PROPRIO SIMBOLO
--------------------------------------------------------------------------------*/

Token::Token(string t, string c, int l, string tp) {
  token = t;
  category = c;
  line = l;
  type = tp;
}

string Token::getToken() {
  return token;
}

string Token::getCategory() {
  return category;
}

string Token::getType(){
  return type;
}

void Token::setCategory(string t){
  category = t;
}

void Token::setType(string t){
  type = t;
}

int Token::getLine() {
  return line;
}
