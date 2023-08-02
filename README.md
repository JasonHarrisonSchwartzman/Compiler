# Compiler

Language:

Keywords:
Types: int, long, double, short, char
Sign: signed, unsigned
Loops: for, while
Loop control: break, continue
Conditionals: if, else, elseif
Functions: return


Operations: + - * / % & | ^ == <= >= < > && || !=
Assignment: =

Expressions:
Expressions are combinations of values where some being literal while others being a variable that are evaluated to a single value. 
These combinations are separated by operations.

The different types of the values:
identifiers (or dereferenced pointers)
function calls (return value)
literals
memory locations
array values
strings

Semantics:


There are two types of declarations: function declarations and variable declarations.

Variables: 

Scope:
Scope is defined by different levels starting at level 0 where the global variables are while local variables are level 1+. Inside any curly braces increases the level by 1. You can have multiple scopes at the same level such as an if { ... } else { ... } where ... in if and else are at the same level but in different scopes. You cannot declare two identifiers with the same name in the same scope, but you can define them with the same name in different scope. Parameters and expression inside statements (such as if (...) { } or while (...) {} ) will be considered within the scope of the { }. You may call identifiers that have been declared in scopes above and at the current scope, but they must be declared BEFORE used. You cannot call a function in the beginning of the file that is declared somewhere below.

Variable declarations:
[sign (optional)] [type] identifier = [CHARACTER/STRING/NUMBER]; //for global variables
                                    = expression; //for local variables
                                 

a ~ may proceed identifier and a [NUM] may follow or both
~id is a pointer
id[NUM] is an array of size NUM
~id[NUM] is an array of pointers size NUM

Function declarations:
[sign (optional)] [type] identifier (parameters) { function body }
parameters are 0 or more [sign (optional)] [type] identifier sepearted by a comma.

The function body can consists of statements:

Conditionals:
 if (expr) { function body }
 elseif (expr) { function body }
 else { function body}
 
 the ordering most go if -> elseif -> else with 0 or more elseif statements. else is optional. elseif and else must directly follow if. There can be if statements without elseif or else statements.

 Reference: https://www3.nd.edu/~dthain/compilerbook/compilerbook.pdf
 
 
 
 
 
 
 
 
