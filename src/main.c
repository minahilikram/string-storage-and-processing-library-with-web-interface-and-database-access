#include <stdio.h>
#include "listio.h"
#include <stdlib.h>
#include <string.h>

int main (int argc, const char * argv[]) {

	dataHeader *myHeader;

	myHeader = buildHeader();

	setName(myHeader, "mona");
	addString(myHeader, "calvin");
	addString(myHeader, "is");
	addString(myHeader, "awesome\n");

	printf("name: %s\n", getName(myHeader));
	printf("length: %d\n", getLength(myHeader));
	printString(myHeader);

	processStrings(myHeader);

	return 0;
}
