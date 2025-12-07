#ifndef TOKENS_HPP
#define TOKENS_HPP
// std::unordered_map<char, Type> Cfunge::tokens {
//    {'J', Type::jump}, {'L', Type::jump_cond}, {'R', Type::return_},
//    {'=', Type::equals},
//    {'?', Type::random_int}, {'s', Type::get_stack},
//    {'g', Type::get}, {'p', Type::put}, {'@', Type::define},s
//    {'$', Type::defer}, {'x', Type::defered_run}, {'X', Type::defered_run_one}, {'O', Type::defered_push}, {'P', Type::defered_duplicate}, {'I', Type::defered_swap}, {'Q', Type::defered_pop}, {'S', Type::defered_size},
// };
struct Token {
   enum Type: char {
      empty,
      right, left, up, down, rightCondition, leftCondition, upCondition, downCondition, bridge,
      add, subtract, multiply, divide, modulo, power, increment, decrement,
      logical_not, greaterThan, equals,
      stringmode, reverseStringMode,
      duplicate, swap, pop, terminate, get, put,
      outputInteger, outputAscii, outputString,
      integerInput, asciiInput, stringInput,
      number, ten, getStackSize,
   };

   Type type = Type::empty;
   char value = 0;
};

#endif
