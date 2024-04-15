.PHONY: build run

build:
	gcc -g ./src/main.c -o ./bin/main.o


run:
	chmod +x bin/main.o
	./bin/main.o
