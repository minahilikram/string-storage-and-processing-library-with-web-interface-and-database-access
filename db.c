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

void show();
void reset();
void clear();

int main(int argc, char *argv[]) {
    if (strcmp(argv[1], "-clear") == 0) {
        clear();
        return 0;
    } else if (strcmp(argv[1], "-reset") == 0) {
        reset();
        return 0;
    } else if (strcmp(argv[1], "-show") == 0) {
        show();
        return 0;
    } else if ((strcmp(argv[1], "-help") == 0) || (argc != 2)) {
        printf("NAME: \n \t %s - a small C program to manage the database \n\n\n", argv[0]);
    		printf("OPTIONS: \n \t -clear \n \t \t Removes all of the rows from the tables in the database.\n \t -reset \n \t \t Deletes the tables from the database. \n \t -show \n \t \t Prints out the name, length, and date for all files stored in the database. \n \t -help \n \t \t Prints out a usage message.\n");

        return 0;
    }

    return 0;
}

void clear() {
    MYSQL mysql;
    char query[MAX_QUERY];

    printf("connecting...\n");

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
    }

    query[0] = '\0';
  	strcpy(query, "TRUNCATE TABLE htmlpages; ");

  	if(mysql_query(&mysql, query)) {
        printf("Fail clear.\n%s\n", mysql_error(&mysql));
  	}

  	mysql_close(&mysql);
  	mysql_library_end();
}

void reset() {
    MYSQL mysql;
    char query[MAX_QUERY];

    printf("connecting...\n");

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
    }

    query[0] = '\0';
  	strcpy(query, "drop table htmlpages; ");

  	if(mysql_query(&mysql, query)) {
        printf("Fail reset.\n%s\n", mysql_error(&mysql));
  	}

  	mysql_close(&mysql);
  	mysql_library_end();
}

void show() {
    MYSQL mysql;
    char query[MAX_QUERY];
    MYSQL_RES *res;
  	MYSQL_ROW row;
    int i = 0;

    printf("connecting...\n");

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
    }

    query[0] = '\0';
  	strcpy(query, "select name, length, date from htmlpages; ");
    if(mysql_query(&mysql, query)) {
        printf("%s\n", mysql_error(&mysql));
  	}

    if(!mysql_field_count(&mysql) == 0) {
      if (!(res = mysql_store_result(&mysql))) {
          printf("Failed to store rows. \n%s\n", mysql_error(&mysql));
      }

      while ((row = mysql_fetch_row(res))) {
      		for (i = 0; i < mysql_num_fields(res); i++){
      			   printf("%s ", row[i]);
      		}
      		printf("\n");
    	}
    }

    mysql_free_result(res);
  	mysql_close(&mysql);
  	mysql_library_end();
}
