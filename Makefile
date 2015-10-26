all:
	rm -rfv bin
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell

rshell:
	rm- rfv bin
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell

clean:
	rm -rfv bin
