#
# concurrency_profiler makefile
#

SRC = src
BIN = bin
INC = inc

EXE = profiler
DEXE = d_profiler

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
EXTRAFLAGS = -ltbb -L$(TBB_PATH) -I/$(INC)

SOURCES = $(wildcard $(SRC)/*.cpp)
OBJS = $(SOURCES:src/%.cpp=bin/%.o)
DOBJS = $(OBJS:bin/%.o=bin/d_%.o)


all: $(EXE)
	@echo ========== finished building ==========
	@echo 

debug: $(DEXE)

$(EXE): $(OBJS)
	@echo building executable "$@"
	@$(CXX) $(CXXFLAGS) $(OBJS) $(EXTRAFLAGS) -o profiler 

$(DEXE): $(DOBJS)
	@$(CXX) $(CXXFLAGS) $(DOBJS) $(EXTRAFLAGS) -g -o d_profiler

$(BIN)/%.o: $(SRC)/%.cpp $(INC)/%.h begin
	@echo building dependency "$@"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BIN)/d_%.o: $(SRC)/%.cpp $(INC)/%.h
	@$(CXX) $(CXXFLAGS) -g -o $@ -c $<

run: all
	@source setup && ./$(EXE)

clean: 
	@echo ========== cleaning up ===========
	@rm -f *~ $(BIN)/*.o profiler d_profiler

begin:
	@echo ========== started building ===========

