SOURCES = solver.c intlist.c exactcover.c

solver: $(SOURCES:.c=.o) main.c
	gcc -o $@ -Wall -Wextra -g $^

test: $(SOURCES:.c=.o) test*.c
	gcc -o $@ -Wall -Wextra -DUNIT_TEST $^ -lcheck -lsubunit -lm

solver.exe: $(SOURCES:.c=.o) main.c
	gcc -o $@ -Wall -Wextra -g $^

test.exe: $(SOURCES:.c=.o) test*.c
	gcc -o $@ -Wall -Wextra -DUNIT_TEST $^ -lcheck -lm

clean:
	$(RM) *.o test.exe test solver.exe solver

winclean:
	del *.o test.exe test solver.exe solver
