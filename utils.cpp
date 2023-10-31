#include "utils.h"

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



