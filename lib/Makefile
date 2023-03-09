SOURCES = solver.c intlist.c exactcover.c
CFLAGS = -Wall -O2 -g

all: test
	./test

test: $(SOURCES:.c=.o) tests/*.c
	gcc -o $@ -Wall -Wextra -DUNIT_TEST $^ -lcheck -lsubunit -lm

clean:
	$(RM) *.o tests/*.o test

