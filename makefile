.PHONY: build run

build:
	clang -O0 -g3 -c ./src/freelist.c -o ./bin/freelist.o -Iinclude/
	clang -O0 -g3 -c ./src/memalloc.c -o ./bin/memalloc.o -Iinclude/
	clang -O0 -g3 ./src/main.c -o ./bin/main.o ./src/memalloc.c ./bin/freelist.o -Iinclude/
run:
	chmod +x bin/main.o
	./bin/main.o
