# string-storage-and-processing-library

A system which stores strings in a [linked structure](https://en.wikipedia.org/wiki/Linked_data_structure).

> Contains function(s) necessary to process the string to remove redundant whitespace characters and format it using HTML tags. The strings can be stored and retrieved from binary files.

## Install

```sh
# download file from dropbox
$ unzip a2_minahilikram-0721370.zip -d a2_minahilikram-0721370
```

## Usage

```sh
$ cd a2_minahilikram-0721370/
$ make
$ export PYTHONPATH=`pwd`
$ make ARGS=<filename> ./mainrunnable
# make sure <filename>.info is available.
```

## Limitations

- readString() function uses addString(); addString() keeps count of the length value in the dataHeader struct; therefore the length provided by the binary file is not used, it is instead calculated by the addString(); &mdash; if this is not correct; fix would be to keep length provided by the binary file, by keeping a variable to save the length read in from the binary file.
- program seg faults if PYTHONPATH is not set properly

## References

1. function [listio.c/removeWithIndex()](http://stackoverflow.com/a/5457657/6175388)
2. function [listio.c/addWithIndex()](http://stackoverflow.com/a/2016015/6175388)
3. function [listio.c/isValInArray()](http://stackoverflow.com/a/15102018/6175388)
4. function [main.c/readQueue()](https://docs.python.org/2/extending/embedding.html)

*References are also referenced in the code comments i.e. [reference #].*

### Developer

Minahil Ikram 0721370
