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
	//std::cout << "\t - parseStatements excute\n";

	StatementNode* statement = parseStatement();
	if (currentToken.next_token != Token::END) {
		//std::cout << ";" << std::endl
		output_Line.append(";");
		eat(Token::SEMI_COLON); 
		if (currentToken.next_token == Token::END) {
			output_Line.erase(std::find(output_Line.begin(), output_Line.end(), ';'));
			errorMessage.append("(Warning)");
			errorMessage.append("Invalid Location of SEMICOLON.");
			errorMessage.append("\n");
			std::cout << output_Line << std::endl;
			if (errorMessage == "\0")
				std::cout << "(OK)\n";
			else
				std::cout << errorMessage;
			std::cout << "ID: " << chk_ID << "; CONST:" << chk_CONST << "; OP: " << chk_OP << std::endl;
			return new StatementsNode(statement, isParsed, nullptr);
		}
		std::cout << output_Line << std::endl;
		if (errorMessage == "\0")
			std::cout << "(OK)\n";
		else
			std::cout << errorMessage;
		std::cout << "ID: " << chk_ID << "; CONST:" << chk_CONST << "; OP: " << chk_OP << std::endl;
		output_Line = "\0";
		errorMessage = "\0";
		return new StatementsNode(statement, isParsed, parseStatements());
	}
	std::cout << output_Line << std::endl;
	if (errorMessage == "\0")
		std::cout << "(OK)\n";
	else
		std::cout << errorMessage;
	std::cout << "ID: " << chk_ID << "; CONST:" << chk_CONST << "; OP: " << chk_OP << std::endl;
	return new StatementsNode(statement, isParsed, nullptr);
}

StatementNode* Parser::parseStatement() {
	chk_ID = 0;
	chk_CONST = 0;
	chk_OP = 0;
	chk_LParen = 0;
	std::string ident = currentToken.token_string;
	//std::cout << ident << " :=";
	output_Line.append(ident);
	output_Line.append(" :=");
	isdefine.push_back(ident);
	eat(Token::IDENT);
	eat(Token::ASSIGN_OP);
	if (currentToken.next_token == Token::SEMI_COLON || currentToken.next_token == Token::END) { 
		errorMessage.append("(Error)");
		errorMessage.append("Empty RHS");
		errorMessage.append("\n");
		output_Line.append(" ");
		return new StatementNode(false, symbolTable, ident, false, nullptr);
	}
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
	if (currentToken.next_token == Token::ADD_OP) {
		//std::cout << "\t\t\t\t term_tail => add_op <term> <term_tail>\n";
		int add_op = (currentToken.token_string == "+") ? 1 : 2;
		if(add_op==1)//std::cout << " +";
		output_Line.append(" +");
		else //std::cout << " -";
		output_Line.append(" -");
		eat(Token::ADD_OP);
		TermNode* term = parseTerm();
		TermTailNode* termTail = parseTermTail();
		return new TermTailNode(isParsed, symbolTable, add_op, term, termTail);
	}
	else if (currentToken.next_token == Token::RIGHT_PAREN) {
		if (chk_LParen == 0) {
			errorMessage.append("(Warning)");
			errorMessage.append("checking if the parentheses are properly opened and closed");
			errorMessage.append("\n");
			eat(Token::RIGHT_PAREN);
			return parseTermTail();
		}
	} 
	else if (currentToken.next_token == Token::IDENT) {
		errorMessage.append("(Error)");
		errorMessage.append("Invalid location of Token.");
		errorMessage.append("\n");
		output_Line.append(' '+currentToken.token_string);
		eat(Token::IDENT);
		return new TermTailNode(false, symbolTable);
	}
	else if(currentToken.next_token == Token::CONST) {
		errorMessage.append("(Error)");
		errorMessage.append("Invalid location of Token.");
		errorMessage.append("\n");
		output_Line.append(' ' + currentToken.token_string);
		eat(Token::CONST);
		return new TermTailNode(false, symbolTable);
	}

	//std::cout << "\t\t\t\t term_tail => lambda\n";
	return nullptr; 
}

