# rshell
#### Introduction
This program is intended to behave like a linux shell. Coded in C++ for University of Riverside's Software Construction Class, CS100.

#### Installation
```
$ git clone https://github.com/randyhehe/rshell.git
$ cd rshell
$ make
$ bin/rshell
```

#### Features
* Supports full support of connectors (||, &&, ;)
* Supports full support of comments (#)
* Supports checking of syntax errors 
  * Starting a command with a connector will yield an error
  * Incorrect use of connector symbols | & and ; will yield an error
* Supports use of quotes to combine multiple words into a single parameter.

#### Bugs/Limitations
* Leading or trailing characters next to quotes will have a spaces appended when using echo. For example. echo hello"world" will print hello world instead of helloworld.
* Putting connectors inside of quotes will still yield a syntax error.
* Ctrl-D will crash program.
