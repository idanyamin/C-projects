CC = gcc
CFLAGS = -Wall -Wextra -Wvla -std=c11

make: heat_eqn.o calculator.o reader.o
	$(CC) heat_eqn.o calculator.o reader.o -o ex3

all: make
	./ex3 input.txt

heat_eqn.o: heat_eqn.c heat_eqn.h
	$(CC) -c heat_eqn.c

calculator.o: calculator.c calculator.h
	$(CC) -c calculator.c

reader.o: reader.c heat_eqn.h calculator.h
	$(CC) -c reader.c

clean:
	rm -f *.o ex3
