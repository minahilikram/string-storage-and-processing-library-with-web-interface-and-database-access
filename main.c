#include <python2.7/Python.h>
#include "listio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql/mysql.h>

struct returnStruct* textFileToStruct(FILE *fp, char *headerName);
void writeFile(struct returnStruct *myStruct, char *filename);
void convert(char *argv1);
void store(char *argv1);
char *convertFilenameToHTML(char *filename);
char *removeFilenameExtension(char *filenameHTML);

int main(int argc, char *argv[]) {
		if (argc == 2) {
				convert(argv[1]);
				return 0;
		} else if (argc == 3) {
				FILE *fp;
				bool isInFolder = false;
				char *filenameHTML = convertFilenameToHTML(argv[1]);

				fp = fopen(filenameHTML, "r");
				if (fp != NULL) {
						isInFolder = true;
				}

				convert(argv[1]);
				store(filenameHTML);

				if (!isInFolder) {
						remove(filenameHTML);
				}
				free(filenameHTML);
				return 0;
		} else {
				printf("usage: %s <filename>\n", argv[0]);
				printf("usage: %s <filename> store\n", argv[0]);
				return 0;
		}
}

char *convertFilenameToHTML(char *filename) {
			char *filenameHTML = NULL;
			char *name = removeFilenameExtension(filename);

			filenameHTML = calloc(strlen(name) + (strlen(".html")) + 1, sizeof(char));
			strcpy(filenameHTML, name);
			strncat(filenameHTML, ".html", strlen(".html"));

			free(name);
			return (filenameHTML);
}

char *removeFilenameExtension(char *name) {
			char *filename = NULL;
			char *lastdot = NULL;

			filename = calloc((strlen(name) + 1), sizeof(char));
			strcpy(filename, name);
			lastdot = strrchr(filename, '.');
			if (lastdot != NULL) {
					*lastdot = '\0';
			}

			return (filename);
}

void store(char *filenameHTML) {
		MYSQL mysql;
		FILE *read;
		char *query = NULL;
		char *createQuery = "create table IF NOT EXISTS htmlpages (html longtext not null, length int not null, name VARCHAR(255) not null PRIMARY KEY, date DATETIME not null ); ";
		char *buffer = NULL;
		char *len = NULL;
		char *filename = NULL;
		char *escapeFix = NULL;
		long length;

		printf("connecting...\n");

		mysql_init(&mysql);
		mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
		if (!mysql_real_connect(&mysql, "dursley.socs.uoguelph.ca", "mikram", "0721370", "mikram", 0, NULL, 0)) {
				printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
		}

		query = calloc(strlen(createQuery) + 1, sizeof(char));
		strcpy(query, createQuery);
		if(mysql_query(&mysql, query)) {
				printf("Could not create table htmlpages.\n%s\n", mysql_error(&mysql));
		}

		read = fopen(filenameHTML, "r");
		if (read) {
			  fseek(read, 0, SEEK_END);
			  length = ftell(read);
				len = malloc(length + 1);
				sprintf(len, "%ld", length);
			  fseek(read, 0, SEEK_SET);
			  buffer = calloc((length + 1), sizeof(char));
			  if(buffer) {
			    	fread(buffer, 1, length, read);
			  }
			  fclose(read);
		}

		filename = removeFilenameExtension(filenameHTML);

		if (buffer) {

				escapeFix = calloc((strlen(buffer) * 2) + 1, sizeof(char));
				mysql_real_escape_string(&mysql, escapeFix, buffer, (strlen(buffer)));

				free(query);
				query = calloc(strlen("insert into htmlpages values ('") + strlen(escapeFix) + strlen("', ") + strlen(len) + strlen(", '") + strlen(filename) + strlen("', now() ); ")  + 1, sizeof(char));
				strcpy(query, "insert into htmlpages values ('");
				strcat(query, escapeFix);
				strcat(query, "', ");
				strcat(query, len);
				strcat(query, ", '");
				strcat(query, filename);
				strcat(query, "', now() ); ");
		}
		printf("Saving %s to database...\n", filename);
		if(mysql_query(&mysql, query)) {
				printf("Failed to save %s into htmlpages. \n%s\n", filenameHTML, mysql_error(&mysql));
		}

		mysql_close(&mysql);
		mysql_library_end();

		free(filename);
		free(buffer);
		free(len);
		free(query);
		free(escapeFix);
}

void convert(char *argv1) {
		FILE *fp, *read;
		char *line = NULL;
		size_t len = 0;
		char *filenameHTML, *filename;
		struct returnStruct *structHTML;

		filenameHTML = convertFilenameToHTML(argv1);

		fp = fopen(filenameHTML, "r");
		if (fp == NULL) {
				printf("filename %s does not exist, creating %s\n\n", filenameHTML, filenameHTML);

				read = fopen(argv1, "r");
				if (read == NULL) {
						printf("could not find: %s\n", argv1);
						return;
				}

				structHTML = textFileToStruct(read, argv1);

				fclose(read);

				if (structHTML == NULL) {
						printf("textFileToStruct() failed, exiting.\n");
						freeStructure(structHTML->header);
						free(structHTML);
						return;
				}

				if (processStrings(structHTML->header) == FAILURE) {
						printf("processStrings() failed, exiting.\n");
						freeStructure(structHTML->header);
						free(structHTML);
						return;
				}

				filename = malloc(strlen("./q2")+1);
				strcpy(filename, "./q2");
				writeFile(structHTML, filename);
				free(filename);

				freeStructure(structHTML->header);
				free(structHTML);

				filename = calloc(strlen("./a4.py ") + (strlen(argv1)) + (strlen(" &")) + 1, sizeof(char));
				strcpy(filename, "./a4.py ");
				strncat(filename, argv1, strlen(argv1));
				strncat(filename, " &", strlen(" &"));

				system(filename);

				mkfifo("./q1", 0777);
				read = fopen("./q1", "r");
				if (read == NULL) {
						printf("could not find: ./q1\n");
						return;
				}

				structHTML = textFileToStruct(read, argv1);

				if (structHTML == NULL) {
						printf("textFileToStruct() failed, exiting.\n");
						freeStructure(structHTML->header);
						free(structHTML);
						return;
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
				printf("printing file %s.html.\n\n", argv1);
						while ((getline(&line, &len, fp)) != -1) {
								printf("%s\n", line);
						}

				fclose(fp);

				if (line) {
						free(line);
				}
		}

		free(filenameHTML);
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
