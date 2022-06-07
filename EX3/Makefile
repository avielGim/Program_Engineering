VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99
FLAGS= -Wall -g

all: shell

shell: main.o
	gcc $(FLAGS) -o shell main.o 
main.o: main.c
	gcc $(FLAGS) -c main.c

valgrind: main
	valgrind --leak-check=full --error-exitcode=99 --tool=memcheck $(VALGRIND_FLAGS) ./a

.PHONY: clean all


clean: 
	rm -f *.o *.a main