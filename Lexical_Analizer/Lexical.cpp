#include "Lexical.h"

/*--------------------------------- CABECALHO ----------------------------------
UNIVERSIDADE FEDERAL DA PARAIBA - UFPB
DATA: 25/02/2016
ALUNO: PAULO RICARDO PEREIRA DA SILVA - 11218638
TRABALHO DA DISCIPLINA DE COMPILADORES I - ANALISADOR LEXICO DA LINGUAGEM PASCAL

ESTA CLASSE IMPLEMENTA UM AUTOMATO FINITO DETERMINISCO QUE RECONHECE A LINGUAGEM
PASCAL. TODA A LOGICA DO AUTOMATO ESTA NA FUNCAO Lexical::start(...) E OUTRAS FUNCOES
AUXILIARES QUE ESTAO DEFINIDAS ABAIXO, JUNTO COM AS VARIAVEIS ESTATICAS DE CON-
TROLE.
--------------------------------------------------------------------------------*/


/************ INICIO DAS VARIAVEIS STATICAS *************/
int Lexical::current_position_ = 0;
int Lexical::initial_position_ = 0;
int Lexical::numberLine_ = 1;
int Lexical::state_ = START;
vector<Token> Lexical::tokens_;
unordered_map<string, string> Lexical::keyWords_;
/************ FIM DAS VARIAVEIS STATICAS **************/



/*********** INICIO DAS DEFINICOES DAS FUNCOES STATICAS **********/
void Lexical::addToken(string code, string category, bool isKeyWord){

  string token = code.substr(initial_position_, current_position_ - initial_position_);

  if(isKeyWord){

    string lowerToken = token;//code.substr(initial_position_, current_position_ - initial_position_);
    transform(lowerToken.begin(), lowerToken.end(), lowerToken.begin(), ::tolower);//converte para minúsculo

    unordered_map<string, string>::iterator key = keyWords_.find(lowerToken);
    if( key != keyWords_.end() ){

      category = keyWords_[lowerToken];//seta a categoria associada ao token

    }

  }

  if(category.compare("inteiro") == 0){
    Token newToken(token, category, numberLine_, "integer");
    tokens_.push_back(newToken);
  }else if(category.compare("real") == 0){
    Token newToken(token, category, numberLine_, "real");
    tokens_.push_back(newToken);
  }else if(category.compare("boolean") == 0){
    Token newToken(token, category, numberLine_, "boolean");
    tokens_.push_back(newToken);
  }else{
    Token newToken(token, category, numberLine_);
    tokens_.push_back(newToken);
  }

  initial_position_ = current_position_;//atualiza a posição de inicio do novo token
  --current_position_;//decrementa para não pular um simbolo na prox iteracao
  restartState();//state = START

}

void Lexical::countLine(){

  numberLine_++;

}

int Lexical::getLine(){

  return numberLine_;

}

int Lexical::getState(){

  return state_;

}

vector<Token>::iterator Lexical::getTokens(){
  return tokens_.begin();
}

vector<Token>::iterator Lexical::getEnd_Tokens(){
  return tokens_.end();
}

void Lexical::ignoreToken(){

  initial_position_ = current_position_ + 1;

}

void Lexical::initMap_KeyWords(){

  string words[] = {"program", "var", "integer", "real", "boolean", "procedure", "function","begin", "end", "if", "then", "else", "while", "for", "to","do"};
  unsigned int n = sizeof(words) / sizeof(words[0]);

  keyWords_["and"] = "operador-multiplicativo";
  keyWords_["or"] = "operador-aditivo";
  keyWords_["not"] = "operador-logico";
  keyWords_["true"] = "boolean";
  keyWords_["false"] = "boolean";

  for(unsigned int i = 0; i < n; i++){

    keyWords_[words[i]] = "palavra-resevada";

  }

}

void Lexical::restartState(){

  state_ = START;

}

