#include "wordLang.cpp"

// Please Note that ideally, this program should be run with the file containing a program written in wordLang. 
// However, for testing purposes I commented out the "Read file" functionality,
// and just created a variable `input` and set it equal to a string which is a program written in wordLang.
int main(int argc, char* argv[]) 
{
    // if (argc != 2) 
    // {
       // std::cerr << "Usage: " << argv[0] << " <source file>\n";
       // return 1;
    //}

    // Open file
    // std::ifstream file(argv[1]);
    // if (!file.is_open()) 
    // {
       // std::cerr << "Error opening file: " << argv[1] << "\n";
       // return 1;
    // }

    // Read file content into buffer -> input
    // std::stringstream buffer;
    // buffer << file.rdbuf();
    // std::string input = buffer.str();
    std::string input = "Declare x to be 5. Set x to 10 plus 5. Show the value of x.";

    // Tokenize input into tokes
    Tokenizer tokenizer(input);
    auto tokens = tokenizer.tokenize();

    // for (const auto& token : tokens) 
    // {
        // std::cout << "Token: " << token.value << " Type: " << token.type << std::endl;
    // }

    // Parse tokens
    Parser parser(tokens);
    auto ast = parser.parse();

    Interpreter interpreter;
    std::cout << "Abstract Syntax Tree (AST):" << std::endl;
    interpreter.printAST(ast); // Print AST? 
    std::cout << "\nOutput:" << std::endl;
    interpreter.interpret(ast);

    return 0;
}