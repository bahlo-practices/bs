# Variables
CC=g++
CFLAGS=-Wall
LDFLAGS=
SOURCES=
MAIN=main.cpp
EXECUTABLE=trash

# Tasks
all:
	${CC} ${CFLAGS} -o ${EXECUTABLE} ${MAIN} ${SOURCES}

run: all
	./${EXECUTABLE}

debug: all
	./${EXECUTABLE} debug

clean:
	rm `find . -name "*o"`
