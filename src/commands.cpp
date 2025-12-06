#include "format.hpp" // IWYU pragma: export
#include "interpreter.hpp"
#include <limits>

#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#endif

// Globals

std::unordered_map<char, Token::Type> Interpreter::tokenTypes {
   {' ', Token::empty}, {'+', Token::add}, {'-', Token::subtract}, {'*', Token::multiply}, {'/', Token::divide},
   {'%', Token::modulo}, {'!', Token::logical_not}, {'`', Token::greaterThan}, {'>', Token::right}, {'<', Token::left},
   {'^', Token::up}, {'v', Token::down}, {'?', Token::randomDirection}, {'_', Token::horizontalIf}, {'|', Token::verticalIf},
   {'"', Token::stringmode}, {':', Token::duplicate}, {'\\', Token::swap}, {'$', Token::pop}, {'.', Token::outputInteger},
   {',', Token::outputAscii}, {'#', Token::bridge}, {'g', Token::get}, {'p', Token::put}, {'&', Token::integerInput},
   {'~', Token::asciiInput}, {'@', Token::terminate}
};

// Init commands

Interpreter::Interpreter() {
   srand(time(nullptr));
   direction = {1, 0};

   commands[Token::empty] = [](char) {};

   // Arithmetic commands

   commands[Token::add] = [this](char value) {
      assertStackSize(2, value);
      push(pop() + pop());
   };
   commands[Token::subtract] = [this](char value) {
      assertStackSize(2, value);
      int a = pop();
      int b = pop();
      push(b - a);
   };
   commands[Token::multiply] = [this](char value) {
      assertStackSize(2, value);
      push(pop() * pop());
   };
   commands[Token::divide] = [this](char value) {
      assertStackSize(2, value);
      int a = pop();
      int b = pop();

      assert(a != 0, "'{}': Attempted to divide '{}' by zero.", value, b);
      push(b / a);
   };
   commands[Token::modulo] = [this](char value) {
      assertStackSize(2, value);
      int a = pop();
      int b = pop();

      assert(a != 0, "'{}': Attempted to divide '{}' by zero.", value, b);
      push(b % a);
   };

   // Logical commands

   commands[Token::logical_not] = [this](char value) {
      assertStackSize(1, value);
      push(!pop());
   };
   commands[Token::greaterThan] = [this](char value) {
      assertStackSize(2, value);
      push(pop() > pop());
   };

   // Movement commands

   commands[Token::right] = [this](char) {
      direction = {1, 0};
   };
   commands[Token::left] = [this](char) {
      direction = {-1, 0};
   };
   commands[Token::up] = [this](char) {
      direction = {0, -1};
   };
   commands[Token::down] = [this](char) {
      direction = {0, 1};
   };
   commands[Token::randomDirection] = [this](char) {
      int value = rand() % 4;

      if (value == 0) {
         direction = {1, 0};
      } else if (value == 1) {
         direction = {-1, 0};
      } else if (value == 2) {
         direction = {0, -1};
      } else {
         direction = {0, 1};
      }
   };
   commands[Token::horizontalIf] = [this](char) {
      if (pop() == 0) {
         direction = {1, 0};
      } else {
         direction = {-1, 0};
      }
   };
   commands[Token::verticalIf] = [this](char) {
      if (pop() == 0) {
         direction = {0, 1};
      } else {
         direction = {0, -1};
      }
   };

   // String commands

   commands[Token::stringmode] = [this](char) {
      stringmode = !stringmode;
   };

   // Stack commands

   commands[Token::duplicate] = [this](char) {
      push(top());
   };
   commands[Token::swap] = [this](char) {
      int a = pop();
      int b = pop();
      push(a);
      push(b);
   };
   commands[Token::pop] = [this](char) {
      pop();
   };

   // Output commands

   commands[Token::outputInteger] = [this](char value) {
      assertStackSize(1, value);
      std::cout << pop(); 
   };
   commands[Token::outputAscii] = [this](char value) {
      assertStackSize(1, value);
      std::cout << static_cast<char>(pop());
   };

   // Misc. commands

   commands[Token::bridge] = [this](char) {
      forward();   
   };
   commands[Token::get] = [this](char value) {
      assertStackSize(2, value);
      int y = pop();
      int x = pop();
      if (map.contains({x, y})) {
         push(map[{x, y}].value);
      } else {
         push(0);
      }
   };
   commands[Token::put] = [this](char value) {
      assertStackSize(3, value);
      int y = pop();
      int x = pop();
      int v = pop();
      map[{x, y}] = {Token::empty, (char)v}; // Won't work as intended, purely teporary
   };
   commands[Token::terminate] = [](char) {
      std::exit(0);
   };

   // Input commands

   commands[Token::integerInput] = [this](char) {
      int num = 0;
      std::cin >> num;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      push(num);
   };
   commands[Token::asciiInput] = [this](char) {
      #ifdef __linux__
      termios oldt {}, newt {};
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);

      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      char ch = getchar();
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      #else
      char ch = getchar();
      #endif
      push(ch);
   };
   commands[Token::number] = [this](char value) {
      push(value - '0');
   };
}
