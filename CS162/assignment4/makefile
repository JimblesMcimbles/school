###############################################################################
# makefile for assignment 4
# Author: Sharon Kuo
# Date: 2015-02-27
# Last modified: 2015-03-01
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

SRCS = creature.cpp assignment04.cpp
HEADERS = creature.h
PROG = assignment04

default: ${OBJS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROG}

assignment04: ${SRCS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROG}
	
clean:
	rm -f ${PROG} *.o *~
