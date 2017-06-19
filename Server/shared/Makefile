#Notes about the linking
#	-Never link any of the stdafx.* files to any of the other object files, they are there just for precompiled headers.
#	-(Though it might be obvious), if there is function defined in header files, make sure that they are inlined correctly.
#	-Always compile each *.cpp file seperately, and then link then together with the main at the same time, see Makefile in AIServer.

EXECUTABLE =
.PHONY = all clean
CFLAGS = -std=c++11

#For some reason, the automated recipe causes an error during the compiliation.
#*.o:
#	g++ ${CFLAGS} -c $@.cpp

Thread.o:
	g++ ${CFLAGS} -c Thread.cpp

TimeThread.o: 
	g++ ${CFLAGS} -c TimeThread.cpp 

globals.o: 
	g++ ${CFLAGS} -c globals.cpp 

#For one-by-one compiling

#Thread: stdafx.o globals.o Thread.o globals.h
#	g++ ${CFLAGS} -o Thread Thread.o globals.o

#TimeThread: stdafx.o globals.o  Thread.o TimeThread.o
#	g++ ${CFLAGS} -o TimeThread TimeThread.o Thread.o globals.o

clean:
	rm -f *.o $(EXECUTABLE)