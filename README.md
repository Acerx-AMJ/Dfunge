# Dfunge
Dfunge is a successor to Cfunge, which is a 2D esoteric programming language based off Befunge-93. There are various examples available in the examples folder. Some might be outdated, though.

## Language Overview
Dfunge is laid out on a two-dimensional playground, just like Befunge-93. The playfield is practically infinite in size (-2147483648 to 2147483647 on both axes). The program counter starts at (0, 0) and is pointed to the right. Just like in Befunge-93, the program counter has inertia, meaning that it will continue moving in a specific direction until it is changed, but it does not wrap around when it reaches a limit. When the program counter lands on a command, it gets executed.

Dfunge uses four different data structures: the stack, the defered stack, the variable map and the registers. The stack is used for operating on and storing values. It stores 32-bit integers, which also count as characters, so all three Dfunge data types can be stored: integers, characters and strings. The defered stack stores commands, which can be operated on and executed using the defer commands. Variable map stores all of the variables, which can store integers and characters, for non-string types this is the preferred way of storing values. The register map stores integers in a specific index, it is used for storing arrays and strings. 

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
|\||Advance a tile forward|0|
|R|Return to the last jump call or at the beginning of the program if all jump calls have been returned to, uses the direction used before jumping|0|
|+|Pop two values, add them together and push result to stack|2|
|-|Pop A, then B, subtract A from B and push result to stack|2|
|*|Pop two values, multiply them together and push result to stack|2|
|/|Pop A, then B, perform integer division on B by A and push result to stack|2|
|M|Pop A, then B, perform integer division on B by A and push the remainder to stack|2|
|P|Pop A, then B, exponentiate B to the power of A and push the result to stack|2|
|i|Pop a value, increment it by one and push it back to stack|1|
|d|Pop a value, decrement it by one and push it back to stack|1|
|n|Pop a value, negate it and push it back to stack|1|
|!|Pop a value, if it is zero, push 1, else 0|1|
|G|Pop A, then B, if B is bigger than A, push 1, else 0|2|
|=|Pop two values, if they're equal, push 1, else 0|2|
|"|Toggle string mode, if string mode is active push all commands as characters to stack|0|
|r|Toggle reverse string mode, if reverse string mode is active, push characters to stack in reverse order|0|
|H|Duplicate top stack value|1|
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
|$|Toggle defer mode, when defer mode is on, push all commands to defer stack instead of executing them|0|
|X|Run all defered commands|0|
|x|Run a single defered command|0|
|T|Pop a value from the stack and push it to the defered stack as a command|1|
|N|Pop a value from the defered stack and push the character value to the stack|0|
|D|Duplicate the top defered command|0|
|I|Swap the top two defered commands|0|
|Q|Pop the top defered command|0|
|S|Push the size of the defered stack to the stack|0|
|0-9|Push the corresponding digit to stack|0|
|t|Push 10 to stack|0|
|'|Toggle number mode. When number mode is enabled, it pushes the next whole number to the stack, number mode is disabled when a non-digit character is parsed. If X is right after ', then number will be parsed as a hexadecimal|0|
|s|Push the stack size to stack|0|
|#|Enter identifiermode, create a new variable and assign a popped value. Variables can include all alpha-numerical characters including underscores|1|
|@|Enter identifiermode, get the value from the given variable|0| 
|%|Enter identifiermode, call the function with the given identifier|0|
|:|Define a label. Labels can only be defined from left to right since they get defined during lexing phase|0|
|;|Enter identifiermode and jump to the position of the label to the stack|0|

## Functions
Functions are esentially commands that use more characters. They can be called using the % operator.

### Utility Functions
|Function|Description|Expected stack size|
|-|-|-|
|abs|Get the absolute value of a popped value|1|
|sign|Pop a value and return the sign of it|1|
|min|Pop two values and return the smallest of them|2|
|max|Pop two values and return the largest of them|2|
|clamp|Pop HI, LO and V and clamp V in the range [LO, HI]|3|
|sclamp|Same as clamp, but changes HI with LO if LO is bigger than HI|3|
|wait|Pop a value and pause execution for that many milliseconds|1|

### Math Functions
|Function|Description|Expected stack size|
|-|-|-|
|mod|Pop A and B and perform modulus|2|
|pow|Pop POWER, then BASE and raise BASE to the power of POWER|2|

### Random Functions
|Function|Description|Expected stack size|
|-|-|-|
|rand|Push a random integer to the stack|0|
|randint|Pop MAX, then MIN and generate a random integer between MIN and MAX including. By default MIN is 0|1|
|randcond|Push either 0 or 1|0|
|srand|Pop a value and seed the random device|1|
|srandt|Seed the random device with the current time in seconds|0|

### File I/O Functions
|Function|Description|Expected stack size|
|-|-|-|
|readfile|Pop the size of the file name, then pop that many characters and create a string. Open the file with the given string and push all contents to stack|>1|
|writefile|Pop the size of the file name, then pop that many characters and create a file name. Pop another number, then pop that many characters and create a string. Write the string to the file name|>2|
|appendfile|Same as writefile, but appends the string|>2| 
|isfile|Get the filename and return 1 if it exists and is a file, 0 otherwise|>1|
|isdirectory|Get the filename and return 1 if it exists and is a directory, 0 otherwise|>1|
|createdirectory|Get the name and create a directory|>1|
|createfile|Get the name and create a file|>1|
|iteratedirectory|Get the name and iterate all files in the directory. Push 0 first, the push all file names and their character count, like so (top to bottom): '6 file.a 7 file2.a 7 file3.a 0'.|>1|
|deletefile|Get the name and delete the file/directory.|>1|

### Debug Functions
|Function|Description|Expected stack size|
|-|-|-|
|logstack|Log all stack values|0|
|logdefer|Log all defer stack commands|0|
|logregs|Log all accessed registers|0|
|logvars|Log all variables and their values|0|
|loglabels|Log all labels and their positions|0|
