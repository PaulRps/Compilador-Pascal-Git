#include "AFD.h"

/*--------------------------------- CABECALHO ----------------------------------
UNIVERSIDADE FEDERAL DA PARAIBA - UFPB
DATA: 25/02/2016
ALUNO: PAULO RICARDO PEREIRA DA SILVA - 11218638
TRABALHO DA DISCIPLINA DE COMPILADORES I - ANALISADOR LEXICO DA LINGUAGEM PASCAL

ESTA CLASSE IMPLEMENTA UM AUTOMATO FINITO DETERMINISCO QUE RECONHECE A LINGUAGEM
PASCAL. TODA A LOGICA DO AUTOMATO ESTA NA FUNCAO AFD::start(...) E OUTRAS FUNCOES
AUXILIARES QUE ESTAO DEFINIDAS ABAIXO, JUNTO COM AS VARIAVEIS ESTATICAS DE CON-
TROLE.
--------------------------------------------------------------------------------*/


/************ INICIO DAS VARIAVEIS STATICAS *************/
int AFD::current_position = 0;
int AFD::initial_position = 0;
int AFD::numberLine = 1;
int AFD::state = START;
vector<Token> AFD::tokens;
unordered_map<string, string> AFD::keyWords;
/************ FIM DAS VARIAVEIS STATICAS **************/



/*********** INICIO DAS DEFINICOES DAS FUNCOES STATICAS **********/
void AFD::addToken(string code, string category, bool isKeyWord){

  string token = code.substr(initial_position, current_position - initial_position);

  if(isKeyWord){

    string lowerToken = token;//code.substr(initial_position, current_position - initial_position);
    transform(lowerToken.begin(), lowerToken.end(), lowerToken.begin(), ::tolower);//converte para minúsculo

    unordered_map<string, string>::iterator key = keyWords.find(lowerToken);
    if( key != keyWords.end() ){

      category = keyWords[lowerToken];//seta a categoria associada ao token

    }

  }

  if(category.compare("inteiro") == 0){
    Token newToken(token, category, numberLine, "integer");
    tokens.push_back(newToken);
  }else if(category.compare("real") == 0){
    Token newToken(token, category, numberLine, "real");
    tokens.push_back(newToken);
  }else if(category.compare("boolean") == 0){
    Token newToken(token, category, numberLine, "boolean");
    tokens.push_back(newToken);
  }else{
    Token newToken(token, category, numberLine);
    tokens.push_back(newToken);
  }

  initial_position = current_position;//atualiza a posição de inicio do novo token
  --current_position;//decrementa para não pular um simbolo na prox iteracao
  restartState();//state = START

}

void AFD::countLine(){

  numberLine++;

}

int AFD::getLine(){

  return numberLine;

}

int AFD::getState(){

  return state;

}

vector<Token>::iterator AFD::getTokens(){
  return tokens.begin();
}

vector<Token>::iterator AFD::getEnd_Tokens(){
  return tokens.end();
}

void AFD::ignoreToken(){

  initial_position = current_position + 1;

}

void AFD::initMap_KeyWords(){

  string words[] = {"program", "var", "integer", "real", "boolean", "procedure", "function","begin", "end", "if", "then", "else", "while", "do"};
  unsigned int n = sizeof(words) / sizeof(words[0]);

  keyWords["and"] = "operador-multiplicativo";
  keyWords["or"] = "operador-aditivo";
  keyWords["not"] = "operador-logico";
  keyWords["true"] = "boolean";
  keyWords["false"] = "boolean";

  for(unsigned int i = 0; i < n; i++){

    keyWords[words[i]] = "palavra-resevada";

  }

}

void AFD::restartState(){

  state = START;

}

