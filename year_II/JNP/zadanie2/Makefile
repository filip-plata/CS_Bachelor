WFLAGS := -Wall -Wextra -g
all: lib ctest cpptest

lib: strdeque.o strdequeconst.o

ctest: lib
	gcc -c $(WFLAGS) -O2 strdeque_test1.c -o strdeque_test1.o
	g++ strdeque_test1.o strdeque.o strdequeconst.o -o strdeque_test1

cpptest: lib
	g++ -c $(WFLAGS) -O2 -std=c++14 strdeque_test2a.cc -o strdeque_test2a.o
	g++ -c $(WFLAGS) -O2 -std=c++14 strdeque_test2b.cc -o strdeque_test2b.o
	g++ strdeque_test2a.o strdeque.o strdequeconst.o -o strdeque_test2a
	g++ strdeque_test2b.o strdeque.o strdequeconst.o -o strdeque_test2b
	
strdeque.o: strdeque.cc strdeque.h
	g++ -c $(WFLAGS) -O2 -std=c++14 strdeque.cc -o strdeque.o

strdequeconst.o: strdequeconst.cc strdequeconst.h
	g++ -c $(WFLAGS) -O2 -std=c++14 strdequeconst.cc -o strdequeconst.o

# Gets a test program from stdin, links symbols from strdeque
# and strdequeconst, compiles it and runs assertions in test program:
# Example: make -s run < strdeque_test1.c
run:
	rm -f strdeque.o strdequeconst.o
	g++ -c -DNDEBUG -O2 -std=c++14 strdeque.cc -o strdeque.o
	g++ -c -DNDEBUG -O2 -std=c++14 strdequeconst.cc -o strdequeconst.o
	gcc -c -o strdeques_test.o -x c -
	g++ strdeques_test.o strdeque.o strdequeconst.o -o strdeques_test
	./strdeques_test
	rm -f strdeques_test strdeques_test.o

.PHONY: lib clean

clean:
	rm -f *.o strdeque_test??
