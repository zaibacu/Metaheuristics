SRC=main.cpp PythonLogger.cpp Util.cpp ObjectFactory.cpp LogLocator.cpp FitnessCalculator.cpp
OBJ=bin/PythonLogger.o bin/Util.o bin/ObjectFactory.o bin/LogLocator.o bin/FitnessCalculator.o
LIB=../bin/MetaHeuristics
INCLUDE =-include precompiled.h

CFLAGS=-m64 -Wall -std=c++0x -g3 -fPIC
LDFLAGS=-shared

PARAMS = -DMAX_VALUE=10 -DMIN_VALUE=0 -DPOPULATION_SIZE=25 -DINPUT_SIZE=2 -DMAX_DEPTH=5 -DSIZE=14

all: $(LIB)

rebuild: clean $(LIB)

clean:
	rm -rf bin/*.o
	rm -rf bin/Cache/*.o

bin/%.o:
	g++ -o bin/$*.o $(INCLUDE) $(CFLAGS) $(PARAMS) -c $*.cpp

$(LIB): $(OBJ)
	g++ $(LDFLAGS) -o $(LIB) $(OBJ)



