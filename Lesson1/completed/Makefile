CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: output

output: main2.o utils.o 
	${CC} main2.o utils.o -o output

main2.o: main2.c
	${CC} ${CFLAGS} main2.c 

utils.o: utils.c
	${CC} ${CFLAGS} utils.c

clean:
	rm -f *.o *~ *.out

