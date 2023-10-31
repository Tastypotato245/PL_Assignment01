#include "parser.h"
#include "treenode.h"

ProgramNode* Parser::parseProgram() {
	std::cout << " - parseProgram excute\n";
    return new ProgramNode(parseStatements());
}

StatementsNode* Parser::parseStatements() {
	std::cout << "\t - parseStatements excute\n";
    StatementNode* statement = parseStatement();
    if (currentToken.type != Token::END) {
    	eat(Token::SEMI_COLON);
        return new StatementsNode(statement, isParsed, parseStatements());
    }
    return new StatementsNode(statement, isParsed, nullptr);
}

StatementNode* Parser::parseStatement() {
    std::string ident = currentToken.value;
    eat(Token::IDENT);
    eat(Token::ASSIGN_OP);
	std::cout << "\t - parseStatement excute : ident : " << ident << "\n";
    ExpressionNode* expression = parseExpression();
    return new StatementNode(isParsed, symbolTable, ident, isParsed, expression);
}

ExpressionNode* Parser::parseExpression() {
	std::cout << "\t\t - parseExpression excute\n";
    TermNode* term = parseTerm();
    TermTailNode* termTail = parseTermTail();
    return new ExpressionNode(isParsed, symbolTable, term, termTail);
}

TermNode* Parser::parseTerm() {
	std::cout << "\t\t\t - parseTerm excute\n";
    FactorNode* factor = parseFactor();
    FactorTailNode* factorTail = parseFactorTail();
    return new TermNode(true, symbolTable, factor, factorTail);
}

TermTailNode* Parser::parseTermTail() {
	std::cout << "\t\t\t - parseTermTail excute\n";
    if (currentToken.type == Token::ADD_OP) {
		std::cout << "\t\t\t\t term_tail => add_op <term> <term_tail>\n";
        int add_op = (currentToken.value == "+") ? 1 : 2;
        eat(Token::ADD_OP);
        TermNode* term = parseTerm();
        TermTailNode* termTail = parseTermTail();
        return new TermTailNode(isParsed, symbolTable, add_op, term, termTail);
    }
	std::cout << "\t\t\t\t term_tail => lambda\n";
    return nullptr; 
}

FactorNode* Parser::parseFactor() {
	std::cout << "\t\t\t - parseFactor excute\n";
    if (currentToken.type == Token::LEFT_PAREN) {
		std::cout << "\t\t\t\t factor => ( ex )\n";
        eat(Token::LEFT_PAREN);
        ExpressionNode* expression = parseExpression();
        eat(Token::RIGHT_PAREN);
        return new FactorNode(isParsed, symbolTable, isParsed, expression, isParsed);
    } else if (currentToken.type == Token::IDENT) {
		std::cout << "\t\t\t\t factor => ident\n";
        std::string ident = currentToken.value;
        eat(Token::IDENT);
        return new FactorNode(true, symbolTable, ident);
    } else if (currentToken.type == Token::CONST) {
        double value = std::stod(currentToken.value);
		std::cout << "\t\t\t\t factor => const : " << value << "\n";
        eat(Token::CONST);
        return new FactorNode(true, symbolTable, value);
    }
    return nullptr;
}

FactorTailNode* Parser::parseFactorTail() {
	std::cout << "\t\t\t - parseFactorTail excute\n";
    if (currentToken.type == Token::MUL_OP) {
        int mul_op = (currentToken.value == "*") ? 1 : 2;
        eat(Token::MUL_OP);
        FactorNode* factor = parseFactor();
        FactorTailNode* factorTail = parseFactorTail();
        return new FactorTailNode(true, symbolTable, mul_op, factor, factorTail);
    }
	std::cout << "\t\t\t\t factor_tail => lambda \n";
    return nullptr;
}

