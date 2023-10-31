#ifndef PARSER_H
#define PARSER_H

#include "treenode.h"
#include <iostream>
#include <cmath>


class Parser {
private:
    Lexer lexer;
    Token currentToken;
    SymbolTable& symbolTable;
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
    Parser(const std::string& input, SymbolTable& symbolTable) : lexer(input), currentToken(lexer.getNextToken()), isParsed(true), symbolTable(symbolTable) {}

//    Parser(Lexer &lexer, Token currentToken) : lexer(lexer), currentToken(currentToken), isParsed(true) {}

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
