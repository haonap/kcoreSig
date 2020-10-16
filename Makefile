GUROBI_HOME =/opt/gurobi/9.0.1/linux64

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

GPP = /usr/bin/g++

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

OPT = -m64 -O3 -std=c++0x -Wextra -pedantic

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CFLAGS=-Wall
INSTANCE  = ./
INC     = $(GUROBI_HOME)/include
OPT_INC = $(OPT) -I$(INC)
LIBDIR  = $(GUROBI_HOME)/lib/
LIB     = -L$(LIBDIR) -lgurobi_g++4.2 -lgurobi_c++ -lgurobi90 -lm -lpthread
SOURCES= main.cpp functions.cpp classes.cpp
HEADERS= functions.h classes.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(GPP) $(OPT_INC) $(OBJECTS) -o $@ $(LIB)

%.o:  %.cpp  $(HEADERS)
	$(GPP) $(CFLAGS) $(OPT_INC) -c $< -o $@ 


	
clean :
	/bin/rm -rf *.o *_c *_c++ *.class *.log *.rlp *.lp *.bas *.ilp
	/bin/rm -rf *~ *.mps *.ord *.sos *.sav *.net *.msg *.clp

# ------------------------------------------------------------
