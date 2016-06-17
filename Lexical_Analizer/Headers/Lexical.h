#ifndef __Lexical_H__
#define __Lexical_H__

#include "Token.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>//função transform(, ::tolower)


enum State{ START ,
    COMMENT ,
    IDENTIFIER ,
    DIGIT ,
    REAL_NUMBER ,
    DELIMITER_TWO_POINT ,
    ATRIBUITION ,
    DELIMITER_SEMICOLON ,
    DELIMITER_DOT ,
    DELIMITER_COMMA ,
    DELIMITER_BRACKET_OPEN ,
    DELIMITER_BRACKET_CLOSE ,
    OPERATOR_ADDITIVE_SUM ,
    OPERATOR_ADDITIVE_SUBTRACTION ,
    OPERATOR_MULTIPLICATIVE_MULTIPLICATION ,
    OPERATOR_MULTIPLICATIVE_DIVISION ,
    RELATION_OPERATOR_EQUALITY ,
    RELATION_OPERATOR_BIGGER ,
    RELATION_OPERATOR_SMALLER ,
    RELATION_OPERATOR_BIGGER_OR_EQUAL ,
    RELATION_OPERATOR_SMALLER_OR_EQUAL ,
    OPENING_STRING,
    COMMENT_NOT_CLOSE ,
    UNKNOW_SYMBOL ,
    LINE_COMMENT ,
    COMPLEX_NUMBER ,
    RELATION_OPERATOR_DIFFERENT
};

using namespace std;

class Lexical{

private:
    int current_position_;
    int initial_position_;
    int numberLine_;
    enum State state_;
    vector<Token> tokens_;
    unordered_map<string, string> keyWords_;
    static Lexical *instance_;
    Lexical ();

public:
    ~Lexical();
    static Lexical* getInstance();
    inline void addToken(string code, string category, bool isKeyWord = false);
    inline void countLine();
    inline int getState() const;
    inline int getLine() const;
    vector<Token>::iterator getTokens();
    vector<Token>::iterator getEnd_Tokens();
    void ignoreToken();
    void initMap_KeyWords();
    inline void restartState();
    void start(string code, bool isComplexNumber = true);
    void showTokens();

};

inline void Lexical::addToken(string code, string category, bool isKeyWord){

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

inline void Lexical::countLine(){

    numberLine_++;

}

inline int Lexical::getLine() const{

    return numberLine_;

}

inline int Lexical::getState() const{

    return state_;

}

inline void Lexical::restartState(){

    state_ = START;

}


#endif
