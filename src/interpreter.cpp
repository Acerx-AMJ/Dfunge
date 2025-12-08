#include "format.hpp" // IWYU pragma: export
#include "interpreter.hpp"

// Vector2

bool Vector2::operator==(const Vector2 &vector) const {
   return x == vector.x && y == vector.y;
}

size_t Vector2::operator()(const Vector2 &vector) const {
   return std::hash<int>()(vector.x) ^ (std::hash<int>()(vector.y) << 1);
}

// Lexer

void Interpreter::lex(const std::string &code) {
   Vector2 lexPosition;

   for (size_t i = 0; i < code.size(); ++i) {
      char character = code[i];

      if (character == '\n') {
         lexPosition.y += 1;
         lexPosition.x = 0;
         continue;
      }

      if (tokenTypes.contains(character)) {
         map[lexPosition] = {tokenTypes[character], character};
      } else if (std::isdigit(character)) {
         map[lexPosition] = {Token::number, character};
      } else {
         map[lexPosition] = {Token::invalid, character};
      }
      lexPosition.x += 1;
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
   if (stringmode && command.type != Token::stringmode) {
      if (reverseString) {
         temporaryString.push_back(command.value);
      } else if (outputString) {
         std::cout << command.value;
      } else {
         push(command.value);
      }
   } else {
      assert(commands.contains(command.type), "Unknown command: {} - '{}'.", (int)command.type, command.value);
      commands[command.type](command.value);
   }
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
