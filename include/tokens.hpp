#ifndef TOKENS_HPP
#define TOKENS_HPP

struct Token {
   enum Type: char {
      empty,
      right, left, up, down, rightCondition, leftCondition, upCondition, downCondition, bridge,
      add, subtract, multiply, divide, modulo, power,
      logical_not, greaterThan,
      stringmode,
      duplicate, swap, pop, terminate, get, put,
      outputInteger, outputAscii,
      integerInput, asciiInput,
      number, ten
   };

   Type type = Type::empty;
   char value = 0;
};

#endif
