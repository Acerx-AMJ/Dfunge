#include "format.hpp" // IWYU pragma: export
#include "interpreter.hpp"
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>

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

   // File I/O functions

   functions["readfile"] = [this]() {
      assertStackSize(1, "readfile");
      int charcount = pop();

      assertStackSize(charcount, "readfile");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }

      std::ifstream file (filename);
      assert(file.is_open(), "'readfile': Failed to open file '{}'.", filename);

      std::string temp, total;
      while (std::getline(file, temp)) {
         total += temp + '\n';
      }

      for (auto it = total.rbegin(); it != total.rend(); ++it) {
         push(*it);
      }
   };

   functions["writefile"] = [this]() {
      assertStackSize(1, "writefile");
      int charcount = pop();

      assertStackSize(charcount, "writefile");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }

      assertStackSize(1, "writefile");
      int writecount = pop();

      assertStackSize(writecount, "writefile");
      std::string write;

      for (int i = 0; i < writecount; ++i) {
         write += pop();
      }

      std::ofstream file (filename, std::ios::out);
      assert(file.is_open(), "'writefile': Failed to open file '{}'.", filename);

      file << write;
   };

   functions["appendfile"] = [this]() {
      assertStackSize(1, "appendfile");
      int charcount = pop();

      assertStackSize(charcount, "appendfile");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }

      assertStackSize(1, "appendfile");
      int writecount = pop();

      assertStackSize(writecount, "appendfile");
      std::string write;

      for (int i = 0; i < writecount; ++i) {
         write += pop();
      }

      std::ofstream file (filename, std::ios::out | std::ios::app);
      assert(file.is_open(), "'appendfile': Failed to open file '{}'.", filename);

      file << write;
   };

   functions["isfile"] = [this]() {
      assertStackSize(1, "isfile");
      int charcount = pop();

      assertStackSize(charcount, "isfile");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }

      push(std::filesystem::exists(filename) && std::filesystem::is_regular_file(filename));
   };

   functions["isdirectory"] = [this]() {
      assertStackSize(1, "isdirectory");
      int charcount = pop();

      assertStackSize(charcount, "isdirectory");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }

      push(std::filesystem::exists(filename) && std::filesystem::is_directory(filename));
   };

   functions["createdirectory"] = [this]() {
      assertStackSize(1, "createdirectory");
      int charcount = pop();

      assertStackSize(charcount, "createdirectory");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }

      assert(std::filesystem::create_directories(filename), "'createdirectory': Failed to create directory '{}'.", filename);
   };

   functions["createfile"] = [this]() {
      assertStackSize(1, "createfile");
      int charcount = pop();

      assertStackSize(charcount, "createfile");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }
      std::ofstream file (filename, std::ios::out);
      assert(file.is_open(), "'createfile': Failed to create file '{}'.", filename);
   };

   functions["iteratedirectory"] = [this]() {
      assertStackSize(1, "iteratedirectory");
      int charcount = pop();

      assertStackSize(charcount, "iteratedirectory");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }

      assert(std::filesystem::exists(filename) && std::filesystem::is_directory(filename), "'iteratedirectory': Cannot iterate directory '{}'.", filename);
      push(0); // EOF
      
      for (const auto &file: std::filesystem::directory_iterator(filename)) {
         std::string entryName = file.path().string();
         for (auto it = entryName.rbegin(); it != entryName.rend(); ++it) {
            push(*it);
         }
         push(entryName.size());
      }
   };

   functions["deletefile"] = [this]() {
      assertStackSize(1, "deletefile");
      int charcount = pop();

      assertStackSize(charcount, "deletefile");
      std::string filename;

      for (int i = 0; i < charcount; ++i) {
         filename += pop();
      }
      assert(std::filesystem::exists(filename), "'deletefile': File '{}' does not exist.", filename);
      assert(std::filesystem::remove_all(filename), "'deletefile': Could not delete file '{}'.", filename);
   };

   // Debug functions

   functions["logstack"] = [this]() {
      std::cout << "STACK (top to bottom):\n";
      std::cout << "SIZE: " << stack.size() << "\n";

      std::stack<int> stackCopy = stack;
      int counter = 1;

      while (!stack.empty()) {
         int popped = pop();
         printf("%5d: Num: %-10d ASCII: '%c'\n", counter, popped, popped);
         counter += 1;
      }

      stack = stackCopy;
      std::cout << "END OF STACK\n";
   };

   functions["logdefer"] = [this]() {
      std::cout << "DEFER STACK (top to bottom):\n";
      std::cout << "SIZE: " << defered.size() << "\n";

      std::stack<Token> deferedCopy = defered;
      int counter = 1;

      while (!defered.empty()) {
         Token popped = defered.top();
         defered.pop();

         printf("%5d: ASCII: '%c' Type: '%s'\n", counter, popped.value, tokenTypeStrings[popped.type]);
         counter += 1;
      }

      defered = deferedCopy;
      std::cout << "END OF DEFER STACK\n";
   };

   functions["logregs"] = [this]() {
      std::cout << "REGISTERS:\n";
      std::cout << "SIZE: " << registers.size() << '\n';
      
      for (auto &[index, value]: registers) {
         printf("%5d: %d\n", index, value);
      }
      std::cout << "END OF REGISTERS\n";
   };

   functions["logvars"] = [this]() {
      std::cout << "VARIABLES:\n";
      std::cout << "SIZE: " << variables.size() << '\n';
      int counter = 1;

      for (auto &[identifier, value]: variables) {
         printf("%5d: '%s': Num: %-10d ASCII: '%c'\n", counter, identifier.c_str(), value, value);
         counter += 1;
      }
      std::cout << "END OF VARIABLES\n";
   };

   functions["loglabels"] = [this]() {
      std::cout << "LABELS:\n";
      std::cout << "SIZE: " << labels.size() << '\n';
      int counter = 1;

      for (auto &[label, position]: labels) {
         printf("%5d: '%s': X: %d Y: %d\n", counter, label.c_str(), position.x, position.y);
         counter += 1;
      }
      std::cout << "END OF LABELS\n";
   };
}
