Randy He (rhe004@ucr.edu)

# rshell
#### Introduction
This program is intended to behave like a linux shell. Coded in C++ for University of Riverside's Software Construction Class, CS100.

#### Features
* Supports full support of connectors (||, &&, ;)
* Supports full support of comments (#)
* Supports checking of syntax errors 
  * Starting a command with a connector will yield an error
  * Incorrect use of connector symbols | & and ; will yield an error
* Supports use of quotes to combine multiple words into a single parameter.
* Supports "test" to check for files and directories
* Supports the use of precedence operators with () to change the precedence of the returns of commands, connectors, and chains of connectors.

#### Design
* My rshell is designed in a way where I have a class of static methods for correctly parsing data.
* Code for running rshell is in the Run class, which leads to a clean main.cpp and a class for specifically running commands.
* Since test is not supported in execvp, I separated the executeSingle command into either executing test or execvp function based on what the user inputs.
* The massive use of boolean return and parameters in functions is to be able
to detect these boolean values in the main function to effectively end the program with return 0, which is a cleaner and less prone to errors than directly calling exit.
* Use of class Connector, which has subclasses Semicolon, Add, and Or inheriting from it. They all have different implementations of the executeNext() function, which will be used to determine whether the next command will be executed or not based on connectors.

#### Bugs/Limitations
* Leading or trailing characters next to quotes will have a space appended when using echo. For example. echo hello"world" will print hello world instead of helloworld.
* Putting connectors inside of quotes will still yield a syntax error.
* Ctrl-D will crash program.
* Precedence operators only work when appended to the end of a command, not when
there is whitespace.

#### Installation
```
$ git clone https://github.com/randyhehe/rshell.git
$ cd rshell
$ git checkout hw2
$ make
$ bin/rshell
```

