#include "interpreter.hpp"

// Globals

static inline std::unordered_map<char, Token::Type> tokenTypes {
   {' ', Token::empty}, {'+', Token::add}, {'-', Token::subtract}, {'*', Token::multiply}, {'/', Token::divide},
   {'%', Token::modulo}, {'!', Token::logical_not}, {'`', Token::greaterThan}, {'>', Token::right}, {'<', Token::left},
   {'^', Token::up}, {'v', Token::down}, {'?', Token::randomDirection}, {'_', Token::horizontalIf}, {'|', Token::verticalIf},
   {'"', Token::stringmode}, {':', Token::duplicate}, {'\\', Token::swap}, {'$', Token::pop}, {'.', Token::outputInteger},
   {',', Token::outputAscii}, {'#', Token::bridge}, {'g', Token::get}, {'p', Token::put}, {'&', Token::integerInput},
   {'~', Token::asciiInput}, {'@', Token::terminate}
};

// Vector2 hash

size_t Vector2::operator()(const Vector2 &vector) const {
   return std::hash<int>()(vector.x) ^ (std::hash<int>()(vector.y) << 1);
}

// Lexer

void Interpreter::lex(const std::string &code) {
   Vector2 lexPosition;

   for (size_t i = 0; i < code.size(); ++i) {
      char character = code[i];

      if (character == '\n') {
         lexPosition.y += 1;
         lexPosition.x = 0;
         continue;
      }

      if (tokenTypes.contains(character)) {
         map[lexPosition] = {tokenTypes[character], character};
      } else if (std::isalnum(character)) {
         map[lexPosition] = {Token::number, character};
      }
      lexPosition.x += 1;
   }
}
