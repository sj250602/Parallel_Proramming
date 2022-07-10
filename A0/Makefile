CC=g++
#g++ -std=c++11 -O2 -mavx2 -c classify.cpp 
CFLAGS=-std=c++11 -O2

sources=main.cpp classify.cpp
objects=$(sources:.cpp=.o)

classify:$(objects)
	$(CC) $(CFLAGS) -fopenmp -pg $^ -o $@

run: classify
	./classify rfile dfile 1009072 4 3

#classify.o: classify.h
#	touch classify.o
#	$(CC) $(CFLAGS) -c -pg $<
#main.o: classify.h
#	$(CC) $(CFLAGS) -c -pg $<

%.o: %.cpp classify.h
	$(CC) $(CFLAGS) -c -pg $<

gprof: 
	gprof -b classify gmon.out > analysis_gprof.out

valgrind: 
	valgrind ./classify rfile dfile 1009072 4 3

valgrind_leak_check:
	valgrind --leak-check=full ./classify rfile dfile 1009072 4 3
valgrind_cache:
	valgrind --tool=cachegrind ./classify rfile dfile 1009072 4 3