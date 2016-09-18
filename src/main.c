#include <stdio.h>
#include "listio.h"
#include <stdlib.h>
#include <string.h>

int main (int argc, const char * argv[]) {

	dataHeader *myHeader;

	myHeader = buildHeader();

	setName(myHeader, "mona\0");
	addString(myHeader, "Hi!\n\n\n\n\nMy name is Calvin.\0");

	printf("name: %s\n", getName(myHeader));
	printf("length: %d\n", getLength(myHeader));

	printString(myHeader);
	processStrings(myHeader);
	processStrings(myHeader);
	processStrings(myHeader);
	processStrings(myHeader);
	printString(myHeader);
	printf("length: %d\n", getLength(myHeader));
	freeStructure(myHeader);

	return 0;
}
