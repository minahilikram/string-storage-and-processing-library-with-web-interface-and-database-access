typedef struct dataHeader {
	char *name;
    int length;
    struct dataString *next;
} dataHeader;

typedef struct dataString {
    char *string;
    struct dataString *next;
} dataString;

/* Preconditions: None.
Postconditions: Returns a pointer to a struct of type dataHeader. It dynamically allocates the structure and sets the initial values of the variables. */
dataHeader *buildHeader();


/* Preconditions: An initalized dataHeader is available. The name as the parameter is of correct type.
Postconditions: Allocates and sets the name string in a dataHeader structure. */
void setName(dataHeader *header, char *name);


/* Preconditions: An initalized dataHeader is available.
Postconditions: Returns a pointer to the name string in the dataHeader structure. */
char *getName(dataHeader *header);


/* Preconditions: An initalized dataHeader is available.
Postconditions: Returns the integer value stored in the length variable in the dataHeader structure. */
int getLength(dataHeader *header);


/* Preconditions: An initalized dataHeader is available. The name as the parameter is of correct type.
Postconditions: Adds a new dataString to the linked structure. The new string is stored in the string pointer within the dataString struct. */
void addString(dataHeader *header, char *str);


/* Preconditions: An initalized dataHeader is available.
Postconditions: This function prints out all of the strings stored in the list. */
void printString(dataHeader *header);


/* Preconditions: An initalized dataHeader is available.
Postconditions: This function prints out all of the strings stored in the list. */
void processStrings(dataHeader *header);


/* Preconditions: An initalized dataHeader is available.
Postconditions: Free() all of the dataString structures and the dataHeader structure. */
void freeStructure(dataHeader *header);

void writeStrings(char *filename, struct dataHeader * header);
