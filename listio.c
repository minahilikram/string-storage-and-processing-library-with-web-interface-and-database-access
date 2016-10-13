#include <stdio.h>
#include "listio.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct returnStruct *buildHeader() {
    struct returnStruct *rtn;
    struct dataHeader *header;

    rtn = malloc(sizeof(struct returnStruct));

    if (rtn == NULL) {
        return NULL;
    }

    header = malloc(sizeof(struct dataHeader));

    if (header == NULL) {
        rtn->value = FAILURE;
        rtn->header = NULL;
    } else {
        rtn->value = SUCCESS;
        rtn->header = header;

        header->next = NULL;
        header->name = NULL;
        header->length = 0;
    }

    return rtn;
}

int setName(struct dataHeader *header, char *name) {
    if (name == NULL) {
        return FAILURE;
    }

    header->name = NULL;
    header->name = malloc(strlen(name)+1);

    if (header->name == NULL) {
        return FAILURE;
    }

    strcpy(header->name, name);

    return SUCCESS;
}

char *getName(struct dataHeader *header) {
    if (header->name == NULL || (header->name)[0] == '\0') {
        return NULL;
    }
    return header->name;
}

int getLength(struct dataHeader *header) {
    if (header->length == 0) {
      return FAILURE;
    }
    return header->length;
}

int addString(struct dataHeader *header, char *str) {
    struct dataString *string;

    if (str == NULL) {
        return FAILURE;
    }

    string = malloc(sizeof(struct dataString));

    if (string == NULL) {
        return FAILURE;
    }

    string->string = NULL;
    string->string = malloc(strlen(str)+1);

    if (string->string == NULL) {
        return FAILURE;
    }

    strcpy(string->string, str);

    string->next = NULL;
    header->length = header->length + strlen(str) + 1;
    if (header->next == NULL) {
        header->next = string;
    } else {
        struct dataString *current;
        current = header->next;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = string;
    }

    return SUCCESS;
}

int printString(struct dataHeader *header) {
    struct dataString *string;

    if (header == NULL || header->next == NULL) {
        return FAILURE;
    }

    string = header->next;

    while (string != NULL) {
        printf("%s\n", string->string);
        string = string->next;
    }
    return SUCCESS;
}


/*** Helper functions for processStrings. ***/

/* [3] Checks whether an arr[] contains the val. Returns a boolean, true if value exists in array or false if it does not. */
bool isValInArray(int val, int arr[], int size) {
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val) {
            return true;
        }
    }
    return false;
}

/* [1] Removes a character from string at a specified index. Result is contained in the output stirng. */
void removeWithIndex(char *input, char *output, int index) {
    strncpy(output, input, index);
    strncpy(((char*)output)+index, ((char*)input)+index+1, strlen(input)-1-index);
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

/* [2] Adds a specific string to the original string at a specified index. Result is in the output string. */
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

/* Remove Extra P Tags Extra Functions. */

/* Removes all p tags after given index */
void compressPTagsAfterIndex(char *string, int index) {
    int asciiArr[1];

    if (string[index] == 0) {
        return;
    }

    if (string[index] == 32 || string[index] == 9) {
        compressPTagsAfterIndex(string, ++index);
    }

    if (string[index] == 60 && string[index+1] == 112 && string[index+2] == 62) {
        asciiArr[0] = 60;
        compressAsciiValueAfterIndex(string, index-1, asciiArr, 1);
        asciiArr[0] = 112;
        compressAsciiValueAfterIndex(string, index-1, asciiArr, 1);
        asciiArr[0] = 62;
        compressAsciiValueAfterIndex(string, index-1, asciiArr, 1);
        compressPTagsAfterIndex(string, index);
    }
}

/* Removes all p tags in given dataString. */
void removePTags(struct dataString *prev) {
    int i = 0;

    if (prev == NULL) {
        return;
    }

    while (prev->string[i] != '\0') {
        if (i+5 <= strlen(prev->string)) {
            /* Look to see if p tag exists */
            if (prev->string[i] == 60 && prev->string[i+1] == 112 && prev->string[i+2] == 62) {
                compressPTagsAfterIndex(prev->string, i+3);
            }
        }
        i++;
    }
}

/* Remove p tags between two given nodes. */
void removePTagsBetweenNodes(struct dataString *prev, struct dataString *curr) {
    int i = 0, j = 0;

    if (prev == NULL) {
        return;
    }

    while (curr->string[i] != '\0') {
        if (curr->string[i] == 32 || curr->string[i] == 9) {
            i++;
        } else if (i+2 <= strlen(curr->string) && curr->string[i] == 60 && curr->string[i+1] == 112 && curr->string[i+2] == 62) {
            break;
        } else {
            return;
        }
    }

    j = strlen(prev->string) - 1;
    while (j >= 0) {
        if (prev->string[j] == 32 || prev->string[j] == 9) {
            j--;
        } else if (j-2 >= 0 && prev->string[j] == '>' && prev->string[j-1] == 'p' && prev->string[j-2] == '<') {
            compressPTagsAfterIndex(prev->string, j-2);
            break;
        } else {
            return;
        }
    }
}

int processStrings(struct dataHeader *header) {
    struct dataString *node, *prev = NULL;

    if (header == NULL || header->next == NULL) {
        return FAILURE;
    }

    node = header->next;

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
        /**removePTags(node);**/
        removePTagsBetweenNodes(prev, node);
        header->length += strlen(node->string);
        prev = node;
        node = node->next;

    }

    return SUCCESS;
}


