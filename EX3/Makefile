VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

valgrind: main
	valgrind --leak-check=full --error-exitcode=99 --tool=memcheck $(VALGRIND_FLAGS) ./a 