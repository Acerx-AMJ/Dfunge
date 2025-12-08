# Dfunge
Dfunge is a successor to Cfunge, which is a 2D esoteric programming language based off Befunge-93.

## Language Overview
Dfunge is laid out on a two-dimensional playground, just like Befunge-93. The playfield is practically infinite in size (-2147483648 to 2147483647 on both axes). The program counter starts at (0, 0) and is pointed to the right. Just like in Befunge-93, the program counter has inertia, meaning that it will continue moving in a specific direction until it is changed, but it does not wrap around when it reaches a limit. When the program counter lands on a command, it gets executed. Dfunge uses two data structures: the stack, where all operators are used, and the registers (with the same size as the playfield), which can only store numeric values and characters.

## Instructions
Command names are case-sensitive. If stack size is less than the expected stack size while calling a command, the command will throw an error. If stack is empty, then any popped value will be 0 (in commands that use a value from the stack, but don't require the stack size, e.g j, k, l, ;, ?).

|Command|Description|Expected Stack Size|
|-|-|-|
|>|Set PC direction to right|0|
|<|Set PC direction to left|0|
|^|Set PC direction to up|0|
|v|Set PC direction to down|0|
|l|Pop from stack, if it's nonzero, set PC direction to right|0|
|h|Pop from stack, if it's nonzero, set PC direction to left|0|
|k|Pop from stack, if it's nonzero, set PC direction to up|0|
|j|Pop from stack, if it's nonzero, set PC direction to down|0|
|;|Advance a tile forward|0|
|J|Pop Y and then X and jump to (X, Y)|2|
|L|Pop Y, X and then VALUE and jump to (X, Y) if VALUE is nonzero|3|
|R|Return to the last jump call or at the beginning of the program if all jump calls have been returned to, uses the direction used before jumping|0|
|+|Pop two values, add them together and push result to stack|2|
|-|Pop A, then B, subtract A from B and push result to stack|2|
|*|Pop two values, multiply them together and push result to stack|2|
|/|Pop A, then B, perform integer division on B by A and push result to stack|2|
|%|Pop A, then B, perform integer division on B by A and push the remainder to stack|2|
|P|Pop A, then B, exponentiate B to the power of A and push the result to stack|2|
|i|Pop a value, increment it by one and push it back to stack|1|
|d|Pop a value, decrement it by one and push it back to stack|1|
|n|Pop a value, negate it and push it back to stack|1|
|!|Pop a value, if it is zero, push 1, else 0|1|
|G|Pop A, then B, if B is bigger than A, push 1, else 0|2|
|=|Pop two values, if they're equal, push 1, else 0|2|
|"|Toggle string mode, if string mode is active push all commands as characters to stack|0|
|r|Toggle reverse string mode, if reverse string mode is active, push characters to stack in reverse order|0|
|:|Duplicate top stack value|1|
| \\ |Swap the top two stack values|2|
|q|Pop the top stack value|0|
|E|Terminate the program|0|
|g|Pop a value from stack and push back value from register at the popped value|1|
|p|Pop A, then B and put B into the register at index A|2|
|.|Pop a value and output it as an integer|1|
|,|Pop a value and output it as an ASCII|1|
|o|Toggle string ouput mode. When string mode is enabled, output all characters instead of pushing them to stack|0|
|`|Get integer input from the user|0|
|~|Get a single character input from the user|0|
|&|Get a string input from the user|0|
|0-9|Push the corresponding digit to stack|0|
|t|Push 10 to stack|0|
|'|Toggle number mode. When number mode is enabled, it pushes the next whole number to the stack, number mode is disabled when a non-digit character is parsed. If X is right after ', then number will be parsed as a hexadecimal|0|
|s|Push the stack size to stack|0|
|?|Pop MAX, then MIN and generate a random integer between MIN and MAX including. By default MIN is 0|1|
|#|Enter identifiermode, create a new variable and assign a popped value|1|
|@|Enter identifiermode, get the value from the given variable|0| 
