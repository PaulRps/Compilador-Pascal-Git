#include "Token.h"

/*--------------------------------- CABECALHO ----------------------------------
UNIVERSIDADE FEDERAL DA PARAIBA - UFPB
DATA: 25/02/2016
ALUNO: PAULO RICARDO PEREIRA DA SILVA - 11218638
TRABALHO DA DISCIPLINA DE COMPILADORES I - ANALISADOR LEXICO DA LINGUAGEM PASCAL

ESTA CLASSE IMPLEMENTA A ESTRUTURA QUE ARMAZENA AS INFORMACOES SOBRE OS SIMBOLOS
(SIMBOLOS) COMO CATEGORIA, NUMERO DA LINHA QUE APARECEU E O PROPRIO SIMBOLO
--------------------------------------------------------------------------------*/

Token::Token(const string t, const string c, const int l, const string tp): token_(t), category_(c), line_(l), type_(tp){}

string Token::getToken() const{
    return token_;
}

string Token::getCategory() const{
    return category_;
}

string Token::getType() const{
    return type_;
}

void Token::setCategory(const string t){
    category_ = t;
}

void Token::setType(const string t){
    type_ = t;
}

int Token::getLine() const{
    return line_;
}
