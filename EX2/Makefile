FLAGS= -Wall -g

all: stree

stree: stree.o
	gcc $(FLAGS) -o stree stree.o 
stree.o: stree.c # func.h
	gcc $(FLAGS) -c stree.c


.PHONY: clean all


clean: 
	rm -f *.o *.a stree