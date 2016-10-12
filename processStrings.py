#!/usr/bin/python3

import os, sys

def rwQueue(filename):
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
                    if (info[:1] == 'U'):
                        info = info[2:]
                        if (line.find(info) > 0):
                            line = line[:(line.find(info))] + "<u>" + line[line.find(info): (line.find(info) + len(info))] + "</u>" + line[(line.find(info) + len(info)):]
                    if (info[:1] == 'B'):
                        info = info[2:]
                        if (line.find(info) > 0):
                            line = line[:(line.find(info))] + "<b>" + line[line.find(info): (line.find(info) + len(info))] + "</b>" + line[(line.find(info) + len(info)):]
            target.write(line)
    target.write("</body>")
    target.write("</head>")
    target.write("</html>")
    target.close()
    os.remove('./q2')
