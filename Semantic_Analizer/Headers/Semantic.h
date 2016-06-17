#ifndef __Semantic_H__
#define __Semantic_H__

#include "Token.h"

#include <vector>
#include <stdexcept>
#include <stack>

using namespace std;

class Semantic{
private:
    int topStack_;
    int baseStack_;
    int count_begin_end_;
    vector<Token> scopes_;//empilha as variaveis para checagem de escopo
    vector<Token> PcT_;//empliha as variaveis e os valores para checagem de tipos
    static Semantic *instance_;
    Semantic();

public:
    ~Semantic();
    static Semantic* getInstance();
    void init_Semantic();
    bool push(Token *id);
    bool pop();
    bool pop_scope();
    bool isEmpty() const;
    bool PcT_isEmpty() const;
    void PcT_clear();
    void end_scope();
    bool search_all_scope(Token *id);
    bool search_current_scope(Token *id);
    bool push_in_PcT(Token const *id);

    void count_begin();
    void count_end();
    bool check_end_scope() const;
    void setType_ID(const int qtd_id, const string type);
    string compute_expression(vector<Token> expression);
    string throw_error_type(Token const *op1, Token const *op2, Token const *op) const;
    string check_type(Token const *op1, Token const *op2, Token const *op) const;
    Token* search_and_get_ID( const string id);
    vector<Token> post_fix_order();
};

#endif
