#include <stdio.h>
#include "listio.h"
#include <stdlib.h>
#include <string.h>

enum AsciiCharacters {
  SPACE = 32
};

enum AsciiCharacters ascii;

/*Initalizes Header*/
dataHeader *buildHeader() {
    dataHeader *header;
    header = malloc(sizeof(dataHeader));
    header->next = NULL;
    header->name = NULL;
    header->length = 0;

    return header;
}

void setName(dataHeader *header, char *name) {
    header->name = NULL;
    header->name = malloc(strlen(name)+1);
    strcpy(header->name, name);
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

   if (str == NULL) {
      return;
   }
   string->string = NULL;
   string->string = malloc(strlen(str)+1);
   strcpy(string->string, str);

   string->next = NULL;
   header->length = header->length + strlen(str);
   if (header->next == NULL) {
        header->next = string;
    } else {
        dataString *current;
        current = header->next;
        /*TODO: maybe move to getTail function*/
        while (current->next != NULL) {
           current = current->next;
         }
        current->next = string;
    }
}

void printString(dataHeader *header) {
    dataString *string;
    string = header->next;
    while (string != NULL) {
        printf("%s\n", string->string);
        string = string->next;
    }
}

void removeWithIndex(char *input, char *output, int index) {
  strncpy(output, input, index);
  strncpy(((char*)output)+index,((char*)input)+index+1,strlen(input)-1-index);
}

void compressChar(char *string, int index, int asciiVal) {

    if (string[index + 1] == asciiVal) {
      char *output = calloc(strlen(string), sizeof(char));

      removeWithIndex(string, output, index);

      strcpy(string, output);
      free(output);

      compressChar(string, index, asciiVal);
    }
}

void handleHTMLConversion(char *string, int index) {

    if (string[index + 1] == 10 || string[index + 1] == 13) {
      /*TODO: Handle Mutiple /n & /r aka <p>*/

    } else {
      /*TODO: <br> stuff*/

    }
}

void processStrings(dataHeader *header) {
    dataString *node = header->next;

    while (node != NULL) {

        int i = 0;
        header->length -= strlen(node->string);
        while (node->string[i] != '\0') {
            switch(node->string[i]) {

                case 32:
                    compressChar(node->string, i, 32);
                    break;
                case 9:
                    compressChar(node->string, i, 9);
                    break;
                case 10:
                case 13:
                    handleHTMLConversion(node->string, i);
                    break;
            }
            i++;
        }
        header->length += strlen(node->string);
        node = node->next;
    }
}

void freeDataStrings (dataString *string) {
   if (string->next != NULL) {
      freeDataStrings(string->next);
   }
   free(string->string);
   free(string);
}

void freeStructure(dataHeader *header) {
    freeDataStrings(header->next);

    free(header->name);
    free(header);
}
