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



There are two types of declarations: function declarations and variable declarations.

Variable declarations:
[sign (optional)] [type] identifier = [CHARACTER/STRING/NUMBER]; creates a variable equal to a value

a ~ may proceed identifier and a [NUM] may follow or both
~id is a pointer
id[NUM] is an array of size NUM
~id[NUM] is an array of pointers size NUM

Function declarations:




Scope:
Scope is defined by different levels starting at level 0 where the global variables are. Inside any curly braces increases the level by 1. You can have multiple scopes at the same level such as an if { ... } else { ... } where ... in if and else are at the same level but in different scopes. You cannot declare two identifiers with the same name in the same scope, but you can define them with the same name in different scope. Parameters and expression inside statements (such as if (...) { } or while (...) {} ) will be considered within the scope of the { }. You may call identifiers that have been declared in scopes above and at the current scope, but they must be declared BEFORE used. You cannot call a function in the beginning of the file that is declared somewhere below.
