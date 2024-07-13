![wordLang Logo](./Logo.png)

# wordLang 🎉

*Programming Language: Readable Code, Human Syntax.*

wordLang is a custom programming language I created for my Principles of Programming Languages course at the European University of Lefke. The goal? To make code look like natural language so it feels like you're reading a story! 📚✨

## Table of Contents
- [Introduction](#introduction)
- [Language Design](#language-design)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Parser Design](#parser-design)
- [Implementation Details](#implementation-details)
- [Planned Future Updates](#planned-future-updates)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Introduction

Welcome to wordLang! This project is all about creating a parser for a custom language using C++. Think of it as writing a novel with code! 📖💻 The syntax is designed to be as close to natural language as possible, making your programs read like regular text.

## Language Design

### Language Syntax (BNF Notation)

Here's a peek at the syntax of wordLang, following the Backus-Naur Form (BNF) notation:

```bnf
<program> ::= <statement>* <end>
<statement> ::= <declaration> | <print> | <if_statement> | <assignment>
<declaration> ::= "Declare" <identifier> "to be" <expression> "."
<assignment> ::= "Set" <identifier> "to" <expression> "."
<print> ::= "Show the value of" <expression> "."
<if_statement> ::= "If" <expression> "is" <comparison> <expression> ":" <block> ["Otherwise:" <block>]
<block> ::= <statement>*
<expression> ::= <term> ("plus" <term> | "minus" <term>)*
<term> ::= <factor> ("times" <factor> | "divided by" <factor>)*
<factor> ::= <number> | <identifier>
<comparison> ::= "greater than" | "less than"
<identifier> ::= <word>
<number> ::= <digit>+
<word> ::= <letter>+
<letter> ::= "a" | "b" | ... | "z"
<digit> ::= "0" | "1" | ... | "9"
<end> ::= ""
```

## Features 🌟

- **Variable Declarations**: `Declare` variables in style!
- **Assignments**: `Set` values with ease.
- **Print Statements**: `Show` off your results.
- **Conditionals**: Use `If` and `Otherwise` to control the flow.
- **Arithmetic Operations**: Do math naturally with `plus`, `minus`, `times`, and `divided by`.

## Installation 🚀

Getting started is super easy! Just follow these steps:

1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/wordLang.git
    cd wordLang
    ```

2. **Compile the code**:
    ```bash
    g++ -o wordLang main.cpp
    ```

## Usage 🛠️

To run your wordLang program, use this command:

```bash
./wordLang sourcefile.txt
```

Replace `sourcefile.txt` with the path to your wordLang code.

### Example

Here's a quick example of wordLang in action:
```
Declare x to be 5. Set x to 10. If x is greater than 5: Show the value of x. Otherwise: Show the value of 0.
```

## Parser Design 🧠

wordLang uses a Recursive Descent Parser to turn your text into an Abstract Syntax Tree (AST). It's like magic! 🪄

## Implementation Details 🔍

### Main Program Flow

1. **Input Handling**: Check if a source file is provided.
2. **File Reading**: Load the source file into a string.
3. **Tokenization**: Break down the string into tokens with `Tokenizer`.
4. **Parsing**: Turn tokens into an AST with `Parser`.
5. **AST Printing**: (Optional) Print the AST for debugging.
6. **Interpretation**: Execute the AST with `Interpreter`.

### Classes and Methods

- **Tokenizer**: Breaks down input into tokens.
- **Parser**: Converts tokens into an AST.
- **Interpreter**: Evaluates the AST and runs your program.

## Planned Future Updates 🔮

- Add unit tests for easier debugging.
- Implement more features like loops and functions.

## Contributing 💡

Got a cool idea? Found a bug? Contributions are welcome! Here's how you can help:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -m 'Add some feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a pull request.

## License 📜

This project is licensed under the MIT License.

## Contact 📧

For questions or support, reach out to Aishah Ayomide Mabayoje at [maishah2540@gmail.com](mailto:maishah2540@gmail.com).