#include "Semantic.h"


Semantic* Semantic::instance_ = nullptr;

Semantic::Semantic(): topStack_(0), baseStack_(0), count_begin_end_(0){}

Semantic::~Semantic(){
    delete [] instance_;
}

Semantic* Semantic::getInstance(){
    if(!instance_){
        instance_ = new Semantic;
    }
    return instance_;
}

void Semantic::init_Semantic(){

    Token aux{"#","begin_scope", this->baseStack_};
    this->scopes_.push_back( aux );
    this->baseStack_ = this->topStack_;
    this->topStack_++;
}

bool Semantic::push(Token *id){

    if(!search_current_scope(id)){
        scopes_.emplace( scopes_.begin() + topStack_, *id);
        ++topStack_;
        return true;
    }else{
        return false;
    }
}

bool Semantic::pop(){

    if(!isEmpty()){
        scopes_.pop_back();
        --topStack_;
        return true;
    }else{
        return false;
    }
}

bool Semantic::pop_scope(){

    if(!isEmpty()){

        int aux = scopes_[baseStack_].getLine();//pega a posicao da base anterior
        scopes_.erase(scopes_.begin() + baseStack_, scopes_.begin() + topStack_);

        topStack_ = baseStack_;
        baseStack_ = aux;//restaura a base do escopo anterior a essa - gambiarra

        return true;

    }else{
        return false;
    }
}

bool Semantic::isEmpty() const{
    return (topStack_ == 0)? true:false;
}

bool Semantic::PcT_isEmpty() const{
    return PcT_.empty() ? true: false;
}

void Semantic::PcT_clear(){
    PcT_.clear();
}

void Semantic::end_scope(){

    Token aux {"#","end_scope", this->baseStack_};
    scopes_.emplace( scopes_.begin() + this->topStack_, aux  );
    baseStack_ = topStack_;
    topStack_++;
}

bool Semantic::search_all_scope(Token *id){

    bool result = false;
    for(int i = 1; i < topStack_; ++i){
        if( id->getType().compare( "procedure" ) == 0 && scopes_[i].getType().compare("procedure") == 0){

            if(id->getToken().compare( scopes_[i].getToken() ) == 0){
                id->setType( scopes_[i].getType() );
                return true;
            }

        }else if(id->getType().compare( "procedure" ) != 0 && scopes_[i].getType().compare("procedure") != 0){

            if(id->getToken().compare( scopes_[i].getToken() ) == 0){
                id->setType( scopes_[i].getType() );
                return true;
            }
        }
    }

    return result;
}

bool Semantic::search_current_scope(Token *id){

    if(isEmpty()) return false;
    for(int i = baseStack_ + 1; i < topStack_; ++i){

        if( id->getType().compare( "procedure" ) == 0 && scopes_[i].getType().compare("procedure") == 0){

            if(id->getToken().compare( scopes_[i].getToken() ) == 0){
                id->setType( scopes_[i].getType() );
                return true;
            }

        }else if(id->getType().compare( "procedure" ) != 0 && scopes_[i].getType().compare("procedure") != 0){

            if(id->getToken().compare( scopes_[i].getToken() ) == 0){
                id->setType( scopes_[i].getType() );
                return true;
            }
        }

    }
    return false;
}

void Semantic::count_begin(){
    ++count_begin_end_;
}

void Semantic::count_end(){
    --count_begin_end_;
}

bool Semantic::check_end_scope() const{
    return (count_begin_end_ == 0)? true: false;
}

void Semantic::setType_ID( const int qtd_id, const string type){

    int top = topStack_-1;
    for(int i = 0; i < qtd_id; ++i){
        scopes_[top--].setType(type);
        //(SyntaticalAnalizer::getTb_Token()-i)->setType(type);
    }
}

bool Semantic::push_in_PcT(Token const *id){
    PcT_.push_back(*id);
    return 1;
}

// bool Stack::push_in_stackOperators(string op){
//   stack_operators_.push_back(op);
//   return true;
// }

