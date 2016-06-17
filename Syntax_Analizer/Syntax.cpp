
#include "Syntax.h"

// Lexical* lexical_ = nullptr;
// vector<Token>::iterator tb_token_;
// vector<Token>::iterator endTb_token_;
Syntax *Syntax::instance_ = nullptr;


Syntax::Syntax(){
    lexical_ = Lexical::getInstance();
}

Syntax::~Syntax(){
    delete [] lexical_;
    delete [] instance_;
}

Syntax *Syntax::getInstance(){
    if(!instance_)
        instance_ = new Syntax;
    return instance_;
}

vector<Token>::iterator Syntax::getTb_Token(){
    return tb_token_;
}

string Syntax::get_currentToken(){
    return tb_token_->getToken();
}

void Syntax::setTb_Token(vector<Token>::iterator it){
    tb_token_ = it;
}

void Syntax::init_tbTokens(){
    tb_token_ = lexical_->getTokens();
    endTb_token_ = lexical_->getEnd_Tokens();
}

bool Syntax::lookahead_token(string t){
    if((tb_token_+1) == endTb_token_) return false;
    return ((tb_token_+1)->getToken()).compare(t) == 0 ? true:false;
}

bool Syntax::lookahead_category(string c){
    if((tb_token_+1) == endTb_token_) return false;
    return ((tb_token_+1)->getCategory()).compare(c) == 0 ? true:false;
}

void Syntax::program(){

    string code = File::getInstance()->dataInput("C:\\Users\\paulo\\Documents\\GitHub\\Compilador-Pascal-Git\\Data_input\\code.txt");//LER OS DADOS DE ENTRADA


    lexical_->start(code);//EXECUTA O Lexical SOBRE A ENTRADA LIDA E GERA A TABELA DE SIMBOLOS
    // lexical_->showTokens();//EXIBE OS TOKENS EXTRAIDOS DA


    init_tbTokens();

    Semantic::getInstance()->init_Semantic();//cria a pilha


    if(compareToken("program")){

        nextToken();

        if(compareCategory("identificador")){

            getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao
            Semantic::getInstance()->push(& (*getTb_Token()) );//guarda o id do programa
            nextToken();

            if(compareToken(";")){

                nextToken();
                Context_Free_Grammar *grammar = new Context_Free_Grammar;
                grammar->variables_statements();//declaracao de variaveis
                grammar->subPrograms_statements();//declaracao de subprogramas
                grammar->composite_command();//comando composto

                if(compareToken(".")){
                    //fim
                }else{
                    Error::error_message("expected '.' in end program instead '", "Syntax.cpp:program()");
                }

            }else{
                Error::error_message("expected ';' after program id instead '", "Syntax.cpp:program()");
            }

        }else{
            //erro identificador
            Error::error_message("expected 'id' after program instead '","Syntax.cpp::program()");
        }

    }else{
        //erro program
        Error::error_message("expected 'program'  instead '","Syntax.cpp::program()");
    }

}

void Syntax::check_scope(){

    if( Semantic::getInstance()->search_current_scope(&(*getTb_Token())) ){

    }else{

        if( Semantic::getInstance()->search_all_scope(&(*getTb_Token())) ){

        }else{
            Error::error_message("variable 'id' don't declared '"," in ");
        }
    }
}

// void Syntax::error(string m, string f){
//
//   cout << "\nERROR::symbol don't expected in "<< f << "::" << tb_token_->getLine() << "  " << m << tb_token_->getToken() <<"' " << endl;
//
//   throw runtime_error("error");
// }
