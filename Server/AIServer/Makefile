#If you want to change makefiles in the project, please first check the notes in the makefile in ./Server/shared

#OBJECTS = ${patsubst %.cpp, %.o, ${SOURCES}}
STDAFX = $(SHARED)/stdafx.h
SHARED = ../shared
.PHONY = all clean

CFLAGS = -std=c++11

main: 2r 3r 4r main.o $(STDAFX)
	g++ -std=c++11  -o main main.o ../shared/Thread.o ../shared/TimeThread.o ../shared/globals.o

.ONESHELL:
2r:
	cd ../shared&&make globals.o
.ONESHELL:
4r:
	cd ../shared&&make TimeThread.o
.ONESHELL:
3r:
	cd ../shared&&make Thread.o

*.o:
	g++ ${CFLAGS} -c $@.cpp

.ONESHELL:
clean:
	rm -f *.o main
	cd ../shared&&make clean




