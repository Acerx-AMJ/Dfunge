#include "format.hpp" // IWYU pragma: export
#include "interpreter.hpp"
#include <cmath>
#include <limits>

#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#endif

// Globals

std::unordered_map<char, Token::Type> Interpreter::tokenTypes {
   {' ', Token::empty},
   {'>', Token::right}, {'<', Token::left}, {'^', Token::up}, {'v', Token::down}, {';', Token::rightCondition}, {'j', Token::leftCondition}, {'l', Token::upCondition}, {'k', Token::downCondition}, {'#', Token::bridge},
   {'J', Token::jump}, {'L', Token::jumpCondition}, {'R', Token::return_},
   {'+', Token::add}, {'-', Token::subtract}, {'*', Token::multiply}, {'/', Token::divide}, {'%', Token::modulo}, {'u', Token::power}, {'i', Token::increment}, {'d', Token::decrement},
   {'!', Token::logical_not}, {'`', Token::greaterThan}, {'=', Token::equals},
   {'"', Token::stringmode}, {'r', Token::reverseStringMode},
   {':', Token::duplicate}, {'\\', Token::swap}, {'$', Token::pop}, {'@', Token::terminate}, {'g', Token::get}, {'p', Token::put}, {'G', Token::getRegister}, {'P', Token::putRegister},
   {'.', Token::outputInteger}, {',', Token::outputAscii}, {'o', Token::outputString},
   {'&', Token::integerInput}, {'~', Token::asciiInput}, {'q', Token::stringInput},
   {'t', Token::ten}, {'s', Token::getStackSize}, {'?', Token::randomGenerator}
};

// Init commands

Interpreter::Interpreter() {
   srand(time(nullptr));
   direction = {1, 0};

   // Empty

   commands[Token::empty] = [](char) {};

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
   commands[Token::rightCondition] = [this](char) {
      if (pop()) {
         direction = {1, 0};
      }
   };
   commands[Token::leftCondition] = [this](char) {
      if (pop()) {
         direction = {-1, 0};
      }
   };
   commands[Token::upCondition] = [this](char) {
      if (pop()) {
         direction = {0, -1};
      }
   };
   commands[Token::downCondition] = [this](char) {
      if (pop()) {
         direction = {0, 1};
      }
   };
   commands[Token::bridge] = [this](char) {
      forward();
   };

   // Jump commands

   commands[Token::jump] = [this](char value) {
      assertStackSize(2, value);
      int y = pop();
      int x = pop();

      jumps.push(direction);
      jumps.push(position);

      position = {x, y};
      back();
   };
   commands[Token::jumpCondition] = [this](char value) {
      assertStackSize(3, value);
      int y = pop();
      int x = pop();
      int condition = pop();

      if (condition) {
         jumps.push(direction);
         jumps.push(position);

         position = {x, y};
         back();
      }
   };
   commands[Token::return_] = [this](char) {
      if (jumps.empty()) {
         position = {-1, 0};
         direction = {1, 0};
      } else {
         position = jumps.top();
         jumps.pop();
         direction = jumps.top();
         jumps.pop();
      }
   };

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
   commands[Token::power] = [this](char value) {
      assertStackSize(2, value);
      int a = pop();
      int b = pop();
      push(std::pow(b, a));
   };
   commands[Token::increment] = [this](char value) {
      assertStackSize(1, value);
      push(pop() + 1);
   };
   commands[Token::decrement] = [this](char value) {
      assertStackSize(1, value);
      push(pop() - 1);
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
   commands[Token::equals] = [this](char value) {
      assertStackSize(2, value);
      push(pop() == pop());
   };

   // String commands

   commands[Token::stringmode] = [this](char) {
      if (stringmode) {
         for (auto it = temporaryString.rbegin(); it != temporaryString.rend(); ++it) {
            if (outputString) {
               std::cout << *it;
            } else {
               push(*it);
            }
         }
         temporaryString.clear();
         outputString = reverseString = false;
      }
      stringmode = !stringmode;
   };
   commands[Token::reverseStringMode] = [this](char) {
      reverseString = !reverseString;
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
   commands[Token::terminate] = [](char) {
      std::exit(0);
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
      map[{x, y}] = {tokenTypes[(char)v], (char)v};
   };
   commands[Token::getRegister] = [this](char value) {
      assertStackSize(1, value);
      push(registers[pop()]);
   };
   commands[Token::putRegister] = [this](char value) {
      assertStackSize(2, value);
      int r = pop();
      int v = pop();
      registers[r] = v;
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
   commands[Token::outputString] = [this](char) {
      outputString = !outputString;
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
   commands[Token::stringInput] = [this](char) {
      std::string input;
      std::getline(std::cin, input);
      for (auto it = input.rbegin(); it != input.rend(); ++it) {
         push(*it);
      }
   };

   // Literal commands

   commands[Token::number] = [this](char value) {
      push(value - '0');
   };
   commands[Token::ten] = [this](char) {
      push(10);
   };
   commands[Token::getStackSize] = [this](char) {
      push(stack.size());
   };
   commands[Token::randomGenerator] = [this](char value) {
      assertStackSize(1, value);
      int max = pop();
      int min = pop();
      int result = min + (rand() % (max - min + 1));
      push(result);
   };
}