void Lexical::start(string code, bool isComplexNumber){

  int sizeCode = code.size();
  int count_open_close_Comment = 0;
  int count_terms_complex_number = 0;
  int first_digit_position = 0;//posicao do primeiro digito do possivel numero complexo
  int i_position = 0;//posicao do i do possivel numero complexo
  initMap_KeyWords();//inicializa as palavra-resevadas

  for(; sizeCode >= current_position_; ++current_position_){

    switch (state_) {

      case START://estado inicial

        if(code[current_position_] == '{'){//é uma abertura de comentário

          state_ = COMMENT;
          count_open_close_Comment++;

        }else if(code[current_position_] == ' ' || code[current_position_] == '\t' || code[current_position_] == '\r'){//é espaço em branco ou tabulacao ou novo paragrafo

          ignoreToken();//atualiza a posicao initial_position_ para o prox simbolo: current_position_ + 1

        }else if( isalpha(code[current_position_]) ){//é um identificador

          state_ = IDENTIFIER;

        }else if( isdigit(code[current_position_] )){//é um dígito

          state_ = DIGIT;


          /* TESTE DOS DELIMITADORES ABAIXO */
        }else if(code[current_position_] == ':'){

          state_ = DELIMITER_TWO_POINT;

        }else if(code[current_position_] == ';'){

          state_ = DELIMITER_SEMICOLON;

        }else if(code[current_position_] == '.'){

          state_ = DELIMITER_DOT;

        }else if(code[current_position_] == ','){

          state_ = DELIMITER_COMMA;

        }else if(code[current_position_] == '('){

          state_ = DELIMITER_BRACKET_OPEN;

        }else if(code[current_position_] == ')'){

          state_ = DELIMITER_BRACKET_CLOSE;


          /* TESTE DOS OPERADORES MATEMÁTICOS BÁSICOS ABAIXO */
        }else if(code[current_position_] == '+'){

          state_ = OPERATOR_ADDITIVE_SUM;

        }else if(code[current_position_] == '-'){

          state_ = OPERATOR_ADDITIVE_SUBTRACTION;

        }else if(code[current_position_] == '*'){

          state_ = OPERATOR_MULTIPLICATIVE_MULTIPLICATION;

        }else if(code[current_position_] == '/'){

          state_ = OPERATOR_MULTIPLICATIVE_DIVISION;

        }else if(code[current_position_] == '\n'){//se for uma quebra de linha

          countLine();//incrementa numberLine_
          ignoreToken();//atualiza a posicao initial_position_ para o prox simbolo: current_position_ + 1


          /* TESTE DOS OPERADORES RELACIONAIS ABAIXO */
        }else if(code[current_position_] == '='){

          state_ = RELATION_OPERATOR_EQUALITY;

        }else if(code[current_position_] == '>'){

          state_ = RELATION_OPERATOR_BIGGER;

        }else if(code[current_position_] == '<'){

          state_ = RELATION_OPERATOR_SMALLER;

        }else if(code[current_position_] == '\''){

          ignoreToken();//atualiza a posicao initial_position_ para o prox simbolo: current_position_ + 1
          state_ = OPENING_STRING;

        }else if(code[current_position_] == '\0'){

          ;

        }else if(code[current_position_] == '}'){//comentario não aberto

          state_ = COMMENT_NOT_CLOSE;

        }else{

          state_ = UNKNOW_SYMBOL;
          --current_position_;//decrementa para o simbolo entrar no case na prox iteracao

        }

      break;

      case COMMENT://comentários

        if(code[current_position_] == '}'){//se fechar comentário

          restartState();//state = START;
          ignoreToken();//atualiza a posicao initial_position_ para o prox simbolo: current_position_ + 1

        }else if(code[current_position_] == '\0'){

          state_ = COMMENT_NOT_CLOSE;
          --current_position_;//decrementa para o simbolo entrar no case na prox iteracao

        }

      break;

      case IDENTIFIER://é um identificador

        if( isalpha(code[current_position_]) ){//se é uma letra do alfabeto
          ;
        }else if( isdigit(code[current_position_]) ){//se é dígito
          ;
        }else if(code[current_position_] == '_'){//se é underscore
          ;
        }else if(code[current_position_] == '\0'){//se chegou ao fim de code

          addToken(code, "identificador", true);//verifica tambem se é palavra chave

        }else if(!isalpha(code[current_position_])){//se for qualquer outra coisa, adiciona o token

          addToken(code, "identificador", true);//verifica tambem se é palavra chave

        }

      break;

      case DIGIT://é um dígito inteiro

        if( isdigit(code[current_position_]) ){//se é dígito inteiro
          ;
        }else if(code[current_position_] == '.'){

          state_ = REAL_NUMBER;

        }else if(code[current_position_] == '+' && isComplexNumber){//se encontrar o + pode ser numero complexo

          state_ = COMPLEX_NUMBER;
          count_terms_complex_number += 2; //ja viu um numero e um +

          int i = current_position_ -1;
          while(isdigit(code[i])){

            first_digit_position = i--;

          }

        }else if( !isdigit(code[current_position_]) ){//se chegou ao fim de code

          addToken(code, "inteiro");
          restartState();

        }else if(code[current_position_] == '\n'){//se for algo diferente de um dígito

          addToken(code, "inteiro");
          countLine();
          restartState();

        }

      break;

      case REAL_NUMBER:

        if( isdigit(code[current_position_] )){//se é dígito inteiro
          ;
        }else if(code[current_position_] == '\0'){//se chegou ao fim de code

          addToken(code, "real");

        }else{

          addToken(code,"real");

        }

      break;

      case DELIMITER_TWO_POINT://é o delimitador ':'

        if(code[current_position_] == '='){//se é uma atribuição

          state_ = ATRIBUITION;

        }else{

          addToken(code,"delimitador");

        }

      break;

      case ATRIBUITION://atribuição ':='

        addToken(code,"atribuicao");

      break;

      case DELIMITER_SEMICOLON: //delimitador ';'

        addToken(code,"delimitador");

      break;

      case DELIMITER_DOT:// delimitador '.'

        if( isdigit(code[current_position_] )){//se é dígito inteiro

          state_ = REAL_NUMBER;

        }else{

          addToken(code,"delimitador");

        }

      break;

      case DELIMITER_COMMA:// delimitador ','

        addToken(code,"delimitador");

      break;

      case DELIMITER_BRACKET_OPEN:// delimitador '('

        addToken(code,"delimitador");

      break;

      case DELIMITER_BRACKET_CLOSE:// delimitador ')'

        addToken(code,"delimitador");

      break;

      case OPERATOR_ADDITIVE_SUM:// +

        addToken(code,"operador-aditivo");

      break;

      case OPERATOR_ADDITIVE_SUBTRACTION:// -

        addToken(code,"operador-aditivo");

      break;

      case OPERATOR_MULTIPLICATIVE_MULTIPLICATION:// *

        addToken(code,"operador-multiplicativo");

      break;

      case OPERATOR_MULTIPLICATIVE_DIVISION:// /

        if(code[current_position_] == '/'){//se encontrar duas barras seguidas, é comentario de linha

          state_ = LINE_COMMENT;

        }else{

          addToken(code,"operador-multiplicativo");

        }

      break;

      case RELATION_OPERATOR_EQUALITY:// ==

        addToken(code,"operador-relacional");

      break;

      case RELATION_OPERATOR_BIGGER:// >

        if(code[current_position_] == '='){

          state_ = RELATION_OPERATOR_BIGGER_OR_EQUAL;

        }else{

          addToken(code,"operador-relacional");

        }

      break;

      case RELATION_OPERATOR_SMALLER:// <

        if(code[current_position_] == '='){

          state_ = RELATION_OPERATOR_SMALLER_OR_EQUAL;

        }else if(code[current_position_] == '>'){

          state_ = RELATION_OPERATOR_DIFFERENT;

        }else{

          addToken(code,"operador-relacional");

        }

      break;

      case RELATION_OPERATOR_BIGGER_OR_EQUAL:// >=

        addToken(code,"operador-relacional");

      break;

      case RELATION_OPERATOR_SMALLER_OR_EQUAL: // <=

        addToken(code,"operador-relacional");

      break;

      case OPENING_STRING: // ''

        if(isdigit(code[current_position_]) || isalpha(code[current_position_]) ){
          ;
        }else if(code[current_position_] == '\''){

          addToken(code, "string");
          restartState();//state = START;
          ++current_position_;
          ignoreToken();//atualiza a posicao initial_position_ para o prox simbolo: current_position_ + 1

        }else{

          cout << "NAO FECHOU STRING NA LINHA: " << numberLine_ << endl;

          return;

        }

      break;

      case COMMENT_NOT_CLOSE://error

        cout << "COMENTARIO NAO FECHADO NA LINHA: " << numberLine_ << endl;

        return;

      break;

      case UNKNOW_SYMBOL://error

        cout << "SIMBOLO DESCONHECIDO " << code[current_position_] << " NA LINHA: " << numberLine_ << endl;

        return;

      break;

      case LINE_COMMENT:// //

        if(code[current_position_] == '\n'){

          restartState();
          countLine();
          ignoreToken();

        }else{
          ;
        }

      break;

      case COMPLEX_NUMBER:// [0...9]+i[0...9]

        if(code[current_position_] == 'i' && count_terms_complex_number == 2){

          ++count_terms_complex_number;//ja viu 3 de 4 termos
          i_position = current_position_;

        }else if( isdigit(code[current_position_]) && count_terms_complex_number == 3){

          ++count_terms_complex_number;//ja viu 4 de 4 termos

        }else if( code[current_position_] == '\n' && count_terms_complex_number == 4){

          addToken(code, "numero-complexo");
          countLine();
          restartState();
          count_terms_complex_number = 0;

        }else if( ( !isdigit(code[current_position_]) ) && count_terms_complex_number == 4){

          restartState();
          addToken(code, "numero-complexo");
          count_terms_complex_number = 0;

        }else if( !isdigit(code[current_position_]) && count_terms_complex_number < 4){//não e complexo

          int aux_current_position_ = current_position_;//guarda a posicao de onde parou
          current_position_ = initial_position_ = 0;//define a posicao inicial da substring
          count_terms_complex_number = 0;//reseta o contador de termos de numero-complexo
          restartState();//reseta o estado

          //pega a substring [0...9]+i
          string subCode = code.substr(first_digit_position, i_position - first_digit_position + 1);

          start(subCode, false);//roda o automato para a substring selecionada, agora setando false para o numero-complexo
          isComplexNumber = true;//reseta a variavel por que pode ter mais numeros complexos a frente
          restartState();
          initial_position_ = aux_current_position_;
          current_position_ = aux_current_position_ - 1;//seta a posicao de onde parou - 1 para que o simbolo seja processado na prox rodada

        }else if( isdigit(code[current_position_] ) ){
          ;
        }

      break;

      case RELATION_OPERATOR_DIFFERENT:
        addToken(code, "operador-relacional");
        restartState();
      break;


    }//fim do switch

  }//fim do while

}//fim da função start()

void Lexical::showTokens(){

  vector<Token>::iterator it = tokens_.begin();
  for(; it != tokens_.end(); it++){

    cout << "token: " << it->getToken() << " \t\tcategoria: " << it->getCategory() << "\t\ttype: " << it->getType() << " \t\t\tlinha: " << it->getLine() << endl;

  }

}
/*********** FIM DAS DEFINICOES DAS FUNCOES STATICAS **********/
