# string-storage-and-processing-library-with-web-interface-and-database-access

A system which stores strings in a [linked structure](https://en.wikipedia.org/wiki/Linked_data_structure). The program using the library will call a Python program. A [graphical interface](https://en.wikipedia.org/wiki/Graphical_user_interface) which will allow the user to view files, convert text to html files using the C program using the library. Add [MySQL](https://en.wikipedia.org/wiki/MySQL) database access to the program to store and retrieve html pages using the web page and conversion program.

> Contains function(s) necessary to process the string to remove redundant whitespace characters and format it using HTML tags. The strings can be stored and retrieved from binary files. The Python which formats the text as HTML, search for strings in the text and change their font characteristics. A small utility program, db.c, which allows the user to view and manage the database.

## Install

```sh
# download file from dropbox
$ unzip a4_minahilikram-0721370.zip
```

## Usage

```sh
$ make
$ chmod 777 THE_FOLDER_WHERE_CONTENTS_WERE_UNZIPPED/*
$ chmod 777 THE_FOLDER_WHERE_CONTENTS_WERE_UNZIPPED/
# open web browser and visit PATH_THAT_THE_FILE_IS_LOCATED_IN/a4.php
```
- main.c accepts either one or two arguments, the first argument has to be filename, i.e. ./a4 <filename>, this will only convert the file and save it under files/. The second argument can be anything, but will convert and store in DB. If three or more arguments are passed, the program will not do anything and will return with a message to help the user choose what they want to do.
- getfiles.c is a helper program which is solely used for the UI, it retrieves the names of files in the DB if no argument is passed, and if a filename is passed, it will return the html of that file.

## Limitations

- readString() function uses addString(); addString() keeps count of the length value in the dataHeader struct; therefore the length provided by the binary file is not used, it is instead calculated by the addString(); &mdash; if this is not correct; fix would be to keep length provided by the binary file, by keeping a variable to save the length read in from the binary file.
- program does not work if there is no compiled C code with an executable named 'a4'
- program cannot upload/convert files which do not have read/write permissions
- if file is selected after clicking "convert", that filename will stay in stored until another one is clicked, however the action to "convert" will not take place until after "OK" is clicked (same goes for DB Store).
- if you run convert and DB store on the same file, there will be duplicates, one in the DB and one in the files/ folder.
- DB is hardcoded to my credentials provided by the University of Guelph

## References

1. function [listio.c/removeWithIndex()](http://stackoverflow.com/a/5457657/6175388)
2. function [listio.c/addWithIndex()](http://stackoverflow.com/a/2016015/6175388)
3. function [listio.c/isValInArray()](http://stackoverflow.com/a/15102018/6175388)
4. used everywhere to [remove filename extension](http://stackoverflow.com/a/2736841)
5. function [main.c/store](http://stackoverflow.com/a/174552)

### Developer

Minahil Ikram 0721370
