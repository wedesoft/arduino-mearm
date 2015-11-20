GTEST=/usr/src/gtest
CXX = g++

all: all-recursive all-local

all-local: test-suite

check: all-local
	./test-suite

test-suite: test-suite.o gtest-all.o
	$(CXX) -o $@ test-suite.o gtest-all.o -lpthread

test-suite.o: test-suite.cc
	$(CXX) -c -o $@ $<

gtest-all.o: $(GTEST)/src/gtest-all.cc
	g++ -c -I$(GTEST) -o $@ $<

clean: clean-recursive clean-local

clean-local:
	rm -f test-suite *.o

all-recursive:
	cd arduino && make && cd ..

clean-recursive:
	cd arduino && make clean && cd ..
