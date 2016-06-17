#include "Context_Free_Grammar.h"
#include "Syntax.h"
#include "Callback.h"

Context_Free_Grammar::Context_Free_Grammar(): count_statement_id_(0){}

Context_Free_Grammar::~Context_Free_Grammar(){
    delete [] Syntax::getInstance();
}

void Context_Free_Grammar::variables_statements(){

    if(Syntax::getInstance()->compareToken("var")){

        Syntax::getInstance()->nextToken();
        variables_statements_list();

    }else if(Syntax::getInstance()->lookahead_category("identificador")){//nao tem var e o prox e um ID
        //erro var
        Error::error_message("expected 'var' instead '","12:Context_Free_Grammar.cpp::variables_statements()");
    }

}

void Context_Free_Grammar::variables_statements_list(){

    identifiers_list();

    if(Syntax::getInstance()->compareToken(":")){

        Syntax::getInstance()->nextToken();

        string t = Syntax::getInstance()->get_currentToken();//pega o tipo das variaveis declaradas

        type();

        Semantic::getInstance()->setType_ID(getCount_statement_ID(), t);//seta o tipo de todos os ids declarados

        reset_count_statement_ID();//zera o contador de IDs

        if(Syntax::getInstance()->compareToken(";")){

            Syntax::getInstance()->nextToken();

            if(Syntax::getInstance()->compareCategory("identificador")){//se tiver mais declaracoes de variaveis

                variables_statements_list_2();

            }else if(Syntax::getInstance()->compareToken("procedure") || Syntax::getInstance()->compareToken("begin") || Syntax::getInstance()->compareToken("function") ){//se for subprograma ou comando composto
                //nao faz nada, segue em frente
            }else{
                Error::error_message("error","Context_Free_Grammar.cpp::variables_statements_list()");
            }

        }else{
            //erro variables_statements_list ';'
            Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp::variables_statements_list()");
        }

    }else{
        //erro variables_statements_list ':'
        Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp::variables_statements_list()");
    }

}

void Context_Free_Grammar::variables_statements_list_2(){

    identifiers_list();

    if(Syntax::getInstance()->compareToken(":")){

        Syntax::getInstance()->nextToken();

        string t = Syntax::getInstance()->get_currentToken();//pega o tipo das variaveis declaradas

        type();

        Semantic::getInstance()->setType_ID(getCount_statement_ID(), t);//seta o tipo dos ids

        reset_count_statement_ID();//zera o contador de IDs

        if(Syntax::getInstance()->compareToken(";")){

            Syntax::getInstance()->nextToken();

            if(Syntax::getInstance()->compareCategory("identificador")){//se tiver mais declaracoes de variaveis

                variables_statements_list_2();

            }else if(Syntax::getInstance()->compareToken("procedure") || Syntax::getInstance()->compareToken("begin")){//se for subprograma ou comando composto
                //nao faz nada, segue em frente
            }else{
                Error::error_message("error","Context_Free_Grammar.cpp::variables_statements_list_2()");
            }

        }else{
            //erro variables_statements_list_2 ';'
            Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp::variables_statements_list_2()");
        }

    }else{
        //erro variables_statements_list_2 ':'
        Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp::variables_statements_list_2()");
    }
}

void Context_Free_Grammar::identifiers_list(){

    if(Syntax::getInstance()->compareCategory("identificador")){

        count_statement_ID();//conta a qtd de variaveis declaradas

        if(Semantic::getInstance()->push(& ( *(Syntax::getInstance()->getTb_Token()) ) ) ){//guarda o id na pilha de escopo
            //id e unico
        }else{
            Error::error_message("dubplicate id '","Context_Free_Grammar.cpp::identifiers_list()");
        }

        Syntax::getInstance()->nextToken();
        identifiers_list_2();

    }else{//TEM QUE SER VAZIO AQUI PARA SUBPROGRAMAS

    }

}

void Context_Free_Grammar::identifiers_list_2(){

    if(Syntax::getInstance()->compareToken(",")){

        Syntax::getInstance()->nextToken();

        if(Syntax::getInstance()->compareCategory("identificador")){

            count_statement_ID();//conta a qtd de variaveis declaradas

            if(Semantic::getInstance()->push( &( *(Syntax::getInstance()->getTb_Token()) ) ) ){//guarda o id
                //id e unico
            }else{
                Error::error_message("dubplicate id '","Context_Free_Grammar.cpp::identifiers_list_2()");
            }

            Syntax::getInstance()->nextToken();
            identifiers_list_2();

        }else{
            //vazio
        }

    }else{
        //vazio
    }

}

