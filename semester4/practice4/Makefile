# Variables
CC=g++
CFLAGS=-Wall
LDFLAGS=
SOURCES=
MAIN=main.cpp
EXECUTABLE=main.o

# Tasks
all:
	${CC} ${CFLAGS} -o ${EXECUTABLE} ${MAIN} ${SOURCES}

run: all
	./${EXECUTABLE}

clean:
	rm `find . -name "*o"`