string Semantic::compute_expression(vector<Token> expression){

    vector<Token>::iterator it = expression.begin();
    for(int i = 0; expression.size() > 1; ++it, ++i){

        if(it->getCategory().compare("operador-unario") == 0 || it->getCategory().compare("operador-logico") == 0){

            string result = check_type( &(*(it-1)), nullptr, &(*it) );
            Token t(result,result,-1,result);
            expression[i] = t;
            --i;
            expression.erase( expression.begin() + i);
            --it;

        }else if(it->getCategory().compare("operador-aditivo") == 0 || it->getCategory().compare("operador-multiplicativo") == 0 || it->getCategory().compare("operador-relacional") == 0 || it->getCategory().compare("atribuicao") == 0){

            string result = check_type( &(*(it-2)), &(*(it-1)), &(*it) );
            Token t(result,result,-1,result);
            expression[i] = t;
            --i;
            expression.erase( expression.begin() + i);
            --it;
            --i;
            expression.erase(expression.begin() + i);
            --it;

        }

    }

    return expression[0].getToken();

}

string Semantic::throw_error_type( Token const *op1, Token const *op2, Token const *op) const{

    string error = "\nERROR:: operands of incompatible types '" + op1->getToken() + "' (" + op1->getType() + ") " + op->getToken() + " '" + op2->getToken() + "' (" + op2->getType() + ") " + "on line: " + to_string(op1->getLine());

    return error;
}

string Semantic::check_type(Token const *op1, Token const *op2, Token const *op) const{


    if(op->getCategory().compare("operador-logico") == 0 ){

        if(op1->getType().compare("boolean") == 0){
            return "boolean";
        }else{

            throw runtime_error( throw_error_type(op1, op2, op) );
        }

    }else if(op->getCategory().compare("operador-unario") == 0 ){

        if(op1->getType().compare("integer") == 0){
            return "integer";
        }else if(op1->getType().compare("real") == 0){
            return "real";
        }else{
            throw runtime_error("tipos incompativeis: sinal x booolean!");
        }

    }else if(op->getCategory().compare("operador-multiplicativo") == 0 || op->getCategory().compare("operador-aditivo") == 0){

        if(op->getToken().compare("or") == 0 || op->getToken().compare("and") == 0){

            if(op1->getType().compare("boolean") == 0 && op2->getType().compare("boolean") == 0){

                return "boolean";

            }else{
                throw runtime_error( throw_error_type(op1, op2, op) );
            }

        }else if(op1->getType().compare("integer") == 0 && op2->getType().compare("integer") == 0){

            return "integer";

        }else if( (op1->getType().compare("integer") == 0 && op2->getType().compare("boolean") == 0) || (op2->getType().compare("integer") == 0 && op1->getType().compare("boolean") == 0)){

            throw runtime_error( throw_error_type(op1, op2, op) );

        }else if( (op1->getType().compare("real") == 0 && op2->getType().compare("boolean") == 0) || (op2->getType().compare("real") == 0 && op1->getType().compare("boolean") == 0)){

            throw runtime_error( throw_error_type(op1, op2, op) );

        }else{//real com inteiro ou inteiro com real

            return "real";
        }

    }else if(op->getCategory().compare("operador-relacional") == 0){

        if( op->getToken().compare("=") == 0 || op->getToken().compare("<>") == 0){

            if( (op1->getType().compare("boolean") == 0 && op2->getType().compare("integer") == 0)  || (op2->getType().compare("boolean") == 0 && op1->getType().compare("integer") == 0)){

                throw runtime_error( throw_error_type(op1, op2, op) );

            }else if( (op1->getType().compare("boolean") == 0 && op2->getType().compare("real") == 0) || (op2->getType().compare("boolean") == 0 && op1->getType().compare("real") == 0) ){

                throw runtime_error( throw_error_type(op1, op2, op) );

            }else{

                return "boolean";
            }

        }else{

            if( op1->getType().compare("boolean") == 0 && op2->getType().compare("boolean") == 0){

                throw runtime_error( throw_error_type(op1, op2, op) );

            }else if( (op1->getType().compare("boolean") == 0 && op2->getType().compare("integer") == 0)  || (op2->getType().compare("boolean") == 0 && op1->getType().compare("integer") == 0)){

                throw runtime_error( throw_error_type(op1, op2, op) );

            }else if( (op1->getType().compare("boolean") == 0 && op2->getType().compare("real") == 0) || (op2->getType().compare("boolean") == 0 && op1->getType().compare("real") == 0) ){

                throw runtime_error( throw_error_type(op1, op2, op) );

            }else{

                return "boolean";
            }

        }

    }else if(op->getCategory().compare("atribuicao") == 0){

        if( (op1->getType().compare("integer") == 0) ){

            if(op2->getType().compare("integer") != 0){

                throw runtime_error( throw_error_type(op1, op2, op) );

            }else{
                return "integer";
            }


        }else if((op1->getType().compare("real") == 0)){

            if(op2->getType().compare("boolean") == 0){

                throw runtime_error( throw_error_type(op1, op2, op) );

            }else{
                return "real";
            }

        }else if((op1->getType().compare("boolean") == 0)){

            if(op2->getType().compare("boolean") != 0){

                throw runtime_error( throw_error_type(op1, op2, op) );

            }else{

                return "boolean";
            }

        }


    }

    throw runtime_error("Semantic.cpp::check_type() faltou retorno!");

}

