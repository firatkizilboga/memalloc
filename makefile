.PHONY: build run

build:
	gcc -O0 -g3 -c ./src/freelist.c -o ./bin/freelist.o -Iinclude/
	gcc -O0 -g3 ./src/main.c -o ./bin/main.o ./bin/freelist.o -Iinclude/


run:
	chmod +x bin/main.o
	./bin/main.o
