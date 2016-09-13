#include <stdio.h>
#include "linkedList.h"
#include <stdlib.h>
#include <string.h>

int main (int argc, const char * argv[]) {

	dataHeader *myHeader;

	myHeader = buildHeader();

	setName(myHeader, "mona");

	printf("name: %s\n", getName(myHeader));
	printf("length: %d\n", getLength(myHeader));

	return 0;
}
