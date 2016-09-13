#include <stdio.h>
#include "listio.h"
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
    if (header->next == NULL) {
        dataString *string;
        string = malloc(sizeof(dataString));

        string->string = NULL;
        string->string = malloc(sizeof(str));
        strncpy(string->string, str, (sizeof(str) + 1));

        string->next = NULL;

        header->length = strlen(str);
        header->next = string;
    }
    else {
        dataString *current;
        current = header->next;
        while (current != NULL) {
            if (current->next == NULL) {
                dataString *string;
                string = malloc(sizeof(dataString));

                string->string = NULL;
                string->string = malloc(sizeof(str));
                strncpy(string->string, str, (sizeof(str) + 1));

                string->next = NULL;

                header->length = header->length + strlen(str);
                current->next = string;
                break;
            }
            else {
                current = current->next;
            }
        }
    }
}

void printString(dataHeader *header) {
    dataString *string;
    string = header->next;
    while (string != NULL) {
        printf("%s ", string->string);
        string = string->next;
    }
}
