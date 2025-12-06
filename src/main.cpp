#include "file.hpp"
#include "format.hpp" // IWYU pragma: export
#include "interpreter.hpp"

int main(int argc, char *argv[]) {
   assert(argc == 2, "Expected 2 arguments, got {} instead. See '-h' for more info.", argc);
   std::string input = argv[1];

   if (isFile(input)) {
      input = readFile(input);
   }

   Interpreter interpreter;
   interpreter.run(input);
   return 0;
}
