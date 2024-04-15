.PHONY: build run

build:
	gcc -O0 -g ./src/main.c -o ./bin/main.o


run:
	chmod +x bin/main.o
	./bin/main.o
