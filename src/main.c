#include <stdio.h>
#include "listio.h"
#include <stdlib.h>
#include <string.h>

int main (int argc, const char * argv[]) {

	struct dataHeader *myHeader;
	struct dataHeader *readHeader;

	myHeader = buildHeader();

	setName(myHeader, "My header is awesome.\0");
	addString(myHeader, "Hi!\n\n\n\n\nMy name is Calvin.\0");
	addString(myHeader, "Hi!\n\r \t\t   My name is Mona.\0");

	processStrings(myHeader);

	writeStrings("calvin.txt", myHeader);
	readHeader = readStrings("calvin.txt");

	if (readHeader != NULL) {

		printf("%s\n", getName(readHeader));
		printf("%d\n", getLength(readHeader));

		printString(readHeader);

		freeStructure(readHeader);
	}
	freeStructure(myHeader);
	return 0;
}
