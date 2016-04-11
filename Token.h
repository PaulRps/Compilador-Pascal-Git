#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

using namespace std;

class Token {

private:
    string token;
    string category;
    string type;
    int line;
public:
    Token(string t, string c, int l, string tp = "");
    string getToken();
    string getCategory();
    string getType();
    void setType(string t);
    void setCategory(string t);
    int getLine();
};

#endif
