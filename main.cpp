#include <fstream>
#include <string>
#include "parser.h"

void asdf()
{
	system("leaks parser");
}

int main(int argc, char **argv)
{
    std::string input;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    std::getline(file, input, '\0');
    file.close();
    std::cout << input << std::endl;
    std::cout << std::endl;
//	std::string input a = "";

	SymbolTable symbolTable;
    Parser parser(input, symbolTable);
    ProgramNode* rootNode = parser.parseProgram();
    rootNode->calculate_statements();
	symbolTable.print_result();

	delete rootNode;
	//atexit(asdf);
    return (0);
}
