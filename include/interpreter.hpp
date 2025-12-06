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

   std::unordered_map<Vector2, Token, Vector2> map;
   std::stack<int> stack;

   Vector2 position, direction;
   bool stringmode = false;

   // Init commands

   Interpreter();

   // Lexer

   void lex(const std::string &code);

   // Interpreter

   void run(const std::string &code);
   void runCommand(Token command);

   // Utility functions

   void forward();
   int pop();
   int top();
   void push(int value);
   void assertStackSize(size_t minimum, char operatorc);
};

#endif
