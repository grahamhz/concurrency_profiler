#
# concurrency_profiler makefile
#

CC = g++
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
INCLUDES = -ltbb -L/opt/intel/compilers_and_libraries/mac/tbb/lib -I/includes

OBJS = profiler.o
DOBJS = d_profiler.o

all: profiler

debug: d_profiler

profiler: $(OBJS)
	$(CXX) $(CXXFLAGS) -o profiler $(OBJS) $(INCLUDES)

d_profiler: $(DOBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -g -o d_profiler $(DOBJS)

profiler.o: profiler.cpp

# the way this is set up, you have to explicitly
# define the debug file call in order to add -g
d_profiler.o: profiler.cpp
	$(CXX) $(CXXFLAGS) -g -c -o d_profiler.o profiler.cpp

clean: 
	rm -f *~ *.o profiler d_profiler
