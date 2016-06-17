#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

using namespace std;

class Token {

private:
    string token_;
    string category_;
    int line_;
    string type_;
public:
    Token(const string t, const string c, const int l, const string tp = "");
    string getToken() const;
    string getCategory() const;
    string getType() const;
    void setType(const string t);
    void setCategory(const string t);
    int getLine() const;
};

#endif
