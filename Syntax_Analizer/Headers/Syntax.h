#ifndef __Syntax_H__
#define __Syntax_H__

#include "Lexical.h"
#include "File.h"
#include "Context_Free_Grammar.h"
#include "Semantic.h"

#include <stdexcept>//excecao

using namespace std;

// class Context_Free_Grammar;

class Syntax{

private:
    Lexical* lexical_;
    // Context_Free_Grammar *grammar_;
    vector<Token>::iterator tb_token_;
    vector<Token>::iterator endTb_token_;
    static Syntax *instance_;//singleton
    Syntax();

public:
    ~Syntax();
    static Syntax* getInstance();
    inline bool compareToken(string t);//inline
    inline bool compareCategory(string c);//inline
    vector<Token>::iterator getTb_Token();
    string get_currentToken();
    void setTb_Token(vector<Token>::iterator it);
    void init_tbTokens();
    bool lookahead_token(string t);
    bool lookahead_category(string c);
    void program();
    inline void nextToken();//inline
    void check_scope();
    void error(string m, string f);
};

inline bool Syntax::compareToken(string t){
    return (tb_token_->getToken().compare(t) == 0) ? true:false;
}

inline bool Syntax::compareCategory(string c){
    return (tb_token_->getCategory().compare(c) == 0) ? true:false;
}

inline void Syntax::nextToken(){
    ++tb_token_;
}

#endif
