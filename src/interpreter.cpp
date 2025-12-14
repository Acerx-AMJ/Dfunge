#include "format.hpp" // IWYU pragma: export
#include "interpreter.hpp"

// Vector2

bool Vector2::operator==(const Vector2 &vector) const {
   return x == vector.x && y == vector.y;
}

size_t Vector2::operator()(const Vector2 &vector) const {
   return std::hash<int>()(vector.x) ^ (std::hash<int>()(vector.y) << 1);
}

// Constructor

Interpreter::Interpreter() {
   srand(time(nullptr));
   direction = {1, 0};
   initCommands();
   initFunctions();
}

// Lexer

void Interpreter::lex(const std::string &code) {
   Vector2 lexPosition;

   bool isLexingLabel = false;
   std::string label;

   for (size_t i = 0; i < code.size(); ++i) {
      char character = code[i];

      if (character == '\n') {
         lexPosition.y += 1;
         lexPosition.x = 0;
         continue;
      }

      if (character == ':') {
         isLexingLabel = true;
      } else if (isLexingLabel && (std::isalnum(character) || character == '_')) {
         label += character;
      } else {
         if (isLexingLabel) {
            labels[label] = lexPosition;
            label.clear();
            isLexingLabel = false;
         }
         map[lexPosition] = lexCommand(character);
      }
      lexPosition.x += 1;
   }
}

Token Interpreter::lexCommand(char character) {
   if (tokenTypes.contains(character)) {
      return {tokenTypes[character], character};
   } else if (std::isdigit(character)) {
      return {Token::number, character};
   } else {
      return {Token::invalid, character};
   }
}

// Interpreter

void Interpreter::run(const std::string &code) {
   lex(code);

   while (true) {
      runCommand(map[position]);
      forward();
   }
}

void Interpreter::runCommand(Token command) {
   // Handle identifier mode
   if (identifiermode && !std::isalnum(command.value) && command.value != '_') {
      if (gettingVariable) {
         assert(variables.contains(identifier), "Variable '{}' is not defined.", identifier);
         push(variables[identifier]);
      } else if (callingFunction) {
         assert(functions.contains(identifier), "Built-in function '{}' is not defined.", identifier);
         functions[identifier]();
      } else if (gettingLabelPos) {
         assert(labels.contains(identifier), "Label '{}' is not defined.", identifier);
         Vector2 &position = labels[identifier];
         push(position.x);
         push(position.y);
      } else {
         // Stack size checked in Token::define command
         int a = pop();
         variables[identifier] = a;
      }
      identifiermode = gettingVariable = callingFunction = false;
      identifier.clear();
   } else if (identifiermode) {
      identifier += command.value;
      return;
   }

   // Handle number mode
   if (numbermode && (hexadecimalNumber ? !isHexadecimal(command.value) : command.type != Token::number)) {
      if (command.value == 'X' && numberString.empty()) {
         hexadecimalNumber = true;
         return;
      }

      numbermode = false;
      if (!numberString.empty()) {
         try {
            push(std::stoi(numberString, nullptr, (hexadecimalNumber ? 16 : 10)));
            numberString.clear();
         } catch (...) {
            raise("''': Cannot convert string '{}' to number. Number is too large.", numberString);
         }
      }
   } else if (numbermode) {
      numberString += command.value;
      return;
   }

   // Handle string mode
   if (stringmode && command.type != Token::stringmode) {
      if (reverseString) {
         temporaryString.push_back(command.value);
      } else if (outputString) {
         std::cout << command.value;
      } else {
         push(command.value);
      }
      return;
   }

   // Handle defer mode
   if (defermode && command.type != Token::defer) {
      defered.push(command);
      return;
   }

   // Handle normal commands
   assert(commands.contains(command.type), "Unknown command: {} - '{}'.", (int)command.type, command.value);
   commands[command.type](command.value);
}

// Utility functions

void Interpreter::forward() {
   position.x += direction.x;
   position.y += direction.y;
}

void Interpreter::back() {
   position.x -= direction.x;
   position.y -= direction.y;
}

int Interpreter::pop() {
   if (stack.empty()) {
      return 0;
   }

   int value = stack.top();
   stack.pop();
   return value;
}

int Interpreter::top() {
   if (stack.empty()) {
      return 0;
   }
   return stack.top();
}

void Interpreter::push(int value) {
   stack.push(value);
}

void Interpreter::assertStackSize(size_t minimum, char operatorc) {
   assert(stack.size() >= minimum, "'{}': Expected stack size to be at least {}, but it is {} instead.", operatorc, minimum, stack.size());
}

void Interpreter::assertStackSize(size_t minimum, const std::string &string) {
   assert(stack.size() >= minimum, "Function '{}': Expected stack size to be at least {}, but it is {} instead.", string, minimum, stack.size());
}

bool Interpreter::isHexadecimal(char character) {
   character = std::tolower(character);
   return std::isdigit(character) || (character >= 'a' && character <= 'f');
}
