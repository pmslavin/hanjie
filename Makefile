CXX	:= g++
CXXFLAGS:= -std=c++11 -g -O0
WARN	:= -Wall -Wextra
OBJECTS	:= Cell.o Grid.o Frame.o
LIBS	:= -lSDL2

all: main

main: ${OBJECTS} main.cpp
	${CXX} ${CXXFLAGS} ${WARN} main.cpp ${OBJECTS} -o main ${LIBS}

Cell.o: Cell.cpp Cell.h
	${CXX} ${CXXFLAGS} ${WARN} -c Cell.cpp

Grid.o: Grid.cpp Grid.h
	${CXX} ${CXXFLAGS} ${WARN} -c Grid.cpp

Frame.o: Frame.cpp Frame.h
	${CXX} ${CXXFLAGS} ${WARN} -c Frame.cpp

clean:
	-rm *.o main