void AFD::start(string code, bool isComplexNumber){

  int sizeCode = code.size();
  int count_open_close_Comment = 0;
  int count_terms_complex_number = 0;
  int first_digit_position = 0;//posicao do primeiro digito do possivel numero complexo
  int i_position = 0;//posicao do i do possivel numero complexo
  initMap_KeyWords();//inicializa as palavra-resevadas

  for(; sizeCode >= current_position; ++current_position){

    switch (state) {

      case START://estado inicial

        if(code[current_position] == '{'){//é uma abertura de comentário

          state = COMMENT;
          count_open_close_Comment++;

        }else if(code[current_position] == ' ' || code[current_position] == '\t' || code[current_position] == '\r'){//é espaço em branco ou tabulacao ou novo paragrafo

          ignoreToken();//atualiza a posicao initial_position para o prox simbolo: current_position + 1

        }else if( isalpha(code[current_position]) ){//é um identificador

          state = IDENTIFIER;

        }else if( isdigit(code[current_position] )){//é um dígito

          state = DIGIT;


          /* TESTE DOS DELIMITADORES ABAIXO */
        }else if(code[current_position] == ':'){

          state = DELIMITER_TWO_POINT;

        }else if(code[current_position] == ';'){

          state = DELIMITER_SEMICOLON;

        }else if(code[current_position] == '.'){

          state = DELIMITER_DOT;

        }else if(code[current_position] == ','){

          state = DELIMITER_COMMA;

        }else if(code[current_position] == '('){

          state = DELIMITER_BRACKET_OPEN;

        }else if(code[current_position] == ')'){

          state = DELIMITER_BRACKET_CLOSE;


          /* TESTE DOS OPERADORES MATEMÁTICOS BÁSICOS ABAIXO */
        }else if(code[current_position] == '+'){

          state = OPERATOR_ADDITIVE_SUM;

        }else if(code[current_position] == '-'){

          state = OPERATOR_ADDITIVE_SUBTRACTION;

        }else if(code[current_position] == '*'){

          state = OPERATOR_MULTIPLICATIVE_MULTIPLICATION;

        }else if(code[current_position] == '/'){

          state = OPERATOR_MULTIPLICATIVE_DIVISION;

        }else if(code[current_position] == '\n'){//se for uma quebra de linha

          countLine();//incrementa numberLine
          ignoreToken();//atualiza a posicao initial_position para o prox simbolo: current_position + 1


          /* TESTE DOS OPERADORES RELACIONAIS ABAIXO */
        }else if(code[current_position] == '='){

          state = RELATION_OPERATOR_EQUALITY;

        }else if(code[current_position] == '>'){

          state = RELATION_OPERATOR_BIGGER;

        }else if(code[current_position] == '<'){

          state = RELATION_OPERATOR_SMALLER;

        }else if(code[current_position] == '\''){

          ignoreToken();//atualiza a posicao initial_position para o prox simbolo: current_position + 1
          state = OPENING_STRING;

        }else if(code[current_position] == '\0'){

          ;

        }else if(code[current_position] == '}'){//comentario não aberto

          state = COMMENT_NOT_CLOSE;

        }else{

          state = UNKNOW_SYMBOL;
          --current_position;//decrementa para o simbolo entrar no case na prox iteracao

        }

      break;

      case COMMENT://comentários

        if(code[current_position] == '}'){//se fechar comentário

          restartState();//state = START;
          ignoreToken();//atualiza a posicao initial_position para o prox simbolo: current_position + 1

        }else if(code[current_position] == '\0'){

          state = COMMENT_NOT_CLOSE;
          --current_position;//decrementa para o simbolo entrar no case na prox iteracao

        }

      break;

      case IDENTIFIER://é um identificador

        if( isalpha(code[current_position]) ){//se é uma letra do alfabeto
          ;
        }else if( isdigit(code[current_position]) ){//se é dígito
          ;
        }else if(code[current_position] == '_'){//se é underscore
          ;
        }else if(code[current_position] == '\0'){//se chegou ao fim de code

          addToken(code, "identificador", true);//verifica tambem se é palavra chave

        }else if(!isalpha(code[current_position])){//se for qualquer outra coisa, adiciona o token

          addToken(code, "identificador", true);//verifica tambem se é palavra chave

        }

      break;

      case DIGIT://é um dígito inteiro

        if( isdigit(code[current_position]) ){//se é dígito inteiro
          ;
        }else if(code[current_position] == '.'){

          state = REAL_NUMBER;

        }else if(code[current_position] == '+' && isComplexNumber){//se encontrar o + pode ser numero complexo

          state = COMPLEX_NUMBER;
          count_terms_complex_number += 2; //ja viu um numero e um +

          int i = current_position -1;
          while(isdigit(code[i])){

            first_digit_position = i--;

          }

        }else if( !isdigit(code[current_position]) ){//se chegou ao fim de code

          addToken(code, "inteiro");
          restartState();

        }else if(code[current_position] == '\n'){//se for algo diferente de um dígito

          addToken(code, "inteiro");
          countLine();
          restartState();

        }

      break;

      case REAL_NUMBER:

        if( isdigit(code[current_position] )){//se é dígito inteiro
          ;
        }else if(code[current_position] == '\0'){//se chegou ao fim de code

          addToken(code, "real");

        }else{

          addToken(code,"real");

        }

      break;

      case DELIMITER_TWO_POINT://é o delimitador ':'

        if(code[current_position] == '='){//se é uma atribuição

          state = ATRIBUITION;

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

        if( isdigit(code[current_position] )){//se é dígito inteiro

          state = REAL_NUMBER;

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

        if(code[current_position] == '/'){//se encontrar duas barras seguidas, é comentario de linha

          state = LINE_COMMENT;

        }else{

          addToken(code,"operador-multiplicativo");

        }

      break;

      case RELATION_OPERATOR_EQUALITY:// ==

        addToken(code,"operador-relacional");

      break;

      case RELATION_OPERATOR_BIGGER:// >

        if(code[current_position] == '='){

          state = RELATION_OPERATOR_BIGGER_OR_EQUAL;

        }else{

          addToken(code,"operador-relacional");

        }

      break;

      case RELATION_OPERATOR_SMALLER:// <

        if(code[current_position] == '='){

          state = RELATION_OPERATOR_SMALLER_OR_EQUAL;

        }else if(code[current_position] == '>'){

          state = RELATION_OPERATOR_DIFFERENT;

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

        if(isdigit(code[current_position]) || isalpha(code[current_position]) ){
          ;
        }else if(code[current_position] == '\''){

          addToken(code, "string");
          restartState();//state = START;
          ++current_position;
          ignoreToken();//atualiza a posicao initial_position para o prox simbolo: current_position + 1

        }else{

          cout << "NAO FECHOU STRING NA LINHA: " << numberLine << endl;

          return;

        }

      break;

      case COMMENT_NOT_CLOSE://error

        cout << "COMENTARIO NAO FECHADO NA LINHA: " << numberLine << endl;

        return;

      break;

      case UNKNOW_SYMBOL://error

        cout << "SIMBOLO DESCONHECIDO " << code[current_position] << " NA LINHA: " << numberLine << endl;

        return;

      break;

      case LINE_COMMENT:// //

        if(code[current_position] == '\n'){

          restartState();
          countLine();
          ignoreToken();

        }else{
          ;
        }

      break;

      case COMPLEX_NUMBER:// [0...9]+i[0...9]

        if(code[current_position] == 'i' && count_terms_complex_number == 2){

          ++count_terms_complex_number;//ja viu 3 de 4 termos
          i_position = current_position;

        }else if( isdigit(code[current_position]) && count_terms_complex_number == 3){

          ++count_terms_complex_number;//ja viu 4 de 4 termos

        }else if( code[current_position] == '\n' && count_terms_complex_number == 4){

          addToken(code, "numero-complexo");
          countLine();
          restartState();
          count_terms_complex_number = 0;

        }else if( ( !isdigit(code[current_position]) ) && count_terms_complex_number == 4){

          restartState();
          addToken(code, "numero-complexo");
          count_terms_complex_number = 0;

        }else if( !isdigit(code[current_position]) && count_terms_complex_number < 4){//não e complexo

          int aux_current_position = current_position;//guarda a posicao de onde parou
          current_position = initial_position = 0;//define a posicao inicial da substring
          count_terms_complex_number = 0;//reseta o contador de termos de numero-complexo
          restartState();//reseta o estado

          //pega a substring [0...9]+i
          string subCode = code.substr(first_digit_position, i_position - first_digit_position + 1);

          start(subCode, false);//roda o automato para a substring selecionada, agora setando false para o numero-complexo
          isComplexNumber = true;//reseta a variavel por que pode ter mais numeros complexos a frente
          restartState();
          initial_position = aux_current_position;
          current_position = aux_current_position - 1;//seta a posicao de onde parou - 1 para que o simbolo seja processado na prox rodada

        }else if( isdigit(code[current_position] ) ){
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

void AFD::showTokens(){

  vector<Token>::iterator it = tokens.begin();
  for(; it != tokens.end(); it++){

    cout << "token: " << it->getToken() << " \t\tcategoria: " << it->getCategory() << "\t\ttype: " << it->getType() << " \t\t\tlinha: " << it->getLine() << endl;

  }

}
/*********** FIM DAS DEFINICOES DAS FUNCOES STATICAS **********/
