#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <map>
#include <cmath>

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

private:
    std::map<std::string, double> symbols;
};

class Parser {
public:
    Parser(Lexer &lexer, Token currentToken);
    void parse();
    SymbolTable getSymbolTable() const;

private:
    void assignment();
    double expression();
    double term();
    double factor();
    void consume(Token::Type type);
    Lexer& lexer;
    Token currentToken;
    SymbolTable symbolTable;
    bool error;
};
#endif
