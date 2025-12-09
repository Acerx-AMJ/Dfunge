#include "interpreter.hpp"

void Interpreter::initFunctions() {
   // Math functions

   functions["abs"] = [this]() {
      assertStackSize(1, "abs");
      int a = pop();
      push((a < 0 ? -a : a));
   };
}
