#include <stdio.h>
#include "listio.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

bool isValInArray(int val, int arr[], int size){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return true;
    }
    return false;
}

void compressAsciiValueAfterIndex(char *string, int index, int asciiVal[], int size) {

    if (isValInArray(string[index+1], asciiVal, size)) {
      char *output = calloc(strlen(string), sizeof(char));
      removeWithIndex(string, output, index+1);

      strcpy(string, output);
      free(output);

      compressAsciiValueAfterIndex(string, index, asciiVal, size);
    }
}

void addWithIndex(char *string, char *output, char *inject, int index) {

    strncpy(output, string, index);
    output[index] = '\0';
    strcat(output, inject);
    strcat(output, string+index);

}

int handleHTMLConversion(char **string, int index) {
    char *br = "<br>";
    char *p = "<p>";

    if ((*string)[index + 1] == 10 || (*string)[index + 1] == 13) {
      char *output = calloc(strlen(*string)+strlen(p)+1, sizeof(char));
      int arr[2] = {10, 13};

      compressAsciiValueAfterIndex(*string, index-1, arr, 2);
      addWithIndex(*string, output, p, index);
      free(*string);
      *string = output;
      output = NULL;

      index += (strlen(p)-1);

    } else {
      char *output = calloc(strlen(*string), sizeof(char));
      removeWithIndex(*string, output, index);
      strcpy(*string, output);
      free(output);

      output = calloc(strlen(*string)+strlen(br)+1, sizeof(char));
      addWithIndex(*string, output, br, index);
      free(*string);
      *string = output;
      output = NULL;

      index += (strlen(br)-1);
    }
    return index;
}

void processStrings(dataHeader *header) {
    dataString *node = header->next;

    while (node != NULL) {
        int i = 0;
        int asciiArr[1];
        header->length -= strlen(node->string);
        while (node->string[i] != '\0') {
            switch(node->string[i]) {

                case 32:
                    asciiArr[0] = 32;
                    compressAsciiValueAfterIndex(node->string, i, asciiArr, 1);
                    break;
                case 9:
                    asciiArr[0] = 9;
                    compressAsciiValueAfterIndex(node->string, i, asciiArr, 1);
                    break;
                case 10:
                case 13:
                    i = handleHTMLConversion(&node->string, i);
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

void writeStrings(char *filename, struct dataHeader * header) {
  
}