void Context_Free_Grammar::type(){

    if(Syntax::getInstance()->compareToken("integer") || Syntax::getInstance()->compareToken("real") || Syntax::getInstance()->compareToken("boolean")){
        Syntax::getInstance()->nextToken();
    }else{
        // erro de tipo
        Error::error_message("unknow type","Context_Free_Grammar.cpp::type()");
    }
}

void Context_Free_Grammar::subPrograms_statements(){
    subPrograms_statements_2();
}

void Context_Free_Grammar::subPrograms_statements_2(){

    if(Syntax::getInstance()->compareToken("procedure") || Syntax::getInstance()->compareToken("function")){//se tiver mais subprogramas

        subProgram_Statement();

        if(Syntax::getInstance()->compareToken(";") ){

            Syntax::getInstance()->nextToken();
            subPrograms_statements_2();

        }else{
            //erro ';'
            Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp:subProgram_Statement_2()");
        }

    }else{
        //vazio
    }

}

void Context_Free_Grammar::subProgram_Statement(){

    if(Syntax::getInstance()->compareToken("procedure")){

        Syntax::getInstance()->nextToken();

        if(Syntax::getInstance()->compareCategory("identificador")){

            Syntax::getInstance()->getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao

            if(Semantic::getInstance()->push( &( *(Syntax::getInstance()->getTb_Token()) ) ) ){//guarda o id do programa na pilha de ids de procedures e verifica se ela ja esta no escopo atual
                //id e unico
            }else{
                Error::error_message("dubplicate id '","Context_Free_Grammar.cpp::subProgram_Statement()");
            }

            Semantic::getInstance()->end_scope();//encerra o escopo anterior e comeca um novo

            Syntax::getInstance()->nextToken();
            arguments();

            if(Syntax::getInstance()->compareToken(";")){

                Syntax::getInstance()->nextToken();
                if(Syntax::getInstance()->compareToken("var"))//se tiver declaracao de variavel

                variables_statements();

                if(Syntax::getInstance()->compareToken("procedure"))//se tiver declaracao de procedure

                subPrograms_statements();

                composite_command();

            }else{
                //erro ';'
                Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
            }

        }else{
            //erro 'id'
            Error::error_message("expected procedure 'id' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
        }

    }else if(Syntax::getInstance()->compareToken("function")){

        Syntax::getInstance()->nextToken();

        if(Syntax::getInstance()->compareCategory("identificador")){

            Syntax::getInstance()->getTb_Token()->setType("procedure");//seta o tipo do ID para diferenciar entre de variavel e funcao

            if(Semantic::getInstance()->push( &( *(Syntax::getInstance()->getTb_Token()) ) ) ){//guarda o id do programa na pilha de ids de procedures e verifica se ela ja esta no escopo atual
                //id e unico
            }else{
                Error::error_message("dubplicate id '","Context_Free_Grammar.cpp::subProgram_Statement()");
            }

            Semantic::getInstance()->end_scope();//encerra o escopo anterior e comeca um novo

            Syntax::getInstance()->nextToken();
            arguments();

            if(Syntax::getInstance()->compareToken(":")){

                Syntax::getInstance()->nextToken();

                type();

                if(Syntax::getInstance()->compareToken(";")){

                    Syntax::getInstance()->nextToken();
                    if(Syntax::getInstance()->compareToken("var"))//se tiver declaracao de variavel

                    variables_statements();

                    if(Syntax::getInstance()->compareToken("procedure") || Syntax::getInstance()->compareToken("function"))//se tiver declaracao de procedure

                    subPrograms_statements();

                    composite_command();

                }else{
                    //erro ';'
                    Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
                }


            }else{
                //erro ':'
                Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
            }

        }else {
            //erro 'id'
            Error::error_message("expected function 'id' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
        }

    }else{
        //erro 'procedure'
        Error::error_message("expected 'procedure' or 'function' instead '","Context_Free_Grammar.cpp:subProgram_Statement()");
    }

}

void Context_Free_Grammar::arguments(){

    if( Syntax::getInstance()->compareToken( "(" ) ){

        Syntax::getInstance()->nextToken();
        parameters_list();

        if( Syntax::getInstance()->compareToken( ")" ) ){
            Syntax::getInstance()->nextToken();
        }else{
            //erro ')'
            Error::error_message("expected ')' instead '","Context_Free_Grammar.cpp:arguments()");
        }

    }else if(Syntax::getInstance()->lookahead_category("identificador")){//se o prox for um id, provavel que seja um erro
        //erro '('
        Error::error_message("expected '(' instead '","Context_Free_Grammar.cpp:arguments()");
    }
}

void Context_Free_Grammar::parameters_list(){

    identifiers_list();

    if(Syntax::getInstance()->compareToken(":")){

        Syntax::getInstance()->nextToken();

        string t = Syntax::getInstance()->get_currentToken();//pega o tipo das variaveis declaradas

        type();

        Semantic::getInstance()->setType_ID(getCount_statement_ID(), t);//seta o tipo de todos os ids declarados

        reset_count_statement_ID();//zera o contador de IDs

        parameters_list_2();

    }else{
        //erro ':'
        Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp:parameters_list()");
    }

}

void Context_Free_Grammar::parameters_list_2(){

    if(Syntax::getInstance()->compareToken(";")){

        Syntax::getInstance()->nextToken();

        if(Syntax::getInstance()->compareCategory("identificador")){//se tiver mais declaracao de variavel

            identifiers_list();

        }else{
            return;//e o caso do vazio
        }

        if(Syntax::getInstance()->compareToken(":")){

            Syntax::getInstance()->nextToken();

            string t = Syntax::getInstance()->get_currentToken();//pega o tipo das variaveis declaradas

            type();

            Semantic::getInstance()->setType_ID(getCount_statement_ID(), t);//seta o tipo de todos os ids declarados

            reset_count_statement_ID();//zera o contador de IDs

            parameters_list_2();

        }else{
            //erro ':'
            Error::error_message("expected ':' instead '","Context_Free_Grammar.cpp:parameters_list_2()");
        }

    }else if(Syntax::getInstance()->compareCategory("identificador")){//se nao colocar ';' e tiver mais identificadores
    //erro ';'
    Error::error_message("expected ';' instead '","Context_Free_Grammar.cpp:parameters_list_2()");
    }else{
        //vazio
    }

}

void Context_Free_Grammar::composite_command(){

    if(Syntax::getInstance()->compareToken("begin")){

        Semantic::getInstance()->count_begin();
        Syntax::getInstance()->nextToken();
        optional_commands();

        if(Syntax::getInstance()->compareToken("end")){

            if(!Semantic::getInstance()->PcT_isEmpty()){

                Semantic::getInstance()->compute_expression( Semantic::getInstance()->post_fix_order() );
                Semantic::getInstance()->PcT_clear();

            }

            Syntax::getInstance()->nextToken();
            Semantic::getInstance()->count_end();

            if(Semantic::getInstance()->check_end_scope()){
                Semantic::getInstance()->pop_scope();
            }else{
                Error::error_message("don't close scope '","Context_Free_Grammar.cpp:composite_command()");
            }

            //nao faz nada
        }else{
            //erro 'end'
            Error::error_message("expected 'end' instead '","Context_Free_Grammar.cpp:composite_command()");
        }

    }
}

void Context_Free_Grammar::optional_commands(){

    commands_list();

}

void Context_Free_Grammar::commands_list(){

    command();
    commands_list_2();

}

void Context_Free_Grammar::commands_list_2(){

    if(Syntax::getInstance()->compareToken(";")){

        vector<Token> a = Semantic::getInstance()->post_fix_order();
        string s = Semantic::getInstance()->compute_expression( a );
        Semantic::getInstance()->PcT_clear();

        Syntax::getInstance()->nextToken();
        command();
        commands_list_2();

    }else{
        //vazio
    }
}

void Context_Free_Grammar::command(){

    if(Syntax::getInstance()->lookahead_token(":=")){//se o prox simbolo for ':=', e o case variable

    variable();

    Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token() -1) ) );//emplinha o id
    Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//emplinha o :=

    Syntax::getInstance()->nextToken();//ja testou o ':=' acima, passa para prox
    expression();

    }else if(Syntax::getInstance()->compareCategory("identificador")){

        procedure_activation();

    }else if(Syntax::getInstance()->compareToken("begin")){

        composite_command();

    }else if(Syntax::getInstance()->compareToken("if")){

        Syntax::getInstance()->nextToken();
        expression();

        Semantic::getInstance()->compute_expression( Semantic::getInstance()->post_fix_order() );//avalia a expressao do if
        Semantic::getInstance()->PcT_clear();//limpa pct

        if(Syntax::getInstance()->compareToken("then")){

            Syntax::getInstance()->nextToken();
            command();
            else_part();

        }else{
            //erro 'then'
            Error::error_message("expected 'then' instead '","Context_Free_Grammar.cpp:command()");
        }

    }else if(Syntax::getInstance()->compareToken("while")){

        Syntax::getInstance()->nextToken();
        expression();

        Semantic::getInstance()->compute_expression( Semantic::getInstance()->post_fix_order() );//avalia a expressao do while
        Semantic::getInstance()->PcT_clear();//limpa pct

        if(Syntax::getInstance()->compareToken("do")){

            Syntax::getInstance()->nextToken();
            command();

        }else{
            //erro 'do'
            Error::error_message("expected 'do' instead '","Context_Free_Grammar.cpp:command()");
        }

    }else if(Syntax::getInstance()->compareToken("for")){

        Syntax::getInstance()->nextToken();

        if(Syntax::getInstance()->compareCategory("identificador")){

            Syntax::getInstance()->nextToken();

            if(Syntax::getInstance()->compareToken(":=")){

                Syntax::getInstance()->nextToken();

                if(Syntax::getInstance()->getTb_Token()->getType().compare("integer") == 0){

                    Syntax::getInstance()->nextToken();

                    if(Syntax::getInstance()->compareToken("to")){

                        Syntax::getInstance()->nextToken();

                        if(Syntax::getInstance()->getTb_Token()->getType().compare("integer") == 0){

                            Syntax::getInstance()->nextToken();

                            if(Syntax::getInstance()->compareToken("do")){

                                Syntax::getInstance()->nextToken();

                                command();

                            }else{
                                //erro 'do' do for
                                Error::error_message("expected 'do' instead '","Context_Free_Grammar.cpp:command()");
                            }

                        }else{
                            //erro tipo for
                            Error::error_message("expected 'integer' instead '","Context_Free_Grammar.cpp:command()");
                        }

                    }else{
                        //erro for
                        Error::error_message("expected 'to' instead '","Context_Free_Grammar.cpp:command()");
                    }

                }else{
                    //erro de tipo
                    Error::error_message("expected 'integer' instead '","Context_Free_Grammar.cpp:command()");
                }

            }else{
                //erro for :=
                Error::error_message("expected ':=' instead '","Context_Free_Grammar.cpp:command()");
            }

        }else{
            //erro id for
            Error::error_message("expected 'id' of 'for' instead '","Context_Free_Grammar.cpp:command()");
        }


    }

}

