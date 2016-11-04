#define _GNU_SOURCE
#include <python2.7/Python.h>
#include "listio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct returnStruct* textFileToStruct(FILE *fp, char *headerName);
void writeFile(struct returnStruct *myStruct, char *filename);

int main(int argc, char *argv[]) {
		FILE *fp, *read;
		char *line = NULL;
		size_t len = 0;
		char *filenameHTML, *filename;
		struct returnStruct *structHTML;

		if (argc != 2) {
				printf("usage: %s <filename>\n", argv[0]);
				return 0;
		}

		filenameHTML = calloc(strlen(argv[1]) + (strlen(".html")) + 1, sizeof(char));
		strcpy(filenameHTML, argv[1]);
		strncat(filenameHTML, ".html", strlen(".html"));

		fp = fopen(filenameHTML, "r");
		if (fp == NULL) {
				printf("filename %s does not exist, creating %s\n\n", filenameHTML, filenameHTML);

				read = fopen(argv[1], "r");
				if (read == NULL) {
						printf("could not find: %s\n", argv[1]);
						fclose(read);
						return 0;
				}

				structHTML = textFileToStruct(read, argv[1]);

				fclose(read);

				if (structHTML == NULL) {
						printf("textFileToStruct() failed, exiting.\n");
						freeStructure(structHTML->header);
						free(structHTML);
						return 0;
				}

				if (processStrings(structHTML->header) == FAILURE) {
						printf("processStrings() failed, exiting.\n");
						freeStructure(structHTML->header);
						free(structHTML);
						return 0;
				}

				filename = malloc(strlen("./q2")+1);
				strcpy(filename, "./q2");
				writeFile(structHTML, filename);
				free(filename);

				freeStructure(structHTML->header);
				free(structHTML);

				filename = calloc(strlen("./a3.py ") + (strlen(argv[1])) + (strlen(" &")) + 1, sizeof(char));
				strcpy(filename, "./a3.py ");
				strncat(filename, argv[1], strlen(argv[1]));
				strncat(filename, " &", strlen(" &"));

				system(filename);

				mkfifo("./q1", 0777);
				read = fopen("./q1", "r");
				if (read == NULL) {
						printf("could not find: ./q1\n");
						fclose(read);
						return 0;
				}

				structHTML = textFileToStruct(read, argv[1]);

				if (structHTML == NULL) {
						printf("textFileToStruct() failed, exiting.\n");
						freeStructure(structHTML->header);
						free(structHTML);
						return 0;
				}

				fclose(read);
				remove("./q1");

				writeFile(structHTML, filenameHTML);

				freeStructure(structHTML->header);
				free(structHTML);
				free(filename);

				read = fopen(filenameHTML, "r");
				while ((getline(&line, &len, read)) != -1) {
						printf("%s\n", line);
				}
				fclose(read);

				if (line) {
						free(line);
				}
		} else {
				printf("printing file %s.html.\n\n", argv[1]);
				while ((getline(&line, &len, fp)) != -1) {
						printf("%s\n", line);
				}

				fclose(fp);

				if (line) {
						free(line);
				}
		}

		free(filenameHTML);

    return 0;
}

struct returnStruct* textFileToStruct(FILE *fp, char *headerName) {
		struct returnStruct *myStruct;
		char *line = NULL;
		size_t len = 0;

		myStruct = buildHeader();

		if (myStruct->value == FAILURE) {
				printf("buildHeader() failed, exiting.\n");
				freeStructure(myStruct->header);
				free(myStruct);
				return NULL;
		}

		if (setName(myStruct->header, headerName) == FAILURE) {
				printf("setName() failed, exiting.\n");
				freeStructure(myStruct->header);
				free(myStruct);
				return NULL;
		}

		while ((getdelim(&line, &len, '\0', fp)) != -1) {
				if (addString(myStruct->header, line) == FAILURE) {
					printf("addString() failed, exiting.\n");
					freeStructure(myStruct->header);
					free(myStruct);
					return NULL;
				}
		}

		if (line) {
				free(line);
		}

		return myStruct;
}

void writeFile(struct returnStruct *myStruct, char *filename) {
		FILE *fp;
		struct dataString *node;

		fp = fopen(filename, "w");
		node = myStruct->header->next;
		while (node != NULL) {
				fprintf(fp, "%s", node->string);
				node = node->next;
		}
		fclose(fp);
}
