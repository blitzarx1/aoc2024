build:
	gcc -c ../array/array.c -o array.o && gcc main.c array.o -I../array -o main 

run: build
	./main 

clean: 
	rm main array.o

