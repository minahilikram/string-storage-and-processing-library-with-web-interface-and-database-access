#include <stdio.h>
#include "linkedList.h"
#include <stdlib.h>
#include <string.h>

/*Initalizes Header*/
dataHeader *buildHeader() {
    dataHeader *header;
    header = malloc(sizeof(dataHeader));
    header->name = NULL;
    header->length = 0;

    return header;
}

void setName(dataHeader *header, char *name) {
    header->name = NULL;
    header->name = malloc(sizeof(name));
    strncpy(header->name, name, (sizeof(name) + 1));
}

char *getName(dataHeader *header) {
    return header->name;
}

int getLength(dataHeader *header) {
    return header->length;
}

void addString(dataHeader *header, char *str) {
    dataString *string;
    string = malloc(sizeof(dataString));

    string->string = NULL;
    string->string = malloc(sizeof(str));
    strncpy(string->string, str, (sizeof(str) + 1));

    string->next = header->next;
    header->next = string;
}
