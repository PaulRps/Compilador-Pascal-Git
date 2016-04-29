#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

using namespace std;

class Token {

private:
    string token_;
    string category_;
    string type_;
    int line_;
public:
    Token(string t, string c, int l, string tp = "");
    string getToken() const;
    string getCategory() const;
    string getType() const;
    void setType(string t);
    void setCategory(string t);
    int getLine() const;
};

#endif
