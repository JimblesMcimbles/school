###############################################################################
# makefile for assignment 3
# Author: Sharon Kuo
# Date: 2015-02-14
# Last modified: 2015-02-15
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

SRCS = creature.cpp assignment03.cpp
HEADERS = creature.h
PROG = assignment03

default: ${OBJS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROG}

assignment03: ${SRCS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROG}
	
clean:
	rm -f ${PROG} *.o *~
