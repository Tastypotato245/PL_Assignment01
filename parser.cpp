#include "parser.h"
#include "treenode.h"
#include <string>
#include <iostream>
#include <map>

ProgramNode* Parser::parseProgram() {
	//std::cout << " - parseProgram excute\n";
    return new ProgramNode(parseStatements());
}

StatementsNode* Parser::parseStatements() {
    chk_ID = 0;
    chk_CONST = 0;
    chk_OP = 0;
	//std::cout << "\t - parseStatements excute\n";
    StatementNode* statement = parseStatement();
    if (currentToken.type != Token::END) {
        //std::cout << ";" << std::endl;
        output_Line.append(";");
    	eat(Token::SEMI_COLON); 
        std::cout << output_Line << std::endl;
        std::cout << "ID: " << chk_ID << "; CONST:" << chk_CONST << "; OP: " << chk_OP << std::endl;
        output_Line = "\0";
        return new StatementsNode(statement, isParsed, parseStatements());
    }
    std::cout << output_Line << std::endl;
    std::cout << "ID: " << chk_ID << "; CONST:" << chk_CONST << "; OP: " << chk_OP << std::endl;
    return new StatementsNode(statement, isParsed, nullptr);
}

StatementNode* Parser::parseStatement() {
    std::string ident = currentToken.value;
    //std::cout << ident << " :=";
    output_Line.append(ident);
    output_Line.append(" :=");
    eat(Token::IDENT);
    eat(Token::ASSIGN_OP);
	//std::cout << "\t - parseStatement excute : ident : " << ident << "\n";
    ExpressionNode* expression = parseExpression();
    return new StatementNode(isParsed, symbolTable, ident, isParsed, expression);
}

ExpressionNode* Parser::parseExpression() {
	//std::cout << "\t\t - parseExpression excute\n";
    TermNode* term = parseTerm();
    TermTailNode* termTail = parseTermTail();
    return new ExpressionNode(isParsed, symbolTable, term, termTail);
}

TermNode* Parser::parseTerm() {
	//std::cout << "\t\t\t - parseTerm excute\n";
    FactorNode* factor = parseFactor();
    FactorTailNode* factorTail = parseFactorTail();
    return new TermNode(true, symbolTable, factor, factorTail);
}

TermTailNode* Parser::parseTermTail() {
	//std::cout << "\t\t\t - parseTermTail excute\n";
    if (currentToken.type == Token::ADD_OP) {
		//std::cout << "\t\t\t\t term_tail => add_op <term> <term_tail>\n";
        int add_op = (currentToken.value == "+") ? 1 : 2;
        if(add_op==1)//std::cout << " +";
            output_Line.append(" +");
        else //std::cout << " -";
            output_Line.append(" -");
        eat(Token::ADD_OP);
        TermNode* term = parseTerm();
        TermTailNode* termTail = parseTermTail();
        return new TermTailNode(isParsed, symbolTable, add_op, term, termTail);
    }
    else if (currentToken.type == Token::RIGHT_PAREN) {
        std::cout << "(Warning)" << "checking if the parentheses are properly opened and closed" << std::endl;
        eat(Token::RIGHT_PAREN);
        return parseTermTail();
        } 
    
	//std::cout << "\t\t\t\t term_tail => lambda\n";
    return nullptr; 
}

FactorNode* Parser::parseFactor() {
	//std::cout << "\t\t\t - parseFactor excute\n";
    if (currentToken.type == Token::LEFT_PAREN) {
		//std::cout << "\t\t\t\t factor => ( ex )\n";
        //std::cout << "(";
        output_Line.append(" (");
        eat(Token::LEFT_PAREN);
        ExpressionNode* expression = parseExpression();
        //std::cout << ")";
        if(currentToken.type==Token::RIGHT_PAREN) {
            //output_Line.append(" )");
            eat(Token::RIGHT_PAREN);
            return new FactorNode(isParsed, symbolTable, isParsed, expression, isParsed);
        }
        else {
            std::cout << "(Warning)" << "checking if the parentheses are properly opened and closed" << std::endl;
            output_Line.erase(std::find(output_Line.begin(), output_Line.end(), '('));
        }
    } else if (currentToken.type == Token::IDENT) {
		//std::cout << "\t\t\t\t factor => ident\n";
        std::string ident = currentToken.value;
        //std::cout << " " <<ident;
        output_Line.append(" ");
        output_Line.append(ident);
        eat(Token::IDENT);
        return new FactorNode(true, symbolTable, ident);
    } else if (currentToken.type == Token::CONST) {
        double value = std::stod(currentToken.value);
		//std::cout << "\t\t\t\t factor => const : " << value << "\n";
        //std::cout << " " << value;
        output_Line.append(" ");
        output_Line.append(std::to_string((int)value));
        eat(Token::CONST);
        return new FactorNode(true, symbolTable, value);
    } else if (currentToken.type == Token::ADD_OP) {
        std::cout << "(Warning)" << "Operator is duplicated." << std::endl;
        eat(Token::ADD_OP);
        return parseFactor();
    } else if (currentToken.type == Token::MUL_OP) {
        std::cout << "(Warning)" << "Operator is duplicated." << std::endl;
        eat(Token::MUL_OP);
        return parseFactor();
    }
    else if (currentToken.type == Token::RIGHT_PAREN) {
        std::cout << "(Warning)" << "checking if the parentheses are properly opened and closed" << std::endl;
        eat(Token::RIGHT_PAREN);
        return parseFactor();
    }
    return nullptr;
}

FactorTailNode* Parser::parseFactorTail() {
	//std::cout << "\t\t\t - parseFactorTail excute\n";
    if (currentToken.type == Token::MUL_OP) {
        int mul_op = (currentToken.value == "*") ? 1 : 2;
        if (mul_op == 1)//std::cout << "*";
            output_Line.append(" *");
        else //std::cout << "/";
            output_Line.append(" /");
        eat(Token::MUL_OP);
        FactorNode* factor = parseFactor();
        FactorTailNode* factorTail = parseFactorTail();
        return new FactorTailNode(true, symbolTable, mul_op, factor, factorTail);
    }
    else if (currentToken.type == Token::RIGHT_PAREN) {
        std::cout << "(Warning)" << "checking if the parentheses are properly opened and closed" << std::endl;
        eat(Token::RIGHT_PAREN);
        return parseFactorTail();
    }
        
    
	//std::cout << "\t\t\t\t factor_tail => lambda \n";
    return nullptr;
}

