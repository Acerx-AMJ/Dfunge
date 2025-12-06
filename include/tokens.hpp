#ifndef TOKENS_HPP
#define TOKENS_HPP

struct Token {
   enum Type: char {
      empty, add, subtract, multiply, divide, modulo, logical_not, greaterThan,
      right, left, up, down, randomDirection, horizontalIf, verticalIf, stringmode,
      duplicate, swap, pop, outputInteger, outputAscii, bridge, get, put, integerInput,
      asciiInput, terminate, number
   };

   Type type = Type::empty;
   char value = 0;
};

#endif
