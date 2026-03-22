#include <iostream>
#include <fstream>
#include <antlr4-runtime.h>
#include "stellaLexer.h"
#include "stellaParser.h"

int main(int argc, char* argv[]) {
    std::cout << "Stella Type Checker v1.0.0" << std::endl;
    
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        return 1;
    }
    
    try {
        std::ifstream stream;
        stream.open(argv[1]);
        
        antlr4::ANTLRInputStream input(stream);
        stella::stellaLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);
        stella::stellaParser parser(&tokens);
        
        stella::stellaParser::ProgramContext* tree = parser.program();
        
        std::cout << "Parsing completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}