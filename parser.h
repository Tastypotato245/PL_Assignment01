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
    int chk_ID;
    int chk_CONST;
    int chk_OP;
    std::string output_Line;
    std::string errorMessage;

    void error(const std::string& message) {
        std::cout << "Error parsing: " << message << ". Got: " << currentToken.value << "\n";
        isParsed = false;
    }

    void eat(Token::Type tokenType) {
        if (currentToken.type == tokenType) {
            if (tokenType == Token::IDENT) chk_ID++;
            else if (tokenType == Token::CONST)chk_CONST++;
            else if (tokenType == Token::ADD_OP || tokenType == Token::MUL_OP)chk_OP++;
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