FactorNode* Parser::parseFactor() {
	//std::cout << "\t\t\t - parseFactor excute\n";
	if (currentToken.next_token == Token::LEFT_PAREN) {
		//std::cout << "\t\t\t\t factor => ( ex )\n";
		//std::cout << "(";
		chk_LParen++;
		output_Line.append(" (");
		eat(Token::LEFT_PAREN);
		ExpressionNode* expression = parseExpression();
		//std::cout << ")";
		if(currentToken.next_token==Token::RIGHT_PAREN) {
			chk_LParen--;
			output_Line.append(" )");
			eat(Token::RIGHT_PAREN);
			return new FactorNode(isParsed, symbolTable, isParsed, expression, isParsed);
		}
		else {
			errorMessage.append("(Warning)");
			errorMessage.append("checking if the parentheses are properly opened and closed");
			errorMessage.append("\n");
			//std::cout << "(Warning)" << "checking if the parentheses are properly opened and closed" << std::endl;
			output_Line.erase(std::find(output_Line.begin(), output_Line.end(), '('));
			return new FactorNode(isParsed, symbolTable, isParsed, expression, isParsed);
		}
	} else if (currentToken.next_token == Token::IDENT) {
		//std::cout << "\t\t\t\t factor => ident\n";
		std::string ident = currentToken.token_string;
		if (std::find(isdefine.begin(), isdefine.end(), ident) == isdefine.end()) {
			errorMessage.append("(Error)");
			errorMessage.append("Cannot use undefined variable.");
			errorMessage.append("\n");
			output_Line.append(' '+ident);
			eat(Token::IDENT);
			return new FactorNode(false, symbolTable, ident);
		}
		//std::cout << " " <<ident;
		output_Line.append(" ");
		output_Line.append(ident);
		eat(Token::IDENT);
		return new FactorNode(true, symbolTable, ident);
	} else if (currentToken.next_token == Token::CONST) {
		double value = std::stod(currentToken.token_string);
		//std::cout << "\t\t\t\t factor => const : " << value << "\n";
		//std::cout << " " << value;
		output_Line.append(" ");
		output_Line.append(std::to_string((int)value));
		eat(Token::CONST);
		return new FactorNode(true, symbolTable, value);
	} else if (currentToken.next_token == Token::ADD_OP) {
		errorMessage.append("(Warning)");
		errorMessage.append("Operator is duplicated.");
		errorMessage.append("\n");
		chk_OP--;
		//std::cout << "(Warning)" << "Operator is duplicated." << std::endl;
		eat(Token::ADD_OP);
		return parseFactor();
	} else if (currentToken.next_token == Token::MUL_OP) {
		errorMessage.append("(Warning)");
		errorMessage.append("Operator is duplicated.");
		errorMessage.append("\n");
		chk_OP--;
		//std::cout << "(Warning)" << "Operator is duplicated." << std::endl;
		eat(Token::MUL_OP);
		return parseFactor();
	}
	else if (currentToken.next_token == Token::RIGHT_PAREN) {
		errorMessage.append("(Warning)");
		errorMessage.append("checking if the parentheses are properly opened and closed");
		errorMessage.append("\n");
		//std::cout << "(Warning)" << "checking if the parentheses are properly opened and closed" << std::endl;
		eat(Token::RIGHT_PAREN);
		return parseFactor();
	}
	return nullptr;
}

FactorTailNode* Parser::parseFactorTail() {
	//std::cout << "\t\t\t - parseFactorTail excute\n";
	if (currentToken.next_token == Token::MUL_OP) {
		int mul_op = (currentToken.token_string == "*") ? 1 : 2;
		if (mul_op == 1)//std::cout << "*";
		output_Line.append(" *");
		else //std::cout << "/";
		output_Line.append(" /");
		eat(Token::MUL_OP);
		FactorNode* factor = parseFactor();
		FactorTailNode* factorTail = parseFactorTail();
		return new FactorTailNode(true, symbolTable, mul_op, factor, factorTail);
	}
	else if (currentToken.next_token == Token::RIGHT_PAREN) {
		if (chk_LParen == 0) {
			errorMessage.append("(Warning)");
			errorMessage.append("checking if the parentheses are properly opened and closed");
			errorMessage.append("\n");
			eat(Token::RIGHT_PAREN);
			return parseFactorTail();
		}
	}
	else if (currentToken.next_token == Token::IDENT) {
		errorMessage.append("(Error)");
		errorMessage.append("Invalid location of Token.");
		errorMessage.append("\n");
		output_Line.append(' ' + currentToken.token_string);
		eat(Token::IDENT);
		return new FactorTailNode(false, symbolTable);
	}
	else if(currentToken.next_token == Token::CONST) {
		errorMessage.append("(Error)");
		errorMessage.append("Invalid location of Token.");
		errorMessage.append("\n");
		output_Line.append(' ' + currentToken.token_string);
		eat(Token::CONST);
		return new FactorTailNode(false, symbolTable);
	}
	//std::cout << "\t\t\t\t factor_tail => lambda \n";
	return nullptr;
}

