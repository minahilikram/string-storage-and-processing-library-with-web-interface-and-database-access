# string-storage-and-processing-library

A system which stores strings in a [linked structure](https://en.wikipedia.org/wiki/Linked_data_structure).

> Contains function(s) necessary to process the string to remove redundant whitespace characters and format it using HTML tags. The strings can be stored and retrieved from binary files.

## Install

```sh
# download file from dropbox
$ unzip a1_minahilikram.zip -d a1_minahilikram
```

## Usage

```sh
$ cd a1_minahilikram/
$ make
# Confirm that .a and .h file are present.
$ ls
```
```c
// Include the header to use the library.
#include "listio.h"
```

## Limitations

- readString() function uses addString(); addString() keeps count of the length value in the dataHeader struct; therefore the length provided by the binary file is not used, it is instead calculated by the addString(); &mdash; if this is not correct; fix would be to keep length provided by the binary file, by keeping a variable to save the length read in from the binary file.
- calling readString() on a non-existing file; results in `Segmentation fault`.
- calling readString() on a wrong file (one that exists); results in ending the program without writing to the provided file.

## References

1. function [removeWithIndex()](http://stackoverflow.com/a/5457657/6175388)
2. function [addWithIndex()](http://stackoverflow.com/a/2016015/6175388)
3. function [isValInArray()](http://stackoverflow.com/a/15102018/6175388)

*References are also referenced in the code comments i.e. [reference #].*

### Developer

Minahil Ikram 0721370
