#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "mikram"
#define PASSWORD  "0721370"
#define DATABASE  "mikram"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

int main(int argc, char *argv[]) {
    char *filename = NULL;
    char *lastdot = NULL;
    MYSQL mysql;
    char *query = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    int i = 0;
    char *filenameHTML = NULL;

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
    }

    if (argc == 2) {
        filename = calloc((strlen(argv[1]) + 1), sizeof(char));
        strcpy (filename, argv[1]);
        lastdot = strrchr(filename, '.');
        if (lastdot != NULL) {
            *lastdot = '\0';
        }

        query = calloc(strlen("select html from htmlpages where name='") + strlen(filename) + strlen("'; ") + 1, sizeof(char));
        strcpy(query, "select html from htmlpages where name='");
        strcat(query, filename);
        strcat(query, "'; ");
        if(mysql_query(&mysql, query)) {
            printf("Could not find file %s in the database. \n %s\n", filename, mysql_error(&mysql));
        } else {
            res = mysql_store_result(&mysql);

            if (mysql_num_rows(res) != 0) {
                while ((row = mysql_fetch_row(res))) {
                    for (i = 0; i < mysql_num_fields(res); i++){
                         printf("%s", row[i]);
                    }
                    printf("\n");
                }
            }

            mysql_free_result(res);
        }
    }
    else {
        query = calloc(strlen("select name from htmlpages where exists (select name from htmlpages); ") + 1, sizeof(char));
        strcpy(query, "select name from htmlpages where exists (select name from htmlpages); ");
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        } else {
            res = mysql_store_result(&mysql);

            if (mysql_num_rows(res) != 0) {
                while ((row = mysql_fetch_row(res))) {
                    for (i = 0; i < mysql_num_fields(res); i++){
                         filenameHTML = calloc(strlen(row[i]) + (strlen(".html")) + 1, sizeof(char));
                         strcpy(filenameHTML, row[i]);
                         strncat(filenameHTML, ".html", strlen(".html"));
                         printf("%s ", filenameHTML);
                    }
                    printf("\n");
                }
            }

            mysql_free_result(res);
        }
    }

    free(query);
    free(filename);
    free(filenameHTML);
    mysql_close(&mysql);
    mysql_library_end();

    return 0;
}
