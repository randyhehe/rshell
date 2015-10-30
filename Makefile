all:
	rm -rfv bin
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic -I boost_1_59_0 src/main.cpp src/Connector.cpp src/Or.cpp src/And.cpp src/Semicolon.cpp src/Run.cpp src/Parse.cpp -o bin/rshell

rshell:
	rm -rfv bin
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic -I boost_1_59_0 src/main.cpp src/Connector.cpp src/Or.cpp src/And.cpp src/Semicolon.cpp src/Run.cpp src/Parse.cpp -o bin/rshell

clean:
	rm -rfv bin