/*** Helper free function ***/

/* Frees all the dataStrings after and including the one passed in. */
void freeDataStrings (struct dataString *string) {
    if (string->next != NULL) {
        freeDataStrings(string->next);
    }

   free(string->string);
   free(string);
}

int freeStructure(struct dataHeader *header) {
    if (header == NULL) {
        return FAILURE;
    }

    if (header->next != NULL) {
        freeDataStrings(header->next);
    }

    free(header->name);
    free(header);

    return SUCCESS;
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

int writeStrings(char *filename, struct dataHeader *header) {
    FILE *fp = fopen(filename, "w");
    struct dataString *node = header->next;

    if (fp == NULL) {
        return FAILURE;
    }

    if (header == NULL || header->name == NULL) {
        fclose(fp);
        return FAILURE;
    }

    writeString(fp, header->name);
    writeInt(fp, &(header->length));

    while (node != NULL) {
        writeString(fp, node->string);
        node = node->next;
    }

    fclose(fp);
    return SUCCESS;
}


/*** Helper readStrings functions ***/

/* Reads an string from the file provided. */
int readInt(FILE *fp) {
    int buffer, check = 0;
    check = fread(&buffer, sizeof(int), 1, fp);

    if (check == 0) {
        return -1;
    }

    return buffer;
}

/* Reads an string from the file provided. */
char* readString(FILE *fp, int size) {
    char *buffer = NULL;
    buffer = malloc(size);
    fread(buffer, sizeof(char), size, fp);
    return buffer;
}

/*TODO: FILE pointer changed to beginning.*/
bool isFileEmpty(FILE *fp) {
    int len;

    if(fp == NULL) {
      return true;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (len == 0) {
        return true;
    }

    return false;
}

struct returnStruct *readStrings(char *filename) {
    struct returnStruct *rtn;
    FILE *fp;
    rtn = buildHeader();

    if (rtn == NULL) {
        return NULL;
    }

    fp = fopen(filename, "r");

    if(fp == NULL) {
        rtn->value = FAILURE;
        rtn->header = NULL;
        return rtn;
    }

    if(isFileEmpty(fp) || rtn->header == NULL) {
        fclose(fp);
        rtn->value = FAILURE;
        rtn->header = NULL;
        return rtn;
    }

    rtn->value = SUCCESS;
    rtn->header->name = readString(fp, readInt(fp));
    rtn->header->length = readInt(fp);
    rtn->header->next = NULL;
    rtn->header->length = 0;

    while (true) {
        char *string;
        int length = readInt(fp);

        if (length == -1) {
            break;
        }

        string = readString(fp, length);
        addString(rtn->header, string);
        free(string);
    }

    fclose(fp);
    return rtn;
}
