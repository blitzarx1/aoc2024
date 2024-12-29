.PHONY: build clean run build-test clean-test run-test clean-all
.SILENT: build clean run build-test clean-test run-test clean-all

build:
	gcc -c ../array/array.c -o array.o && \
	gcc -c xmas_finder/xmas_finder.c -o xmas_finder.o && \
	gcc main.c array.o xmas_finder.o -I../array -Ixmas_finder -o main 

clean: 
	rm -f main tests array.o xmas_finder.o

run: build
	./main

build-test:
	gcc -c xmas_finder/xmas_finder.c -o xmas_finder.o && \
    gcc xmas_finder/tests.c xmas_finder.o -I/opt/homebrew/include -L/opt/homebrew/Cellar/cunit/2.1-3/lib/ -lCUnit -o tests

run-test: build-test
	./tests
	
clean-test:
	rm -f tests xmas_finder.o

clean-all: clean clean-test
