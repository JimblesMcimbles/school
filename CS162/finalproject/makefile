###############################################################################
# makefile for final project
# Based on robust makefile for CS 162 and video #3 on makefiles
###############################################################################

CXX = g++
CXXFLAGS = -std=c++0x
# turning on all warnings
CXXFLAGS += -Wall
# -pedantic-errors enforces standard
CXXFLAGS += -pedantic-errors
# turns on debugging info
CXXFLAGS += -g
# optimization for tail recursion
# CXXFLAGS += -O3

SRCS = main.cpp room.cpp subroom.cpp player.cpp
HEADERS = room.h subroom.h player.h
PROGS = finalproject

all: ${PROGS}

default: ${SRCS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROGS}

finalproject: ${SRCS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROGS}
	
clean:
	rm -f ${PROGS} *.o *~