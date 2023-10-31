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
        END,        // Input의 끝
        INVALID     // 잘못된 입력
    };

    Token(Type type, std::string value);

    Type type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& input);
    Token getNextToken();

private:
    const std::string input;
    size_t position;
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
