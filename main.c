#define _GNU_SOURCE
#include <python2.7/Python.h>
#include "listio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct returnStruct* textFileToStruct(char *filename, char *headerName);
void writeFile(struct returnStruct *myStruct, char *filename);
struct returnStruct* readQueue();

int main(int argc, char *argv[]) {
		FILE *fp;
		char *line = NULL;
		size_t len = 0;
		char *filenameHTML, *filename;
		struct returnStruct *structHTML;
		char cwd[1024];
		char *PYTHONPATH;

		getcwd(cwd, sizeof(cwd));
		PYTHONPATH = calloc(strlen("PYTHONPATH=") + (strlen(cwd)) + 1, sizeof(char));
		strcpy(PYTHONPATH, "PYTHONPATH=");
		strncat(PYTHONPATH, cwd, strlen(cwd));

		putenv(PYTHONPATH);

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
				structHTML = textFileToStruct(argv[1], argv[1]);

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

				structHTML = readQueue(argv[1]);

				if (structHTML == NULL) {
						printf("readQueue() failed, exiting.\n");
						freeStructure(structHTML->header);
						free(structHTML);
						return 0;
				}

				writeFile(structHTML, filenameHTML);
				remove("./q2");

				freeStructure(structHTML->header);
				free(structHTML);

				fp = fopen(filenameHTML, "r");
				while ((getline(&line, &len, fp)) != -1) {
						printf("%s\n", line);
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
		free(PYTHONPATH);

    return 0;
}

struct returnStruct* textFileToStruct(char *filename, char *headerName) {
		struct returnStruct *myStruct;
		FILE *fp;
		char *line = NULL;
		size_t len = 0;

		fp = fopen(filename, "r");
		if (fp == NULL) {
				printf("could not find %s\n", filename);
				fclose(fp);
				return NULL;
		}

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

		while ((getline(&line, &len, fp)) != -1) {
				if (addString(myStruct->header, line) == FAILURE) {
					printf("addString() failed, exiting.\n");
					freeStructure(myStruct->header);
					free(myStruct);
					return NULL;
				}
		}

		fclose(fp);
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

/* [4] used to add the python */
struct returnStruct* readQueue(char *filename) {
		struct returnStruct *readStruct;
		PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
		char *tempFile;

    Py_Initialize();
    pName = PyString_FromString("processStrings");

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    pFunc = PyObject_GetAttrString(pModule, "rwQueue");
		pArgs = PyTuple_New(1);
		pValue = PyString_FromString(filename);
		PyTuple_SetItem(pArgs, 0, pValue);
    PyObject_CallObject(pFunc, pArgs);

		Py_DECREF(pArgs);
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);

    Py_Finalize();

		tempFile = calloc(strlen(filename) + (strlen(".temp")) + 1, sizeof(char));
		strcpy(tempFile, filename);
		strncat(tempFile, ".temp", strlen(".temp"));

		readStruct = textFileToStruct(tempFile, filename);

		if (readStruct == NULL) {
				printf("textFileToStruct() failed, exiting.\n");
				freeStructure(readStruct->header);
				free(readStruct);
				return 0;
		}

		remove(tempFile);
		free(tempFile);

		return readStruct;
}
