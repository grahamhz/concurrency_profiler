#
# concurrency_profiler makefile
#

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

OBJS = profiler.o
DOBJS = d_profiler.o

all: profiler

debug: d_profiler

profiler: $(OBJS)
	$(CXX) $(CXXFLAGS) -o profiler $(OBJS)

d_profiler: $(DOBJS)
	$(CXX) $(CXXFLAGS) -g -o d_profiler $(DOBJS)

profiler.o: profiler.cpp

# the way this is set up, you have to explicitly
# define the debug file call in order to add -g
d_profiler.o: profiler.cpp
	$(CXX) $(CXXFLAGS) -g -c -o d_profiler.o profiler.cpp

clean: 
	rm -f *~ *.o profiler d_profiler
