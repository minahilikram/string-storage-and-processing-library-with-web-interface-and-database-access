# C compiler
CC          =	gcc

# Flags that are sent to the compiler
CFLAGS      =	-Wall -ansi -pedantic -g

# Directories where the compiler can find things
INCLUDES    =	-Iinclude

# Object files to be created
OBJS    	=	listio.o

# Source File
SRC			=	listio.c

# Name of the library to be created
LIBNAME		= 	liblistio.a

all : mklib main

mklib: compile
	ar cr $(LIBNAME) $(OBJS)

compile:
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJS) $(INCLUDES)

main: link
	$(CC) main.o -o a3 -llistio -L.

link:
	$(CC) $(CFLAGS) main.c -o main.o -c $(INCLUDES)

run:
	./a3 ${ARGS}

clean:
	@ rm *.o
	@ rm *.a
	@ rm a3
	@ rm *.html
