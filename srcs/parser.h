#ifndef PARSER_H
#define PARSER_H

#include "treenode.h"
#include <iostream>
#include <cmath>
#include <vector>

class Parser {
private:
    Lexer lexer;
    Token currentToken;
    SymbolTable& symbolTable;
    bool isParsed;
    int chk_ID;
    int chk_CONST;
    int chk_OP;
    int chk_LParen;
    std::string output_Line;
    std::string errorMessage;
    std::vector <std::string>isdefine;

    void error(const std::string& message) {
		errorMessage.append("\e[31;1m(Error)\e[37;0m" + message + ". Got: " + currentToken.token_string + "\n");
        isParsed = false;
		lexer.position++;
        currentToken = lexer.lexical();
    }

    void eat(Token::Type tokenType) {
        if (currentToken.next_token == tokenType) {
            if (tokenType == Token::IDENT) chk_ID++;
            else if (tokenType == Token::CONST)chk_CONST++;
            else if (tokenType == Token::ADD_OP || tokenType == Token::MUL_OP)chk_OP++;
            currentToken = lexer.lexical();
        } else {
            error("Expected token type: " + std::to_string(tokenType));
        }
    }

public:
    Parser(const std::string& input, SymbolTable& symbolTable) : lexer(input), currentToken(lexer.lexical()), isParsed(true), symbolTable(symbolTable) {}

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
