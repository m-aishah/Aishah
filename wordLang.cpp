#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>


// The Tokenizer

/**
 * An enumeration of the possible tokentypes.
*/
enum class TokenType
{
    DECLARE, 
    SET, 
    TO_BE, 
    TO, SHOW, 
    VALUE, 
    IF, 
    IS, 
    OTHERWISE, 
    PLUS, 
    MINUS, 
    TIMES, 
    DIVIDED_BY, 
    GREATER_THAN, 
    LESS_THAN, 
    PERIOD, 
    COLON, 
    IDENTIFIER, 
    NUMBER, 
    END
};

/**
 * struct Token - A data type that represents a token.
 * type: The type of the token (can only be one of Token type).
 * value: The value the token contains (string).
*/
struct Token 
{
    TokenType type;
    std::string value;
};

class Tokenizer {
public:
    Tokenizer(const std::string &input) : input(input), pos(0) 
    {
        initKeywordMap(); // Set up the keyword map.
    }

    /**
     * tokenize: Analyzes every letter in the input string and creates tokens accordingly.
     * 
     * Returns: A vector of the tokens created from the input string.
    */
    std::vector<Token> tokenize() 
    {
        // Create vector to store tokens. Vector of "Token"
        std::vector<Token> tokens;
        // Loop through input string and match characters to token type.
        // If match i sfound, add token to tokens vector.
        while (pos < input.size()) 
        {
            if (isspace(input[pos])) 
            {
                pos++; // Skip spaces.
            } 
            else if (isalpha(input[pos])) 
            {
                tokens.push_back(tokWord());
            } 
            else if (isdigit(input[pos])) 
            {
                tokens.push_back(tokNumber());
            } 
            else if (input[pos] == '.') 
            {
                tokens.push_back(Token{TokenType::PERIOD, "."});
                pos++;
            } 
            else if (input[pos] == ':') 
            {
                tokens.push_back(Token{TokenType::COLON, ":"});
                pos++;
            } 
            else 
            {
                pos++;
            }
        }
        // Add END to signify end of tokens vector.
        tokens.push_back(Token{TokenType::END, ""});
        return tokens;
    }

private:
    std::string input;
    size_t pos;
    std::unordered_map<std::string, TokenType> keywordMap; // Language Keywords

    /**
     * Set up the `keywordMap` variable. Include every possible keywords (including multi-worded keywords).
    */
    void initKeywordMap() 
    {
        keywordMap["Declare"] = TokenType::DECLARE;
        keywordMap["Set"] = TokenType::SET;
        keywordMap["to be"] = TokenType::TO_BE;
        keywordMap["to"] = TokenType::TO;
        keywordMap["Show"] = TokenType::SHOW;
        keywordMap["the value of"] = TokenType::VALUE;
        keywordMap["If"] = TokenType::IF;
        keywordMap["is"] = TokenType::IS;
        keywordMap["Otherwise"] = TokenType::OTHERWISE;
        keywordMap["plus"] = TokenType::PLUS;
        keywordMap["minus"] = TokenType::MINUS;
        keywordMap["times"] = TokenType::TIMES;
        keywordMap["divided by"] = TokenType::DIVIDED_BY;
        keywordMap[">"] = TokenType::GREATER_THAN;
        keywordMap["less than"] = TokenType::LESS_THAN;
    }