void Context_Free_Grammar::command_2(){

    if(Syntax::getInstance()->compareToken(":=")){

        Syntax::getInstance()->nextToken();
        expression();
        //checar tipos
    }else{
        procedure_activation_2();
    }
}

void Context_Free_Grammar::expression(){

    simple_expression();
    expression_2();

}

void Context_Free_Grammar::expression_2(){

    if(Syntax::getInstance()->compareCategory("operador-relacional")){

        relational_op();

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token() -1) ) );

        simple_expression();

    }else{
        // vazio
    }

}

void Context_Free_Grammar::expressions_list(){

    expression();
    expressions_list_2();
}

void Context_Free_Grammar::expressions_list_2(){

    if(Syntax::getInstance()->compareToken(",")){

        Syntax::getInstance()->nextToken();
        expression();
        expressions_list_2();

    }else{
        //vazio
    }
}

void Context_Free_Grammar::else_part(){

    if(Syntax::getInstance()->compareToken("else")){

        Syntax::getInstance()->nextToken();
        command();

    }else{
        //vazio
    }

}

void Context_Free_Grammar::simple_expression(){

    if(Syntax::getInstance()->compareToken("+") || Syntax::getInstance()->compareToken("-")){

        Syntax::getInstance()->getTb_Token()->setCategory("operador-unario");
        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );

        Syntax::getInstance()->nextToken();

        term();

        simple_expression_2();

    }else if( check_track() ){//RESOLVER PROBLEMA COM CHECKTRACK DEPOIS

        simple_expression_2();

    }else{
        Error::error_message("expected simple_expression instead '","Context_Free_Grammar.cpp:simple_expression()");
    }

}

