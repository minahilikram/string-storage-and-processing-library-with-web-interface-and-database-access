/*********************
Minahil Ikram 0721370
CIS*2750 Assignment 1
September 23rd, 2016
*********************/


#include <stdio.h>
#include "listio.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct dataHeader *buildHeader() {
    struct dataHeader *header;
    header = malloc(sizeof(struct dataHeader));
    header->next = NULL;
    header->name = NULL;
    header->length = 0;

    return header;
}

void setName(struct dataHeader *header, char *name) {
    header->name = NULL;
    header->name = malloc(strlen(name)+1);
    strcpy(header->name, name);
}

char *getName(struct dataHeader *header) {
    return header->name;
}

int getLength(struct dataHeader *header) {
    return header->length;
}

void addString(struct dataHeader *header, char *str) {
   struct dataString *string;
   string = malloc(sizeof(struct dataString));

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
        struct dataString *current;
        current = header->next;
        /*TODO: maybe move to getTail function*/
        while (current->next != NULL) {
           current = current->next;
         }
        current->next = string;
    }
}

void printString(struct dataHeader *header) {
    struct dataString *string;
    string = header->next;
    while (string != NULL) {
        printf("%s\n", string->string);
        string = string->next;
    }
}


/*** Helper functions for processStrings. ***/

/* Checks whether an arr[] contains the val. Returns a boolean, true if value exists in array or false if it does not. */
bool isValInArray(int val, int arr[], int size){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return true;
    }
    return false;
}

/* Removes a character from string at a specified index. Result is contained in the output stirng. */
void removeWithIndex(char *input, char *output, int index) {
  strncpy(output, input, index);
  strncpy(((char*)output)+index,((char*)input)+index+1,strlen(input)-1-index);
}

/* Removes all the subsequent occurances of any of the given characters, arr[], after the index provided. */
void compressAsciiValueAfterIndex(char *string, int index, int asciiVal[], int size) {
    if (isValInArray(string[index+1], asciiVal, size)) {
      char *output = calloc(strlen(string), sizeof(char));
      removeWithIndex(string, output, index+1);

      strcpy(string, output);
      free(output);

      compressAsciiValueAfterIndex(string, index, asciiVal, size);
    }
}

/* Adds a specific string to the original string at a specified index. Result is in the output string. */
void addWithIndex(char *string, char *output, char *inject, int index) {
    strncpy(output, string, index);
    output[index] = '\0';
    strcat(output, inject);
    strcat(output, string+index);
}

/* Changes newlines/carraige returns to HTML tags. */
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

void processStrings(struct dataHeader *header) {
    struct dataString *node = header->next;

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


/*** Helper free function ***/

/* Frees allt he dataStrings after and including the one passed in. */
void freeDataStrings (struct dataString *string) {
   if (string->next != NULL)
      freeDataStrings(string->next);

   free(string->string);
   free(string);
}

void freeStructure(struct dataHeader *header) {
    if (header == NULL)
      return;

    if (header->next != NULL)
      freeDataStrings(header->next);

    free(header->name);
    free(header);
}


/*** Helper writeStrings functions ***/

/* Writes an int to the file provided. */
void writeInt(FILE *fp, int integer[]) {
    fwrite(integer, sizeof(int), 1, fp);
}

/* Writes an string to the file provided. */
void writeString(FILE *fp, char *string) {
    int i[1];
    i[0] = strlen(string)+1;

    writeInt(fp, i);
    fwrite(string, sizeof(char), strlen(string)+1, fp);
}

void writeStrings(char *filename, struct dataHeader *header) {
  FILE *fp = fopen(filename, "w");
  struct dataString *node = header->next;

  writeString(fp, header->name);
  writeInt(fp, &(header->length));

  while (node != NULL) {
    writeString(fp, node->string);
    node = node->next;
  }

  fclose(fp);
}


/*** Helper readStrings functions ***/

/* Reads an string from the file provided. */
int readInt(FILE *fp) {
    int buffer, check = 0;
    check = fread(&buffer, sizeof(int), 1, fp);
    if (check == 0)
      return -1;
    return buffer;
}

/* Reads an string from the file provided. */
char* readString(FILE *fp, int size) {
    char *buffer = NULL;
    buffer = malloc(size);
    fread(buffer, sizeof(char), size, fp);
    return buffer;
}

struct dataHeader *readStrings(char *filename) {
  struct dataHeader *header;
  FILE *fp = fopen(filename , "r");

  if(fp == NULL) {
    fclose(fp);
    return NULL;
  }

  header = buildHeader();
  header->name = readString(fp, readInt(fp));
  header->length = readInt(fp);
  header->next = NULL;

  while (true) {
    char *string;
    int length = readInt(fp);
    if (length == -1)
      break;

    string = readString(fp, length);
    addString(header, string);
    free(string);
  }

  fclose(fp);

  return header;
}
