#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"

int main(int argc, char **argv)
{
    std::string input;
//    if (argc != 2) {
//        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
//        return 1;
//    }
//
//    std::ifstream file(argv[1]);
//    if (!file.is_open()) {
//        std::cerr << "Failed to open file: " << argv[1] << std::endl;
//        return 1;
//    }

//    std::getline(file, input, '\0');
//    file.close();
//    std::cout << input << std::endl;
	input = "a := 10; b := a; c := a + b;";

	Lexer lexer(input);
	Parser parser(lexer, Token(Token::END, ""));
	parser.parse();

    return (0);
}
