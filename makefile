.PHONY: build run

build:
	rm -rf bin
	mkdir bin
	clang -O0 -g3 -c src/freelist.c -o bin/freelist.o -Iinclude/
	clang -O0 -g3 -c src/memalloc.c -o bin/memalloc.o bin/freelist.o -Iinclude/
	clang -O0 -g3 -c src/easyalloc.c -o bin/easyalloc.o bin/memalloc.o bin/freelist.o -Iinclude/

run:	
	clang -O0 -g3 src/$(CMD).c  -o bin/$(CMD).o bin/easyalloc.o bin/memalloc.o bin/freelist.o -Iinclude
	./bin/$(CMD).o $(STRATEGY)
