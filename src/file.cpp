#include "file.hpp"
#include "format.hpp" // IWYU pragma: export
#include <filesystem>
#include <fstream>

bool isFile(const std::string &file) {
   return std::filesystem::exists(file) && std::filesystem::is_regular_file(file);
}

std::string readFile(const std::string &input) {
   std::ifstream file (input);
   assert(file.is_open(), "Could not read file '{}'.", input);
   check(std::filesystem::path(input).extension() == ".dfng", "Extension of file '{}' should be '.dfng'.", input);

   std::string output, temporary;
   while (std::getline(file, temporary)) {
      output += temporary + '\n';
   }
   return output;
}
