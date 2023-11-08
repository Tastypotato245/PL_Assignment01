#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <map>
#include <cmath>
#include <string>

class Token {
public:
    enum Type {
        IDENT,      // Identifier
        CONST,      // Constant
        ASSIGN_OP,  // :=
        ADD_OP,     // + or -
        MUL_OP,     // * or /
        SEMI_COLON, // ;
        LEFT_PAREN, // (
        RIGHT_PAREN,// )
        END,        // 
        INVALID     // 
    };

    Token(Type next_token, std::string token_string);

    Type next_token;
    std::string token_string;
};

class Lexer {
public:
    Lexer(const std::string& input);
    Token lexical();
    size_t position;

private:
    const std::string input;
};

class SymbolTable {
public:
    void set(const std::string& name, double value);
    bool get(const std::string& name, double& value) const;
    void del(const std::string& name);
    bool exists(const std::string& name) const;
	void print_result() const;
private:
    std::map<std::string, double> symbols;
};


#endif
