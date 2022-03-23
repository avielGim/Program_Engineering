FLAGS= -Wall -g

all: slast

slast: main.o libmyslast.a
	gcc $(FLAGS) -o slast main.o libmyslast.a

libmyslast.a: slast_func.o
	ar -rcs libmyslast.a slast_func.o

main.o: main.c slast_func.h
	gcc $(FLAGS) -c main.c

slast_func.o: slast_func.c slast_func.h
	gcc $(FLAGS) -c slast_func.c


.PHONY: clean all

clean: 
	rm -f *.o *.a slast