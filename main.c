#define _GNU_SOURCE
#include <python2.7/Python.h>
#include "listio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createHTMLFile(char *filename) {
		struct returnStruct *myStruct;
		struct dataString *node;
		FILE *fp, *fpQ;
		char *line = NULL;
		size_t len = 0;
		PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;

		fp = fopen(filename, "r");
		if (fp == NULL) {
				printf("could not find %s\n", filename);
				fclose(fp);
				return;
		}

		myStruct = buildHeader();

		if (myStruct->value == FAILURE) {
				printf("buildHeader() failed, exiting.\n");
				return;
		}

		if (setName(myStruct->header, filename) == FAILURE) {
				printf("setName() failed, exiting.\n");
				return;
		}

		while ((getline(&line, &len, fp)) != -1) {
				addString(myStruct->header, line);
		}

		processStrings(myStruct->header);

		fpQ = fopen("./q2", "w");
		node = myStruct->header->next;
		while (node != NULL) {
				fprintf(fpQ, "%s\n", node->string);
				node = node->next;
		}
		fclose(fpQ);

    Py_Initialize();
    pName = PyString_FromString("processStrings");
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "readQueue");
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
						pArgs = PyTuple_New(1);
						pValue = PyString_FromString("inputfile");
						PyTuple_SetItem(pArgs, 0, pValue);
            PyObject_CallObject(pFunc, pArgs);
						Py_DECREF(pArgs);
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"\n");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"\n");
        return;
    }
    Py_Finalize();

		fpQ = fopen("inputfile.temp", "r");
		while ((getline(&line, &len, fpQ)) != -1) {
				printf("%s\n", line);
		}
		fclose(fpQ);

		fclose(fp);
		if (line) {
				free(line);
		}

		freeStructure(myStruct->header);
		free(myStruct);
}

int main(int argc, char *argv[]) {
		FILE *fp;
		char *line = NULL;
		size_t len = 0;
		char *filenameHTML;

		if (argc != 2) {
				printf("usage: %s filename\n", argv[0]);
				return 0;
		}

		filenameHTML = calloc(strlen(argv[1]) + (strlen(".html")) + 1, sizeof(char));
		strcpy(filenameHTML, argv[1]);
		strncat(filenameHTML, ".html", strlen(".html"));

		fp = fopen(filenameHTML, "r");
		if (fp == NULL) {
				printf("filename %s does not exist, creating %s\n", filenameHTML, filenameHTML);
				createHTMLFile(argv[1]);
		} else {
				while ((getline(&line, &len, fp)) != -1) {
						printf("%s\n", line);
				}
				fclose(fp);

				if (line) {
						free(line);
				}
		}

    return 0;
}
