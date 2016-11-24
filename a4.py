#!/usr/bin/python

import os, sys, time
from os.path import basename

try:
    sys.stdout.close()
except:
    pass
try:
    sys.stderr.close()
except:
    pass

filename = sys.argv[1]

# open a fifo queue for writing, write 10 strings, close fifo
# sends text through a fifo queue to the reader program

pipe = os.open("./q1", os.O_WRONLY)
os.write(pipe, "<html>")
os.write(pipe, "<head>")
os.write(pipe, "<title>" + filename + "</title>")
os.write(pipe, "<body>")


# open a fifo queue for reading, read 10 strings, close fifo
# reads text from another queue sent by the reader program

#os.mkfifo(filename)
with open('./q2') as openfileobject:
    for line in openfileobject:
        if (os.path.isfile(filename + '.info')):
            with open(filename + '.info') as infoObject:
                for info in infoObject:
                    info = info.rstrip('\n')
                    if (info[:1] == 'I'):
                        info = info[2:]
                        if (line.find(info) >= 0):
                            line = line[:(line.find(info))] + "<i>" + line[line.find(info): (line.find(info) + len(info))] + "</i>" + line[(line.find(info) + len(info)):]
                    if (info[:1] == 'U'):
                        info = info[2:]
                        if (line.find(info) >= 0):
                            line = line[:(line.find(info))] + "<u>" + line[line.find(info): (line.find(info) + len(info))] + "</u>" + line[(line.find(info) + len(info)):]
                    if (info[:1] == 'B'):
                        info = info[2:]
                        if (line.find(info) >= 0):
                            line = line[:(line.find(info))] + "<b>" + line[line.find(info): (line.find(info) + len(info))] + "</b>" + line[(line.find(info) + len(info)):]
        elif (os.path.isfile(basename(filename) + '.info')):
            with open(basename(filename) + '.info') as infoObject:
                for info in infoObject:
                    info = info.rstrip('\n')
                    if (info[:1] == 'I'):
                        info = info[2:]
                        if (line.find(info) >= 0):
                            line = line[:(line.find(info))] + "<i>" + line[line.find(info): (line.find(info) + len(info))] + "</i>" + line[(line.find(info) + len(info)):]
                    if (info[:1] == 'U'):
                        info = info[2:]
                        if (line.find(info) >= 0):
                            line = line[:(line.find(info))] + "<u>" + line[line.find(info): (line.find(info) + len(info))] + "</u>" + line[(line.find(info) + len(info)):]
                    if (info[:1] == 'B'):
                        info = info[2:]
                        if (line.find(info) >= 0):
                            line = line[:(line.find(info))] + "<b>" + line[line.find(info): (line.find(info) + len(info))] + "</b>" + line[(line.find(info) + len(info)):]
        os.write(pipe, line)
os.remove('./q2')

os.write(pipe, "</body>")
os.write(pipe, "</head>")
os.write(pipe, "</html>")

os.close(pipe)

sys.exit()