    /**
     * tokWord: Handles alphabets/words. 
     * Creates tokens appropriately, 
     * by assigning the right token type to the right words and skipping extra words. 
     * Evaluates whether they are keywords, identifiers, or operands.
     * Then moves `pos` to the right position.
     * 
     * Returns: A "Token".
    */
    Token tokWord() 
    {
        size_t start = pos;
        while (pos < input.size() && isalpha(input[pos])) pos++;
        std::string word = input.substr(start, pos - start);

        // Check for multi-word keywords
        if (word == "to" && pos + 2 < input.size() && input.substr(pos, 3) == " be") 
        {
            pos += 3;
            return Token{TokenType::TO_BE, "to be"};
        } 
        else if (word == "the" && pos + 8 < input.size() && input.substr(pos, 9) == " value of") 
        {
            pos += 9;
            return Token{TokenType::VALUE, "the value of"};
        } 
        else if (word == "greater" && pos + 4 < input.size() && input.substr(pos, 5) == " than") 
        {
            pos += 5;
            return Token{TokenType::GREATER_THAN, ">"};
        } 
        else if (word == "less" && pos + 4 < input.size() && input.substr(pos, 5) == " than") 
        {
            pos += 5;
            return Token{TokenType::LESS_THAN, "less than"};
        }

        if (keywordMap.find(word) != keywordMap.end()) 
        {
            return Token{keywordMap[word], word};
        } 
        else 
        {
            return Token{TokenType::IDENTIFIER, word};
        }
    }

    /**
     * tokNumber: Evaluates numbers. and moves the `pos` to the next index after the number.
     * 
     * Returns: complete string of digits.
    */
    Token tokNumber() 
    {
        size_t start = pos;
        while (pos < input.size() && isdigit(input[pos])) pos++;
        return Token{TokenType::NUMBER, input.substr(start, pos - start)};
    }
};



// The Parser

// Possible types of AST (Abstract Syntax Tree) Nodes.
enum class ASTNodeType 
{
    Program, 
    Declaration, 
    Assignment, 
    Print, 
    If, 
    Block, 
    BinaryOp, 
    Number, 
    Identifier
};

/**
 * struct ASTNode - Represent a node in the AST.
 * type: The type of the value in the Node.
 * value: The value in the node.
 * children: Vector of shared pointers to the children of the node. 
*/
struct ASTNode 
{
    ASTNodeType type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode(ASTNodeType type, std::string value) : type(type), value(std::move(value)) {}
};

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), pos(0) {}

    /**
     * parse - Umbrella function. Parses the program.
     * 
     * Returns: A shared pointer to the root node of the AST - after parsing.
    */
    std::shared_ptr<ASTNode> parse() 
    {
        return parseProgram();
    }