Token* Semantic::search_and_get_ID( const string id){

    if(isEmpty()) return nullptr;
    for(int i = topStack_-1; i >= baseStack_+1; --i){
        if( scopes_[i].getToken().compare( id ) == 0 ){
            return &scopes_[i];
        }
    }

    return nullptr;
}

vector<Token> Semantic::post_fix_order(){

    vector<Token>::iterator it = PcT_.begin();
    stack<Token> operators;
    vector<Token> expression;

    for(; it != PcT_.end(); ++it){

        if(it->getType().compare("integer") == 0 || it->getType().compare("boolean") == 0 || it->getType().compare("real") == 0){

            expression.push_back(*it);

        }else if(it->getCategory().compare("operador-multiplicativo") == 0 || it->getCategory().compare("operador-aditivo") == 0 || it->getCategory().compare("operador-relacional") == 0 || it->getCategory().compare("atribuicao") == 0 || it->getCategory().compare("operador-unario") == 0 || it->getCategory().compare("operador-logico") == 0) {

            if(operators.empty() || operators.top().getToken().compare("(") == 0){

                operators.push(*it);

            }else{

                if(it->getCategory().compare("operador-multiplicativo") == 0){

                    if(operators.top().getCategory().compare("operador-relacional") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-aditivo") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-multiplicativo") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("atribuicao") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-unario") == 0 || operators.top().getCategory().compare("operador-logico") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }

                }else if(it->getCategory().compare("operador-aditivo") == 0){

                    if(operators.top().getCategory().compare("operador-relacional") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-aditivo") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-multiplicativo") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;
                        //talvez de merda aqui
                    }else if(operators.top().getCategory().compare("atribuicao") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-unario") == 0 || operators.top().getCategory().compare("operador-logico") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }

                }else if(it->getCategory().compare("operador-relacional") == 0){

                    if(operators.top().getCategory().compare("operador-relacional") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-aditivo") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }else if(operators.top().getCategory().compare("operador-multiplicativo") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }else if(operators.top().getCategory().compare("atribuicao") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-unario") == 0 || operators.top().getCategory().compare("operador-logico") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }

                }else if(it->getCategory().compare("atribuicao") == 0){

                    if(operators.top().getCategory().compare("operador-relacional") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }else if(operators.top().getCategory().compare("operador-aditivo") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }else if(operators.top().getCategory().compare("operador-multiplicativo") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }else if(operators.top().getCategory().compare("atribuicao") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-unario") == 0){

                        expression.push_back(operators.top());
                        operators.pop();
                        --it;

                    }

                }else if(it->getCategory().compare("operador-unario") == 0 || it->getCategory().compare("operador-logico") == 0){

                    if(operators.top().getCategory().compare("operador-relacional") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-aditivo") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-multiplicativo") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("atribuicao") == 0){

                        operators.push(*it);

                    }else if(operators.top().getCategory().compare("operador-unario") == 0 || operators.top().getCategory().compare("operador-logico") == 0 ){

                        expression.push_back(operators.top());
                        operators.pop();
                        operators.push(*it);

                    }

                }

            }

        }else if(it->getToken().compare("(") == 0){

            operators.push(*it);

        }else if(it->getToken().compare(")") == 0){

            // do{
            //
            //   expression.push_back(operators.top());
            //   operators.pop();
            //
            // }while(operators.top().getToken().compare("(") != 0);
            while(operators.top().getToken().compare("(") != 0){

                expression.push_back(operators.top());
                operators.pop();

            }

            operators.pop();

        }

    }

    while (!operators.empty()) {

        expression.push_back(operators.top());
        operators.pop();

    }

    return expression;

}

// void Semantic::show(){
//
//   for(int i = 0, j = 1; i < topStack_ - 1; ++i){
//
//     if( scopes_[i].getToken().compare("#") == 0 ){
//       cout << "\tScope " << j++ << endl;
//     }else{
//       cout << scopes_[i].getToken() << endl;
//     }
//   }
// }
