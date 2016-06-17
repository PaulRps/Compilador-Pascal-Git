#include "Error.h"

string Error::message_;

void Error::error_message(const string m, const string f, const string t, const int l){

    if(l){
        message_ = "\n\nERROR::symbol don't expected in " + f + "::" + to_string( Syntax::getInstance()->getTb_Token()->getLine() ) + " " + m + Syntax::getInstance()->getTb_Token()->getToken() + "' \n";
    }else{
        message_ = "\n\nERROR::symbol don't expected in " + f + "::" + to_string( l ) + " " + m + " '" + t + "'\n";
    }

    throw runtime_error(message_);
}