private:

    std::vector<Token> tokens;
    size_t pos;

    /**
     * parseProgram - Creates the root node of the AST tree,
     * and connects its children to shared pointers of every expression/statement in the program.
     * 
     * Returns: A shared pointer to the root node of the AST (Program).
    */
    std::shared_ptr<ASTNode> parseProgram() 
    {
        auto rootNode = std::make_shared<ASTNode>(ASTNodeType::Program, "");
        // While not at the `END` parse Statement.
        while (currentToken().type != TokenType::END) 
        {
            rootNode->children.push_back(parseNextStatement());
        }
        return rootNode;
    }

    /**
     * parseNextStatement - Parses next statment by evaluating next related tokens.
     * 
     * Returns: Node containing parsed statement. 
    */
    std::shared_ptr<ASTNode> parseNextStatement() 
    {
        // std::cout << "Parsing statement: " << currentToken().value << std::endl;
        if (currentToken().type == TokenType::DECLARE) 
        {
            return parseDeclaration();
        } 
        else if (currentToken().type == TokenType::SET) 
        {
            return parseAssignment();
        } 
        else if (currentToken().type == TokenType::SHOW) 
        {
            return parsePrint();
        } 
        else if (currentToken().type == TokenType::IF) 
        {
            return parseIfStatement();
        } 
        else 
        {
            throw std::runtime_error("Unexpected token in statement: " + currentToken().value);
        }
    }

    // Handle declarations.
    std::shared_ptr<ASTNode> parseDeclaration() 
    {
        // std::cout << "Parsing declaration" << std::endl;
        auto node = std::make_shared<ASTNode>(ASTNodeType::Declaration, "");
        tokToken(); // skip 'Declare'
        node->children.push_back(std::make_shared<ASTNode>(ASTNodeType::Identifier, tokToken().value));
        tokToken(); // skip 'to be'
        node->children.push_back(parseExpression());
        tokToken(); // skip '.'
        return node;
    }

    // Handle assignments.
    std::shared_ptr<ASTNode> parseAssignment() 
    {
        // std::cout << "Parsing assignment" << std::endl;
        auto node = std::make_shared<ASTNode>(ASTNodeType::Assignment, "");
        tokToken(); // skip 'Set'
        node->children.push_back(std::make_shared<ASTNode>(ASTNodeType::Identifier, tokToken().value));
        tokToken(); // skip 'to'
        node->children.push_back(parseExpression());
        tokToken(); // skip '.'
        return node;
    }

    // Handle print statements
    std::shared_ptr<ASTNode> parsePrint() 
    {
        // std::cout << "Parsing print" << std::endl;
        auto node = std::make_shared<ASTNode>(ASTNodeType::Print, "");
        tokToken(); // skip 'Show'
        tokToken(); // skip 'the value of'
        node->children.push_back(parseExpression());
        tokToken(); // skip '.'
        return node;
    }

    // Handle conditional blocks if-otherwise.
    std::shared_ptr<ASTNode> parseIfStatement() 
    {
        // std::cout << "Parsing if statement" << std::endl;
        auto node = std::make_shared<ASTNode>(ASTNodeType::If, "");
        tokToken(); // skip 'If'
        node->children.push_back(parseExpression());
        tokToken(); // skip 'is'
        node->children.push_back(parseComparison());
        node->children.push_back(parseExpression());
        tokToken(); // skip ':'
        node->children.push_back(parseBlock());
        if (currentToken().type == TokenType::OTHERWISE) 
        {
            tokToken(); // skip 'Otherwise'
            tokToken(); // skip ':'
            node->children.push_back(parseBlock());
        }
        return node;
    }

    // Handle Code blocks
    std::shared_ptr<ASTNode> parseBlock() 
    {
        // std::cout << "Parsing block" << std::endl;
        auto node = std::make_shared<ASTNode>(ASTNodeType::Block, "");
        while (currentToken().type != TokenType::PERIOD && currentToken().type != TokenType::END) 
        {
            node->children.push_back(parseNextStatement());
        }
        tokToken(); // skip '.'
        return node;
    }

    // Handle expressions (in if statements? Only? Aishah Check!)
    // Need better implementation to handle Order of operator evaluation.
    std::shared_ptr<ASTNode> parseExpression() 
    {
        auto node = parseTerm();  // Handle LHS of expression, may or not include * & /
        while (currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS) 
        {
            auto opNode = std::make_shared<ASTNode>(ASTNodeType::BinaryOp, tokToken().value);
            opNode->children.push_back(node);
            opNode->children.push_back(parseTerm());
            node = opNode;
        }
        return node;
    }

    std::shared_ptr<ASTNode> parseTerm() 
    {
        auto node = parseFactor(); // Handle LHS of expression, either a number or a variable/identifier.
        while (currentToken().type == TokenType::TIMES || currentToken().type == TokenType::DIVIDED_BY) 
        {
            auto opNode = std::make_shared<ASTNode>(ASTNodeType::BinaryOp, tokToken().value);
            opNode->children.push_back(node);
            opNode->children.push_back(parseFactor());
            node = opNode;
        }
        return node;
    }

    // Handle factors. either a number or a variable/identifier.
    std::shared_ptr<ASTNode> parseFactor() 
    {
        if (currentToken().type == TokenType::NUMBER) 
        {
            return std::make_shared<ASTNode>(ASTNodeType::Number, tokToken().value);
        } 
        else if (currentToken().type == TokenType::IDENTIFIER) 
        {
            return std::make_shared<ASTNode>(ASTNodeType::Identifier, tokToken().value);
        } 
        else 
        {
            throw std::runtime_error("Unexpected token in factor");
        }
    }

    // Handle comparisons
    std::shared_ptr<ASTNode> parseComparison() 
    {
        // Do greater than, less than, equal to, not euqal to.
        if (currentToken().type == TokenType::GREATER_THAN) 
        {
            return std::make_shared<ASTNode>(ASTNodeType::BinaryOp, tokToken().value);
        } 
        else if (currentToken().type == TokenType::LESS_THAN) 
        {
            return std::make_shared<ASTNode>(ASTNodeType::BinaryOp, tokToken().value);
        } 
        else 
        {
            throw std::runtime_error("Unexpected token in comparison: " + currentToken().value);
        }
    }

    Token currentToken() 
    {
        return tokens[pos];
    }

    Token nextToken() 
    {
        return tokens[pos + 1];
    }

    Token tokToken() 
    {
        return tokens[pos++];
    }
};