void Context_Free_Grammar::simple_expression_2(){

    if(Syntax::getInstance()->compareCategory("operador-aditivo")){

        additive_op();

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token() -1) ) );

        term();
        simple_expression_2();

    }else{
        //vazio
    }

}

void Context_Free_Grammar::relational_op(){

    if(Syntax::getInstance()->compareCategory("operador-relacional")){

        Syntax::getInstance()->nextToken();

    }else{
        //erro unknow relational_op
        //Syntax::getInstance()->error("unknow relational_op ","Context_Free_Grammar.cpp:relational_op()");
    }

}

void Context_Free_Grammar::term(){
    factor();
    term_2();
}

void Context_Free_Grammar::term_2(){

    if(Syntax::getInstance()->compareCategory("operador-multiplicativo")){

        multiplicative_op();

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token() -1) ) );

        factor();
        term_2();

    }else{
        //vazio
    }
}

void Context_Free_Grammar::factor(){

    if(Syntax::getInstance()->compareCategory("identificador")){

        Syntax::getInstance()->check_scope();

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca na pct

        Syntax::getInstance()->nextToken();
        factor_2();

    }else if(Syntax::getInstance()->compareCategory("inteiro")){

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca os numeros inteiros na pct
        Syntax::getInstance()->nextToken();

    }else if(Syntax::getInstance()->compareCategory("real")){

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca os numeros reais na pct
        Syntax::getInstance()->nextToken();

    }else if(Syntax::getInstance()->compareCategory("boolean")){

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca os booleans na pct
        Syntax::getInstance()->nextToken();

    }else if(Syntax::getInstance()->compareToken("(")){

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca na pct

        Syntax::getInstance()->nextToken();
        expression();

        if(Syntax::getInstance()->compareToken(")")){

            Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca na pct
            Syntax::getInstance()->nextToken();

        }else{
            //erro ')'
            Error::error_message("expected ')' instead '","Context_Free_Grammar.cpp:factor()");
        }

    }else if(Syntax::getInstance()->compareToken("not")){

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca na pct

        Syntax::getInstance()->nextToken();
        factor();

    }

}

