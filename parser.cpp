#include <iostream>
#include <string>
#include <unordered_map>

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

class SymbolTable {
public:
    void set(const std::string& name, double value) {
        symbols[name] = value;
    }

    bool get(const std::string& name, double& value) const {
        auto it = symbols.find(name);
        if (it != symbols.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    bool exists(const std::string& name) const {
        return symbols.find(name) != symbols.end();
    }

private:
    std::unordered_map<std::string, double> symbols;
};

class Parser {
public:
    Parser(Lexer &lexer, Token currentToken) : lexer(lexer), currentToken(currentToken) {}

    void parse() {
        currentToken = lexer.getNextToken();
        while (currentToken.type != Token::END && currentToken.type != Token::INVALID) {
            assignment();
        }
    }

    SymbolTable getSymbolTable() const {
        return symbolTable;
    }

private:
    void assignment() {
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

    double expression() {
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

    double term() {
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

    double factor() {
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

    void consume(Token::Type type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        } else {
            std::cerr << "Unexpected token: " << currentToken.value << std::endl;
            error = true;
        }
    }

    Lexer& lexer;
    Token currentToken;
    SymbolTable symbolTable;
    bool error = false;
};

//int main() {
//    std::string code =
//            "operand2 := 2;\n"
//            "target := operand2 * 3;";
//    Lexer lexer(code);
//    Parser parser(lexer, Token(Token::END, ""));
//    parser.parse();
//    return (0);
//}
