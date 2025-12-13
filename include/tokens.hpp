#ifndef TOKENS_HPP
#define TOKENS_HPP

struct Token {
   enum Type: char {
      invalid, empty,
      right, left, up, down, rightCondition, leftCondition, upCondition, downCondition, bridge,
      jump, jumpCondition, return_,
      add, subtract, multiply, divide, increment, decrement, negate,
      logical_not, greaterThan, equals,
      stringmode, reverseStringMode,
      duplicate, swap, pop, terminate, getRegister, putRegister,
      outputInteger, outputAscii, outputString,
      integerInput, asciiInput, stringInput,
      defer, deferRun, deferRunOne, deferGet, deferPush, deferDuplicate, deferSwap, deferPop, deferSize,
      number, ten, numbermode, getStackSize,
      define, getVariable, callFunction
   };

   Type type = Type::empty;
   char value = 0;
};

constexpr const char *tokenTypeStrings[] {
   "Invalid", "Empty",
   "Right", "Left", "Up", "Down", "RightCondition", "LeftCondition", "UpCondition", "DownCondition", "Bridge",
   "Jump", "JumpCondition", "Return",
   "Add", "Subtract", "Multiply", "Divide", "Increment", "Decrement", "Negate",
   "LogicalNot", "GreaterThan", "Equals",
   "Stringmode", "ReverseStringmode",
   "Duplicate", "Swap", "Pop", "Terminate", "GetRegister", "PutRegister",
   "OutputInteger", "OutputASCII", "OutputString",
   "IntegerInput", "ASCIIInput", "StringInput",
   "Defer", "DeferRun", "DeferRunOne", "DeferGet", "DeferPush", "DeferDuplicate", "DeferSwap", "DeferPop", "DeferSize",
   "Number", "Ten", "Numbermode", "GetStackSize",
   "Define", "GetVariable", "CallFunction"
};

#endif
