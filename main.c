#define _GNU_SOURCE
#define MAX_QUERY 5120
#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "mikram"
#define PASSWORD  "0721370"
#define DATABASE  "mikram"

#include <python2.7/Python.h>
#include "listio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

struct returnStruct* textFileToStruct(FILE *fp, char *headerName);
void writeFile(struct returnStruct *myStruct, char *filename);
int convert(char *argv1);
int store(char *argv1);

int main(int argc, char *argv[]) {
		if (argc == 2) {
				convert(argv[1]);
				return 0;
		} else if (argc == 3) {
				convert(argv[1]);
				store(argv[1]);
				return 0;
		} else {
				printf("usage: %s <filename>\n", argv[0]);
				printf("usage: %s <filename> store\n", argv[0]);
				return 0;
		}
}

int store(char *argv1) {
		MYSQL mysql;
		FILE *read;
		char query[MAX_QUERY];

		char *filenameHTML, *len;
		char *buffer = NULL;
		long length;

		filenameHTML = calloc(strlen(argv1) + (strlen(".html")) + 1, sizeof(char));
		strcpy(filenameHTML, argv1);
		strncat(filenameHTML, ".html", strlen(".html"));

		printf("connecting...\n");

		mysql_init(&mysql);
		mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
		if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
				printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
		}

		query[0] = '\0';
		strcat(query, "create table IF NOT EXISTS htmlpages (html text not null,");
		strcat(query, "length int not null,");
		strcat(query, "name VARCHAR(255) not null PRIMARY KEY,");
		strcat(query, "date DATETIME not null )");
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

		if (buffer) {
				query[0] = '\0';

				strcat(query, "insert into htmlpages values ('");
				strcat(query, buffer);
				strcat(query, "', ");
				strcat(query, len);
				strcat(query, ", '");
				strcat(query, argv1);
				strcat(query, "', now() ); ");
		}
		if(mysql_query(&mysql, query)) {
				printf("Failed to save %s into htmlpages. \n%s\n", filenameHTML, mysql_error(&mysql));
		}

		mysql_close(&mysql);
		mysql_library_end();

		free(filenameHTML);
		free(buffer);
		free(len);

		return 0;
}

int convert(char *argv1) {
		FILE *fp, *read;
		char *line = NULL;
		size_t len = 0;
		char *filenameHTML, *filename;
		struct returnStruct *structHTML;

		filenameHTML = calloc(strlen(argv1) + (strlen(".html")) + 1, sizeof(char));
		strcpy(filenameHTML, argv1);
		strncat(filenameHTML, ".html", strlen(".html"));

		fp = fopen(filenameHTML, "r");
		if (fp == NULL) {
				printf("filename %s does not exist, creating %s\n\n", filenameHTML, filenameHTML);

				read = fopen(argv1, "r");
				if (read == NULL) {
						printf("could not find: %s\n", argv1);
						fclose(read);
						return 0;
				}

				structHTML = textFileToStruct(read, argv1);

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

				filename = calloc(strlen("./a3.py ") + (strlen(argv1)) + (strlen(" &")) + 1, sizeof(char));
				strcpy(filename, "./a3.py ");
				strncat(filename, argv1, strlen(argv1));
				strncat(filename, " &", strlen(" &"));

				system(filename);

				mkfifo("./q1", 0777);
				read = fopen("./q1", "r");
				if (read == NULL) {
						printf("could not find: ./q1\n");
						fclose(read);
						return 0;
				}

				structHTML = textFileToStruct(read, argv1);

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
