#ifndef _Error_H__
#define _Error_H__

#include "Syntax.h"

#include <stdexcept>
#include <string>

using namespace std;

class Error{
private:
    static string message_;

public:
    static void error_message(const string m, const string f, const string t = "", const int l = 0);

};


#endif
