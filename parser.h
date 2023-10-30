#ifndef PARSER_H
#define PARSER_H

#include "treenode.h"
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
private:
    Lexer lexer;
    Token currentToken;
    SymbolTable symbolTable;
    bool isParsed;

    void error(const std::string& message) {
        std::cout << "Error parsing: " << message << ". Got: " << currentToken.value << "\n";
        isParsed = false;
    }

    void eat(Token::Type tokenType) {
        if (currentToken.type == tokenType) {
            currentToken = lexer.getNextToken();
        } else {
            error("Expected token type: " + std::to_string(tokenType));
        }
    }

public:
    Parser(const std::string& input) : lexer(input), currentToken(lexer.getNextToken()), symbolTable(), isParsed(true) {}
    Parser(Lexer &lexer, Token currentToken) : lexer(lexer), currentToken(currentToken), symbolTable(), isParsed(true) {}
    ProgramNode* parseProgram();
    StatementsNode* parseStatements();
    StatementNode* parseStatement();
    ExpressionNode* parseExpression();
    TermTailNode* parseTermTail();
    TermNode* parseTerm();
    FactorTailNode* parseFactorTail();
    FactorNode* parseFactor();
};
#endif
