#include "Error.h"

void Error::error_message(string m, string f){

  // cout << "\nERROR::symbol don't expected in "<< f << "::" << tb_token_->getLine() << "  " << m << tb_token_->getToken() <<"' " << endl;

  throw runtime_error("error");
}
