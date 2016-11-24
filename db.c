#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "mikram"
#define PASSWORD  "0721370"
#define DATABASE  "mikram"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

void show();
void reset();
void clear();
void help();

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "-clear") == 0) {
            clear();
            return 0;
        } else if (strcmp(argv[1], "-reset") == 0) {
            reset();
            return 0;
        } else if (strcmp(argv[1], "-show") == 0) {
            show();
            return 0;
        } else if ((strcmp(argv[1], "-help") == 0)) {
            help(argv[0]);
            return 0;
        }
    }
    else if (argc != 2) {
        help(argv[0]);
    }

    return 0;
}

void help(char *argv0) {
    printf("NAME: \n \t %s - a small C program to manage the database \n\n\n", argv0);
    printf("OPTIONS: \n \t -clear \n \t \t Removes all of the rows from the tables in the database.\n \t -reset \n \t \t Deletes the tables from the database. \n \t -show \n \t \t Prints out the name, length, and date for all files stored in the database. \n \t -help \n \t \t Prints out a usage message.\n");
}

void clear() {
    MYSQL mysql;
    char *query = NULL;

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
    }

    query = calloc(strlen("TRUNCATE TABLE htmlpages; ") + 1, sizeof(char));
  	strcpy(query, "TRUNCATE TABLE htmlpages; ");
  	if(mysql_query(&mysql, query)) {
        printf("Failed to remove all of the rows from table htmlpages.\n%s\n", mysql_error(&mysql));
  	} else {
        printf("Successfully removed all of the rows from table htmlpages.\n%s\n", mysql_error(&mysql));
    }

    free(query);
  	mysql_close(&mysql);
  	mysql_library_end();
}

void reset() {
    MYSQL mysql;
    char *query = NULL;

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
    }

    query = calloc(strlen("drop table htmlpages; ") + 1, sizeof(char));
  	strcpy(query, "drop table htmlpages; ");
  	if(mysql_query(&mysql, query)) {
        printf("Failed to drop table htmlpages.\n%s\n", mysql_error(&mysql));
  	} else {
        printf("Successfully dropped table htmlpages.\n%s\n", mysql_error(&mysql));
    }

    free(query);
  	mysql_close(&mysql);
  	mysql_library_end();
}

void show() {
    MYSQL mysql;
    char *query = NULL;
    MYSQL_RES *res = NULL;
  	MYSQL_ROW row;
    int i = 0;

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
    }

    query = calloc(strlen("select name, length, date from htmlpages where exists (select name, length, date from htmlpages); ") + 1, sizeof(char));
  	strcpy(query, "select name, length, date from htmlpages where exists (select name, length, date from htmlpages); ");
    if(mysql_query(&mysql, query)) {
        printf("%s\n", mysql_error(&mysql));
  	} else {
        res = mysql_store_result(&mysql);

        if (mysql_num_rows(res) == 0) {
            printf("Table htmlpages is empty.\n");
        } else {
            printf("The following is the name, length, and date for all files stored in the database.\n");
        }

        while ((row = mysql_fetch_row(res))) {
            for (i = 0; i < mysql_num_fields(res); i++){
                 printf("%s ", row[i]);
            }
            printf("\n");
        }

        mysql_free_result(res);
    }

    free(query);
  	mysql_close(&mysql);
  	mysql_library_end();
}
