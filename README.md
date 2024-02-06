# JLang Documentation

Try running code on my website: https://jasonhschwartzman.com/jlang

### Keywords:
Types: int, long, double, short, char  
Sign: signed, unsigned  
Loops: for, while  
Loop control: break, continue  
Conditionals: if, else, elseif  
Functions: return  


Operations: + - * / % & | ^ == <= >= < > && || !=  
Assignment: =  

### Expressions:
Expressions are combinations of values where some being literal while others being a variable that are evaluated to a single value. Expressions are read BACKWARDS.  

2 - 3 / 4  
read 4 / 3 - 2 which is equal to -1  
3 < 1 - 3  
read 3 - 1 < 3 which is equal to 1 (true)  

These combinations are separated by operations.  

The different types of the values:  
identifiers (or dereferenced pointers)  
function calls (return value)  
literals  
memory locations  
array values  
strings  


There are two types of declarations: function declarations and variable declarations.

## Variables: 

### Variable declarations:
[sign (optional)] [type] identifier = [CHARACTER/STRING/NUMBER]; //for global variables  
[sign (optional)] [type] identifier = expression; //for local variables  
                                 
### Variable assignments:
identifier = expression;

Identifier must be declared prior.

a ~ may proceed identifier and a [NUM] may follow or both  
~id is a pointer  
id[NUM] is an array of size NUM  
~id[NUM] is an array of pointers size NUM  

### Arrays:
Arrays must have a constant size even if the array is a local variable.  
int array[5] = 0;  
This initializes an array with 5 elements all equal to 0.  
You can update values in the array like:  
array[0] = 2;  
This updates the 1st element to 2.  

### Function declarations:
[sign (optional)] [type] identifier (parameters) { function body }  
parameters are 0 or more [sign (optional)] [type] identifier    separated by a comma.  

## The function body can consists of statements:

### Conditionals:
    if (expr) { statements }
    elseif (expr) { statements }
    else { statements }
 
 The ordering most go if -> elseif -> else with 0 or more elseif statements. else is optional. elseif and else must directly follow if. There can be if statements without elseif or else statements.

### Variable declarations and assignments:
shown above

### Loops:
    while (expr) {
        statements
    }

    for (variable declaration; expr; assignment) {
        statements
    }
    Ex.:
    for (int i = 0; 5 < i; i = i + 1) {
        statements
    }

### Function Calls:
identifier(arg1,arg2);
If there is a function declaration previously defined as calc(int num1, int num2)
we can call this function like calc(10,15);

## Scope:
Scope is defined by different levels starting at level 0 where the global variables are while local variables are level 1+. Inside any curly braces increases the level by 1. You can have multiple scopes at the same level such as an if { ... } else { ... } where ... in if and else are at the same level but in different scopes. You cannot declare two identifiers with the same name in the same scope, but you can define them with the same name in different scope. Parameters and expression inside statements (such as if (...) { } or while (...) {} ) will be considered within the scope of the { }. You may call identifiers that have been declared in scopes above and at the current scope, but they must be declared BEFORE used. You cannot call a function in the beginning of the file that is declared somewhere below.

## Printing:
There are two built in functions: printd(int number) and prints(char ~string)  
printd takes a number and prints it to stdout.  
prints takes a GLOBAL char pointer (also known as a string ex: char ~string = "Hello world!\n") and prints it to stdout.  

## Future work:
JLang recognizes different data types such as short, int, long syntactically. Even though data types have different sizes in theory(int is 4 bytes, short is 2 bytes) all data types are implemented as signed 8 bytes. In the future, I plan to add support for this feature. 

Try running code on my website: https://jasonhschwartzman.com/jlang

Reference: https://www3.nd.edu/~dthain/compilerbook/compilerbook.pdf
