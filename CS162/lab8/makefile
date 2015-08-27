###############################################################################
# makefile for lab 8
# Author: Sharon Kuo
# Date: 2015-02-28
# Last modified: 2015-02-28
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

SRCS = lab08.cpp
HEADERS = 
PROGS = lab08

all: ${PROGS}

default: ${SRCS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROGS}
	
lab08: ${SRCS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROGS}
	
clean:
	rm -f ${PROGS} *.o *~
