#ifndef __Lexical_H__
#define __Lexical_H__

#include "Token.h"
#include <string>
#include <iostream>
#include <ctype.h>
#include <vector>
//#include <tr1/unordered_map>//map de key words on linux and windows
#include <unordered_map>//on mac
#include <algorithm>//função transform(, ::tolower)


#define START 0
#define COMMENT 1
#define IDENTIFIER 2
#define DIGIT 3
#define REAL_NUMBER 4
#define DELIMITER_TWO_POINT 5
#define ATRIBUITION 6
#define DELIMITER_SEMICOLON 7
#define DELIMITER_DOT 8
#define DELIMITER_COMMA 9
#define DELIMITER_BRACKET_OPEN 10
#define DELIMITER_BRACKET_CLOSE 11
#define OPERATOR_ADDITIVE_SUM 12
#define OPERATOR_ADDITIVE_SUBTRACTION 13
#define OPERATOR_MULTIPLICATIVE_MULTIPLICATION 14
#define OPERATOR_MULTIPLICATIVE_DIVISION 15
#define RELATION_OPERATOR_EQUALITY 16
#define RELATION_OPERATOR_BIGGER 17
#define RELATION_OPERATOR_SMALLER 18
#define RELATION_OPERATOR_BIGGER_OR_EQUAL 19
#define RELATION_OPERATOR_SMALLER_OR_EQUAL 20
#define OPENING_STRING 21
#define COMMENT_NOT_CLOSE 22
#define UNKNOW_SYMBOL 23
#define LINE_COMMENT 24
#define COMPLEX_NUMBER 25
#define RELATION_OPERATOR_DIFFERENT 26

using namespace std;
//using namespace tr1;

class Lexical{

private:
  static int current_position_;
  static int initial_position_;
  static int numberLine_;
  static int state_;
  static vector<Token> tokens_;
  static unordered_map<string, string> keyWords_;

public:
  //AFD();
  static void addToken(string code, string category, bool isKeyWord = false);
  static void countLine();
  static int getState();
  static int getLine();
  static vector<Token>::iterator getTokens();
  static vector<Token>::iterator getEnd_Tokens();
  static void ignoreToken();
  static void initMap_KeyWords();
  static void restartState();
  static void start(string code, bool isComplexNumber = true);
  static void showTokens();

};


#endif
