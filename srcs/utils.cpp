#include "utils.h"

Token::Token(Type next_token, std::string token_string) : next_token(next_token), token_string(token_string) {}

Lexer::Lexer(const std::string& input) : input(input), position(0) {}

Token Lexer::lexical() {
	while (position < input.size() && std::isspace(input[position])) {
		++position;
	}
	while (position < input.size() && input[position]<=32) {
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
	auto itr = symbols.find(name);
	if (itr != symbols.end())return true;
	else return false;
	//return symbols.find(name) != symbols.end();
}

void SymbolTable::print_result() const{
	std::cout << "Result ==>";
	for(const auto& pair : symbols) {
        std::cout << " " << pair.first << ": ";
		if (std::isnan(pair.second))
			std::cout << "Unknown";
		else
			std::cout << pair.second;
		std::cout << ";";
    }
}
