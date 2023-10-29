#include <iostream>
#include <string>

class Token {
public:
    enum Type {
        IDENT,      // Identifier
        CONST,      // Constant
        ASSIGN_OP,  // :=
        ADD_OP,     // + or -
        MUL_OP,     // * or /
        SEMI_COLON, // ;
        LEFT_PAREN, // (
        RIGHT_PAREN,// )
        END,        // Input의 끝
        INVALID     // 잘못된 입력
    };

    Token(Type type, std::string value) : type(type), value(value) {}

    Type type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& input)
            : input(input), position(0) {}

    Token getNextToken() {
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

private:
    const std::string input;
    size_t position;
};
/*
int main() {
    std::string code = "a := 3 + 5;";
    Lexer lexer(code);
    Token token = lexer.getNextToken();
    while (token.type != Token::END) {
        std::cout << "Type: " << token.type << ", Value: " << token.value << std::endl;
        token = lexer.getNextToken();
    }

    return 0;
}
 */