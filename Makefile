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

DEST		= /home/minahil/Documents/CIS*2750/a1/example

all : mklib

mklib: compile
	ar cr $(LIBNAME) $(OBJS);\
	cp listio.h $(DEST)

compile:
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJS) $(INCLUDES)

clean:
	@ rm *.o
