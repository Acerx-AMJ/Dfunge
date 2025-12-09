#include "format.hpp" // IWYU pragma: export
#include "interpreter.hpp"
#include <cmath>
#include <limits>

#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#endif

// Globals

// Unused characters
//                             (  )  _
//    wW e        yY uU     O    [{ }]  |
// aA       fF     H     K
// zZ    cC  V bB    m

std::unordered_map<char, Token::Type> Interpreter::tokenTypes {
   {' ', Token::empty},
   {'>', Token::right}, {'<', Token::left}, {'^', Token::up}, {'v', Token::down}, {'l', Token::rightCondition}, {'h', Token::leftCondition}, {'k', Token::upCondition}, {'j', Token::downCondition}, {';', Token::bridge},
   {'J', Token::jump}, {'L', Token::jumpCondition}, {'R', Token::return_},
   {'+', Token::add}, {'-', Token::subtract}, {'*', Token::multiply}, {'/', Token::divide}, {'M', Token::modulo}, {'P', Token::power}, {'i', Token::increment}, {'d', Token::decrement}, {'n', Token::negate},
   {'!', Token::logical_not}, {'G', Token::greaterThan}, {'=', Token::equals},
   {'"', Token::stringmode}, {'r', Token::reverseStringMode},
   {':', Token::duplicate}, {'\\', Token::swap}, {'q', Token::pop}, {'E', Token::terminate}, {'g', Token::getRegister}, {'p', Token::putRegister},
   {'.', Token::outputInteger}, {',', Token::outputAscii}, {'o', Token::outputString},
   {'`', Token::integerInput}, {'~', Token::asciiInput}, {'&', Token::stringInput},
   {'$', Token::defer}, {'X', Token::deferRun}, {'x', Token::deferRunOne}, {'T', Token::deferGet}, {'N', Token::deferPush}, {'D', Token::deferDuplicate}, {'I', Token::deferSwap}, {'Q', Token::deferPop}, {'S', Token::deferSize},
   {'t', Token::ten}, {'\'', Token::numbermode}, {'s', Token::getStackSize}, {'?', Token::randomGenerator},
   {'#', Token::define}, {'@', Token::getVariable}, {'%', Token::callFunction},
};

// Init commands

void Interpreter::initCommands() {
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
   commands[Token::negate] = [this](char value) {
      assertStackSize(1, value);
      push(-pop());
   };

   // Logical commands

   commands[Token::logical_not] = [this](char value) {
      assertStackSize(1, value);
      push(!pop());
   };
   commands[Token::greaterThan] = [this](char value) {
      assertStackSize(2, value);
      push(pop() < pop());
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

   commands[Token::duplicate] = [this](char value) {
      assertStackSize(1, value);
      push(top());
   };
   commands[Token::swap] = [this](char value) {
      assertStackSize(2, value);
      int a = pop();
      int b = pop();
      push(a);
      push(b);
   };
   commands[Token::pop] = [this](char) {
      pop();
   };
   commands[Token::terminate] = [](char) {
      std::cout << std::flush;
      std::exit(0);
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
      termios oldt, newt;
      tcgetattr(STDIN_FILENO, &oldt);
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

   // Defer commands

   commands[Token::defer] = [this](char) {
      defermode = !defermode;
   };
   commands[Token::deferRun] = [this](char) {
      while (!defered.empty()) {
         Token command = defered.top();
         defered.pop();
         runCommand(command);
      }
   };
   commands[Token::deferRunOne] = [this](char) {
      if (!defered.empty()) {
         Token command = defered.top();
         defered.pop();
         runCommand(command);
      }
   };
   commands[Token::deferGet] = [this](char value) {
      assertStackSize(1, value);
      char character = pop();
      Token token = lexCommand(character);
      defered.push(token);
   };
   commands[Token::deferPush] = [this](char) {
      char command = 0;
      if (!defered.empty()) {
         command = defered.top().value;
         defered.pop();
      }
      push(command);
   };
   commands[Token::deferDuplicate] = [this](char) {
      Token command;
      if (!defered.empty()) {
         command = defered.top();
      }
      defered.push(command);
   };
   commands[Token::deferSwap] = [this](char) {
      Token first;
      if (!defered.empty()) {
         first = defered.top();
         defered.pop();
      }

      Token second;
      if (!defered.empty()) {
         second = defered.top();
         defered.pop();
      }
      defered.push(first);
      defered.push(second);
   };
   commands[Token::deferPop] = [this](char) {
      if (!defered.empty()) {
         defered.pop();
      }
   };
   commands[Token::deferSize] = [this](char) {
      push(defered.size());
   };

   // Literal commands

   commands[Token::number] = [this](char value) {
      push(value - '0');
   };
   commands[Token::ten] = [this](char) {
      push(10);
   };
   commands[Token::numbermode] = [this](char) {
      numbermode = !numbermode;
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

   // Variable functions

   commands[Token::define] = [this](char value) {
      assertStackSize(1, value);
      identifiermode = true;
      gettingVariable = callingFunction = false;
   };
   commands[Token::getVariable] = [this](char) {
      identifiermode = gettingVariable = true;
      callingFunction = false;
   };
   commands[Token::callFunction] = [this](char) {
      identifiermode = callingFunction = true;
      gettingVariable = false;
   };
}
