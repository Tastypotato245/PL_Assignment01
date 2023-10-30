#include "parser.h"
#include "treenode.h"

Token::Token(Type type, std::string value) : type(type), value(value) {}

Lexer::Lexer(const std::string& input) : input(input), position(0) {}

Token Lexer::getNextToken() {
	while (position < input.size() && std::isspace(input[position])) {
		++position;
	}

	if (position >= input.size()) {
		return Token(Token::END, "");
	}

	char currentChar = input[position];

	if (std::isdigit(currentChar)) {
		std::string num;
		while (position < input.size() && std::isdigit(input[position])) {
			num += input[position++];
		}
		return Token(Token::CONST, num);
	}

	if (std::isalpha(currentChar)) {
		std::string ident;
		while (position < input.size() && (std::isalnum(input[position]) || input[position] == '_')) {
			ident += input[position++];
		}
		return Token(Token::IDENT, ident);
	}

	if (currentChar == '+' || currentChar == '-') {
		++position;
		return Token(Token::ADD_OP, std::string(1, currentChar));
	}

	if (currentChar == '*' || currentChar == '/') {
		++position;
		return Token(Token::MUL_OP, std::string(1, currentChar));
	}

	if (currentChar == ';') {
		++position;
		return Token(Token::SEMI_COLON, ";");
	}

	if (currentChar == '(') {
		++position;
		return Token(Token::LEFT_PAREN, "(");
	}

	if (currentChar == ')') {
		++position;
		return Token(Token::RIGHT_PAREN, ")");
	}

	if (currentChar == ':' && (position + 1 < input.size()) && input[position + 1] == '=') {
		position += 2;
		return Token(Token::ASSIGN_OP, ":=");
	}

	return Token(Token::INVALID, std::string(1, currentChar));
}

void SymbolTable::set(const std::string& name, double value) {
	symbols[name] = value;
}

bool SymbolTable::get(const std::string& name, double& value) const {
	auto it = symbols.find(name);
	if (it != symbols.end()) {
		value = it->second;
		return true;
	}
	return false;
}

bool SymbolTable::exists(const std::string& name) const {
	return symbols.find(name) != symbols.end();
}

/***********************PARSER*********************/

ProgramNode* Parser::parseProgram() {
    return new ProgramNode(*parseStatements());
}

StatementsNode* Parser::parseStatements() {
    StatementNode* statement = parseStatement();
    eat(Token::SEMI_COLON);
    if (currentToken.type != Token::END) {
        return new StatementsNode(*statement, true, *parseStatements());
    }
    return nullptr; // or handle it as per your requirement
}

StatementNode* Parser::parseStatement() {
    std::string ident = currentToken.value;
    eat(Token::IDENT);
    eat(Token::ASSIGN_OP);
    ExpressionNode* expression = parseExpression();
    return new StatementNode(isParsed, symbolTable, ident, isParsed, *expression);
}

ExpressionNode* Parser::parseExpression() {
    TermNode* term = parseTerm();
    TermTailNode* termTail = parseTermTail();
    return new ExpressionNode(true, symbolTable, *term, *termTail);
}

TermTailNode* Parser::parseTermTail() {
    if (currentToken.type == Token::ADD_OP) {
        int add_op = (currentToken.value == "+") ? 1 : 2;
        eat(Token::ADD_OP);
        TermNode* term = parseTerm();
        TermTailNode* termTail = parseTermTail();
        return new TermTailNode(true, symbolTable, add_op, term, termTail);
    }
    return nullptr; // or handle it as per your requirement
}

TermNode* Parser::parseTerm() {
    FactorNode* factor = parseFactor();
    FactorTailNode* factorTail = parseFactorTail();
    return new TermNode(true, symbolTable, *factor, *factorTail);
}

FactorTailNode* Parser::parseFactorTail() {
    if (currentToken.type == Token::MUL_OP) {
        int mul_op = (currentToken.value == "*") ? 1 : 2;
        eat(Token::MUL_OP);
        FactorNode* factor = parseFactor();
        FactorTailNode* factorTail = parseFactorTail();
        return new FactorTailNode(true, symbolTable, mul_op, factor, factorTail);
    }
    return nullptr; // or handle it as per your requirement
}

FactorNode* Parser::parseFactor() {
    if (currentToken.type == Token::LEFT_PAREN) {
        eat(Token::LEFT_PAREN);
        ExpressionNode* expression = parseExpression();
        eat(Token::RIGHT_PAREN);
        return new FactorNode(isParsed, symbolTable, isParsed, expression, isParsed);
    } else if (currentToken.type == Token::IDENT) {
        std::string ident = currentToken.value;
        eat(Token::IDENT);
        return new FactorNode(true, symbolTable, ident);
    } else if (currentToken.type == Token::CONST) {
        double value = std::stod(currentToken.value);
        eat(Token::CONST);
        return new FactorNode(true, symbolTable, value);
    }
    return nullptr; // or handle it as per your requirement
}
