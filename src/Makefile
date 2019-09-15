
APP=sim
# DAPP=calculadora_d


SRC= Bus.cpp BusPort.cpp Cache.cpp CacheController.cpp Clock.cpp CPU.cpp InstructionGenerator.cpp RAM.cpp SimulationComputer.cpp
HEADERS = $(patsubst %,$(INCDIR)%,$(SRC:.cpp=.hpp))
OBJECTS = $(patsubst %,$(ODIR)%,$(SRC:.cpp=.o)) 

#extensions
SRC_EXT=.cpp
OBJ_EXT=.o
INC_EXT=.hpp


#SRCDIR=../src
INCDIR=../include/
BINDIR=../build/bin/
ODIR=../build/obj/
# LDIR=../lib

# OBJ=calculadora.o 
# SLIB=simple_math.o
# DLIB=simple_math.so

CC=gcc
CXX=g++
CFLAGS = -c -Wall -std=c++11 -fopenmp -g
LDFLAGS = -L$(ODIR) -I$(INCDIR)  -Wall -std=c++11 -g
LIBS= -fopenmp -lpthread
.PHONY:all clean

all: $(BINDIR)$(APP)


$(BINDIR)$(APP) :  $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)


# $(OBJECTS) : $(SRC) $(HEADERS)
# 	$(CXX) $(CFLAGS) -o $@ $(@:.o=.cpp) 

$(ODIR)%.o : %.cpp $(HEADERS)
	$(CXX) -o $@ $< $(CFLAGS)


clean: 
	rm -rf ../build/bin/* ../build/obj/* 