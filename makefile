.PHONY: build run

build:
	clang -O0 -g3 -c src/freelist.c -o bin/freelist.o -Iinclude/
	clang -O0 -g3 -c src/memalloc.c -o bin/memalloc.o bin/freelist.o -Iinclude/
	clang -O0 -g3 -c src/mymalloc.c -o bin/mymalloc.o bin/memalloc.o bin/freelist.o -Iinclude/

	clang -O0 -g3 src/P1.c  -o bin/P1.o bin/mymalloc.o bin/memalloc.o bin/freelist.o -Iinclude/
	clang -O0 -g3 src/P2.c  -o bin/P2.o bin/mymalloc.o bin/memalloc.o bin/freelist.o -Iinclude/
	clang -O0 -g3 src/P3.c  -o bin/P3.o bin/mymalloc.o bin/memalloc.o bin/freelist.o -Iinclude/
	clang -O0 -g3 src/P4.c  -o bin/P4.o bin/mymalloc.o bin/memalloc.o bin/freelist.o -Iinclude/

	clang -O0 -g3 src/allprograms.c  -o bin/allprograms.o bin/mymalloc.o bin/memalloc.o bin/freelist.o -Iinclude/

run:	
	clang -O0 -g3 src/$(CMD).c  -o bin/$(CMD).o bin/mymalloc.o bin/memalloc.o bin/freelist.o -Iinclude
	./bin/$(CMD).o $(STRAT)