// The interpreter
class Interpreter {
public:
    /**
     * Traverses through the AST and evaluates every node.
     * Performs necessary operations.
    */
    void interpret(const std::shared_ptr<ASTNode>& node) 
    {
        // Interpret every child in a Program/Block subtree.
        if (node->type == ASTNodeType::Program || node->type == ASTNodeType::Block) 
        {
            for (const auto& child : node->children) 
            {
                interpret(child);
            }
        } 
        else if (node->type == ASTNodeType::Declaration) 
        {
            std::string varName = node->children[0]->value;
            int value = evaluate(node->children[1]);
            variables[varName] = value;
        } 
        else if (node->type == ASTNodeType::Assignment) 
        {
            std::string varName = node->children[0]->value;
            int value = evaluate(node->children[1]);
            variables[varName] = value;
        } 
        else if (node->type == ASTNodeType::Print) 
        {
            int value = evaluate(node->children[0]);
            std::cout << value << std::endl;
        } 
        else if (node->type == ASTNodeType::If) 
        {
            int left = evaluate(node->children[0]);
            int right = evaluate(node->children[2]);
            std::string op = node->children[1]->value;
            bool condition = (op == ">") ? (left > right) : (left < right);
            if (condition) 
            {
                interpret(node->children[3]);
            } 
            else if (node->children.size() == 5) 
            {
                interpret(node->children[4]);
            }
        } 
        else 
        {
            throw std::runtime_error("Unexpected node type");
        }
    }

    /**
     * Print the AST (or debugging purpose)
    */
    void printAST(const std::shared_ptr<ASTNode>& node, int indent = 0) 
    {
        if (!node) return;
        std::string indentation(indent, ' ');
        std::cout << indentation << "|--Node Type: " << nodeTypeToString(node->type) << ", Value: " << node->value << std::endl;
        for (const auto& child : node->children) 
        {
            printAST(child, indent + 2);
        }
    }

private:
    std::unordered_map<std::string, int> variables;

    /**
     * Chnage Node Type to a string for print function (or any future need?)
    */
    std::string nodeTypeToString(ASTNodeType type) 
    {
        switch (type) 
        {
            case ASTNodeType::Program: return "Program";
            case ASTNodeType::Declaration: return "Declaration";
            case ASTNodeType::Assignment: return "Assignment";
            case ASTNodeType::Print: return "Print";
            case ASTNodeType::If: return "If";
            case ASTNodeType::Block: return "Block";
            case ASTNodeType::BinaryOp: return "BinaryOp";
            case ASTNodeType::Number: return "Number";
            case ASTNodeType::Identifier: return "Identifier";
            default: return "Unknown";
        }
    }

    // Evaluate any expression that needs any evaluation. Change number strings to integers
    int evaluate(const std::shared_ptr<ASTNode>& node) 
    {
        if (node->type == ASTNodeType::Number) 
        {
            return std::stoi(node->value);
        } 
        else if (node->type == ASTNodeType::Identifier) 
        {
            return variables.at(node->value);
        } 
        else if (node->type == ASTNodeType::BinaryOp) 
        {
            int left = evaluate(node->children[0]);
            int right = evaluate(node->children[1]);
            if (node->value == "plus") return left + right;
            if (node->value == "minus") return left - right;
            if (node->value == "times") return left * right;
            if (node->value == "divided by") return left / right;
            throw std::runtime_error("Unexpected binary operator");
        }
        throw std::runtime_error("Unexpected node type");
    }
};

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