void Context_Free_Grammar::factor_2(){

    if(Syntax::getInstance()->compareToken("(")){

        Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca na pct

        Syntax::getInstance()->nextToken();
        expressions_list();

        if(Syntax::getInstance()->compareToken(")")){

            Semantic::getInstance()->push_in_PcT( &( *(Syntax::getInstance()->getTb_Token()) ) );//coloca na pct

            Syntax::getInstance()->nextToken();

        }else{
            //erro ')'
            Error::error_message("expected ')' instead '","Context_Free_Grammar.cpp:factor_2()");
        }

    }else{
        //Vazio
    }
}

void Context_Free_Grammar::additive_op(){

    if(Syntax::getInstance()->compareCategory("operador-aditivo")){
        Syntax::getInstance()->nextToken();
    }else{
        //Syntax::getInstance()->error("unknow additive_op ","Context_Free_Grammar.cpp:additive_op()");
    }
}

void Context_Free_Grammar::multiplicative_op(){

    if(Syntax::getInstance()->compareCategory("operador-multiplicativo")){
        Syntax::getInstance()->nextToken();
    }else{
        //Syntax::getInstance()->error("unknow multiplicative_op ","Context_Free_Grammar.cpp:multiplicative_op()");
    }
}

void Context_Free_Grammar::procedure_activation(){

    if(Syntax::getInstance()->compareCategory("identificador")){

        Syntax::getInstance()->nextToken();
        procedure_activation_2();
    }
}

void Context_Free_Grammar::procedure_activation_2(){

    if(Syntax::getInstance()->compareToken("(")){

        Syntax::getInstance()->nextToken();
        expressions_list();

        if(Syntax::getInstance()->compareToken(")")){

            Syntax::getInstance()->nextToken();

        }else{
            Error::error_message("expected ')' instead '","Context_Free_Grammar.cpp:procedure_activation_2()");
        }

    }else{
        //vazio
    }
}

void Context_Free_Grammar::variable(){

    if(Syntax::getInstance()->compareCategory("identificador")){

        Syntax::getInstance()->check_scope();

        Syntax::getInstance()->nextToken();

    }else{
        Error::error_message("expected variable 'id' instead '","Context_Free_Grammar.cpp:variable()");
    }
}


//*********** Aux Functions *******************

bool Context_Free_Grammar::check_track(){

    vector<Token>::iterator aux = Syntax::getInstance()->getTb_Token();
    try{
        // (this->*func)();

        term();
    }catch(runtime_error &e){
        Syntax::getInstance()->setTb_Token(aux);
        return false;
    }
    return true;
}
