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

all : mklib

mklib: compile
	ar cr $(LIBNAME) $(OBJS)

compile:
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJS) $(INCLUDES)

clean:
	@ rm *.o
