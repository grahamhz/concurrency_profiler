#
# concurrency_profiler makefile
#

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

OBJS = profiler.o

all: profiler

debug: dprofiler

profiler: $(OBJS)
	$(CXX) $(CXXFLAGS) -o profiler $(OBJS)

dprofiler: $(OBJS)
	$(CXX) $(CXXFLAGS) -g -o profiler $(OBJS)

profiler.o: profiler.cpp

clean: 
	rm -f *~ *.o profiler
