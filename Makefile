SOURCES = solver.c intlist.c exactcover.c intprogramming.c
CFLAGS = -Wall -O2 -g

solver: $(SOURCES:.c=.o) main.c
	gcc -o $@ -Wall -Wextra -g $^

test: $(SOURCES:.c=.o) test*.c
	gcc -o $@ -Wall -Wextra -DUNIT_TEST $^ -lcheck -lsubunit -lm

clean:
	$(RM) *.o test.exe test solver.exe solver

