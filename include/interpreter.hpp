#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "tokens.hpp"
#include <string>
#include <unordered_map>

// Vector2

struct Vector2 {
   int x = 0;
   int y = 0;

   // Hash
   size_t operator()(const Vector2 &vector) const;
};

// Interpreter

struct Interpreter {
   std::unordered_map<Vector2, Token, Vector2> map;

   // Lexer

   void lex(const std::string &code);
};

#endif
