#include "interpreter.hpp"
#include <cmath>
#include <ctime>

void Interpreter::initFunctions() {
   // Math utility functions

   functions["abs"] = [this]() {
      assertStackSize(1, "abs");
      int a = pop();
      push((a < 0 ? -a : a));
   };
   functions["sign"] = [this]() {
      assertStackSize(1, "sign");
      int a = pop();
      push((a == 0 ? 0 : (a > 0 ? 1 : -1)));
   };
   functions["min"] = [this]() {
      assertStackSize(2, "min");
      int a = pop();
      int b = pop();
      push((a < b ? a : b));
   };
   functions["max"] = [this]() {
      assertStackSize(2, "max");
      int a = pop();
      int b = pop();
      push((a < b ? b : a));
   };
   functions["clamp"] = [this]() {
      assertStackSize(3, "clamp");
      int hi = pop();
      int lo = pop();
      int a = pop();
      push((a < lo ? lo : (a > hi ? hi : a)));
   };
   functions["sclamp"] = [this]() {
      assertStackSize(3, "sclamp");
      int hi = pop();
      int lo = pop();
      int a = pop();

      if (lo > hi) {
         std::swap(lo, hi);
      }
      push((a < lo ? lo : (a > hi ? hi : a)));
   };
   functions["mod"] = [this]() {
      assertStackSize(2, "mod");
      int mod = pop();
      int value = pop();
      int negativeCount = 0;

      if (mod < 0) {
         negativeCount += 1;
         mod = -mod;
      }
      if (value < 0) {
         negativeCount += 1;
         value = -value;
      }
      push((negativeCount == 1 ? -1 : 1) * (value % mod));
   };
   
   // Math functions

   functions["pow"] = [this]() {
      assertStackSize(2, "pow");
      int power = pop();
      int base = pop();
      push(std::pow(base, power));
   };

   // Random functions

   functions["rand"] = [this]() {
      push(rand());
   };
   functions["randint"] = [this]() {
      assertStackSize(1, "randint");
      int max = pop();
      int min = pop();
      int result = min + (rand() % (max - min + 1));
      push(result);
   };
   functions["randcond"] = [this]() {
      push(rand() % 2);
   };
   functions["srand"] = [this]() {
      assertStackSize(1, "srand");
      int seed = pop();
      srand(seed);
   };
   functions["srandt"] = []() {
      srand(time(nullptr));
   };
}
