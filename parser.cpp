#include "parser.h"

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

Parser::Parser(Lexer &lexer, Token currentToken) : lexer(lexer), currentToken(currentToken) {}

void Parser::parse() {
	currentToken = lexer.getNextToken();
	while (currentToken.type != Token::END && currentToken.type != Token::INVALID) {
		assignment();
	}
}

SymbolTable Parser::getSymbolTable() const {
	return symbolTable;
}

void Parser::assignment() {
	std::string varName = currentToken.value;
	consume(Token::IDENT);
	consume(Token::ASSIGN_OP);
	double value = expression();

	if (!error) {
		symbolTable.set(varName, value);
		std::cout << varName << ": " << value << std::endl;
	} else {
		std::cout << "(Error) \"" << varName << "\" 사용 전에 선언되어야 합니다." << std::endl;
	}

	consume(Token::SEMI_COLON);
}

double Parser::expression() {
	double result = term();
	while (currentToken.type == Token::ADD_OP) {
		if (currentToken.value == "+") {
			consume(Token::ADD_OP);
			result += term();
		} else if (currentToken.value == "-") {
			consume(Token::ADD_OP);
			result -= term();
		}
	}
	return result;
}

double Parser::term() {
	double result = factor();
	while (currentToken.type == Token::MUL_OP) {
		if (currentToken.value == "*") {
			consume(Token::MUL_OP);
			result *= factor();
		} else if (currentToken.value == "/") {
			consume(Token::MUL_OP);
			double denom = factor();
			if (denom != 0) {
				result /= denom;
			} else {
				std::cerr << "Zero division error!" << std::endl;
				error = true;
			}
		}
	}
	return result;
}

double Parser::factor() {
	double result = 0;
	if (currentToken.type == Token::CONST) {
		result = std::stod(currentToken.value);
		consume(Token::CONST);
	} else if (currentToken.type == Token::IDENT) {
		if (symbolTable.get(currentToken.value, result)) {
			consume(Token::IDENT);
		} else {
			error = true;
		}
	} else if (currentToken.type == Token::LEFT_PAREN) {
		consume(Token::LEFT_PAREN);
		result = expression();
		consume(Token::RIGHT_PAREN);
	}
	return result;
}

void Parser::consume(Token::Type type) {
	if (currentToken.type == type) {
		currentToken = lexer.getNextToken();
	} else {
		std::cerr << "Unexpected token: " << currentToken.value << std::endl;
		error = true;
	}
}
