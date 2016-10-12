#!/usr/bin/python3

import os, sys

# open a fifo queue for writing, write 10 strings, close fifo
# sends text through a fifo queue to the reader program

# filename = "./q1"
# pipe = os.open(filename, os.O_WRONLY)
# for i in range(10):
#    os.write(pipe, "aa\n")
# os.close(pipe)


# open a fifo queue for reading, read 10 strings, close fifo
# reads text from another queue sent by the reader program

def readQueue(filename):
    target = open(filename + ".temp", 'w')
    target.write("<html>")
    target.write("<head>")
    target.write("<title>" + filename + "</title>")
    target.write("<body>")
    with open('./q2') as openfileobject:
        for line in openfileobject:
            with open(filename + '.info') as infoObject:
                for info in infoObject:
                    info = info.rstrip('\n')
                    if (info[:1] == 'I'):
                        info = info[2:]
                        if (line.find(info) > 0):
                            line = line[:(line.find(info))] + "<i>" + line[line.find(info): (line.find(info) + len(info))] + "</i>" + line[(line.find(info) + len(info)):]
            target.write(line)
    target.write("</body>")
    target.write("</head>")
    target.write("</html>")
    target.close()
    os.remove('./q2')
