#ifndef TOKENS_HPP
#define TOKENS_HPP

struct Token {
   enum Type: char {
      empty,
      right, left, up, down, rightCondition, leftCondition, upCondition, downCondition, bridge,
      jump, jumpCondition, return_,
      add, subtract, multiply, divide, modulo, power, increment, decrement,
      logical_not, greaterThan, equals,
      stringmode, reverseStringMode,
      duplicate, swap, pop, terminate, get, put, getRegister, putRegister,
      outputInteger, outputAscii, outputString,
      integerInput, asciiInput, stringInput,
      number, ten, getStackSize, randomGenerator,
   };

   Type type = Type::empty;
   char value = 0;
};

#endif
