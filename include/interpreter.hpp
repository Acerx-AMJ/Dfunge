#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "tokens.hpp"
#include <functional>
#include <stack>
#include <string>
#include <unordered_map>

// Vector2

struct Vector2 {
   int x = 0;
   int y = 0;

   // Operators
   bool operator==(const Vector2 &vector) const;

   // Hash
   size_t operator()(const Vector2 &vector) const;
};

// Interpreter

struct Interpreter {
   static std::unordered_map<char, Token::Type> tokenTypes;
   std::unordered_map<Token::Type, std::function<void(char)>> commands;
   std::unordered_map<std::string, std::function<void()>> functions;

   std::unordered_map<Vector2, Token, Vector2> map;
   std::unordered_map<int, int> registers;
   std::unordered_map<std::string, int> variables;
   std::stack<Vector2> jumps;
   std::stack<Token> defered;
   std::stack<int> stack;

   Vector2 position, direction;
   bool stringmode = false, outputString = false, reverseString = false;
   bool numbermode = false, hexadecimalNumber = false;
   bool identifiermode = false, gettingVariable = false, callingFunction = false;
   bool defermode = false;
   std::string temporaryString, numberString, identifier;

   // Init commands

   Interpreter();
   void initCommands();
   void initFunctions();

   // Lexer

   void lex(const std::string &code);
   Token lexCommand(char character);

   // Interpreter

   void run(const std::string &code);
   void runCommand(Token command);

   // Utility functions

   void forward();
   void back();
   int pop();
   int top();
   void push(int value);

   void assertStackSize(size_t minimum, char operatorc);
   void assertStackSize(size_t minimum, const std::string &function);
   bool isHexadecimal(char character);
};

#endif
