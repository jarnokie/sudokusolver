SOURCES = ../lib/solver.c ../lib/intlist.c ../lib/exactcover.c
CFLAGS = -Wall -O2 -g

solver: $(SOURCES:.c=.o) main.c
	gcc -o $@ -Wall -Wextra -g $^

clean:
	$(RM) *.o solver

