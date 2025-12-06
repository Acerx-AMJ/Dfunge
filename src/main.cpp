#include "file.hpp"
#include "format.hpp" // IWYU pragma: export

int main(int argc, char *argv[]) {
   assert(argc == 2, "Expected 2 arguments, got {} instead. See '-h' for more info.", argc);
   std::string input = argv[1];

   if (isFile(input)) {
      input = readFile(input);
   }

   warn("Input: '{}'", input);
   return 0;
}
