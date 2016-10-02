# C compiler
CC          =	gcc

# Flags that are sent to the compiler
CFLAGS      =	-Wall -ansi -pedantic -g

# Directories where the compiler can find things
INCLUDES    =	-Iinclude

# Object files to be created
OBJS    	=	obj/listio.o

# Source File
SRC			=	src/listio.c

# Name of the library to be created
LIBNAME		= 	include/liblistio.a

all :
	gcc -shared -Wl,-soname,listio -o listio.so -fPIC $(SRC) $(INCLUDES)

mklib: compile
	ar cr $(LIBNAME) $(OBJS);\
	chmod +x src/main.py

compile:
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJS) $(INCLUDES)

run:
	src/./main.py inputfile

clean:
	@ rm obj/*;\
	@ rm include/*.a
