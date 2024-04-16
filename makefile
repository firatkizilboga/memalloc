.PHONY: build run

build:
	gcc -O0 -g3 ./src/main.c -o ./bin/main.o


run:
	chmod +x bin/main.o
	./bin/main.o
