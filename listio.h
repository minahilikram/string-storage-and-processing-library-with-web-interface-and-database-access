#define SUCCESS 1
#define FAILURE 0

/* Structure defines a return value to indicate the success or failure of a function. */
struct returnStruct {
    int value;
    struct dataHeader *header;
};

/* Structure defines the head of the data.
It contains a data name, length of all the data strings combined and a pointer to the first dataString structure. */
struct dataHeader {
	char *name;
    int length;
    struct dataString *next;
};

/* Structure to contain a string and a pointer to the next dataString structure. */
struct dataString {
    char *string;
    struct dataString *next;
};

/* Preconditions: None.
Postconditions: Returns a pointer to a struct of type dataHeader.
It dynamically allocates the structure and sets the initial values of the variables. */
struct returnStruct *buildHeader();

/* Preconditions: An initalized dataHeader is available. A pointer to a string is avaialbe.
Postconditions: Allocates and sets the name string in a dataHeader structure. */
int setName(struct dataHeader *header, char *name);

/* Preconditions: An initalized dataHeader is available.
Postconditions: Returns a pointer to the name string in the dataHeader structure. */
char *getName(struct dataHeader *header);

/* Preconditions: An initalized dataHeader is available.
Postconditions: Returns the integer value stored in the length variable in the dataHeader structure. */
int getLength(struct dataHeader *header);

/* Preconditions: An initalized dataHeader is available. A pointer to a string is avaialbe.
Postconditions: Adds a new dataString to the linked structure.
The new string is stored in the string pointer within the dataString struct. */
int addString(struct dataHeader *header, char *str);

/* Preconditions: An initalized dataHeader is available.
Postconditions: This function prints out all of the strings stored in the list. */
int printString(struct dataHeader *header);

/* Preconditions: An initalized dataHeader is available.
Postconditions: This function modifies the characters space, tab, newline, carriage return contained within the strings. */
int processStrings(struct dataHeader *header);

/* Preconditions: An initalized dataHeader is available.
Postconditions: Free() all of the dataString structures and the dataHeader structure. */
int freeStructure(struct dataHeader *header);

/* Preconditions: An initalized dataHeader is available. A pointer to a string containing the filename is avaialbe.
Postconditions: Writes all of the contents of the dataHeader and linked structure to a binary file. */
int writeStrings(char *filename, struct dataHeader *header);

/* Preconditions:  A pointer to a string containing the filename is avaialbe.
Postconditions: This function reads the linked structure from a file created using the writeStrings() function
and rebuilds the linked structure that is stored in the file. A pointer to the dataHeader structure is returned. */
struct returnStruct *readStrings(char *filename);
