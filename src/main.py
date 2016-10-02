#!/usr/bin/python3

import sys
from pathlib import Path
from ctypes import *

def print_file(file_name):
    try:
        file = open(file_name + ".html", "r")
    except IOError:
        print ("There was an error reading " + file_name)
        sys.exit()
    for line in file.read().split('\0'):
        print (line)
    file.close()
    return

def convert_file(file_name):
    class dataHeader(Structure):
        _fields_ = [('name', c_char_p), ('length', c_int), ('dataString', c_void_p)]

    class returnStruct(Structure):
        _fields_ = [('value', c_int), ('header', POINTER(dataHeader))]

    testlib = cdll.LoadLibrary('/home/minahil/Documents/CIS*2750/a1/minahilikram-0721370/listio.so')

    buildHeader = testlib.buildHeader
    buildHeader.restype = POINTER(returnStruct)
    buildHeader.argtypes = []

    returnStruct = buildHeader().contents

    setName = testlib.setName
    setName.restype = c_int
    setName.argtypes = [POINTER(dataHeader), c_char_p]
    setName(returnStruct.header, (b'Hello, World'))

    getLength = testlib.getLength
    getLength.restype = c_int
    getLength.argtypes = [POINTER(dataHeader)]
    # print(getLength(byref(returnStruct.header))
    #
    print(cast(returnStruct.header.contents.name, c_char_p).value)
    print((returnStruct.header.contents.length))

    return

def main():
    file_name = sys.argv[1]

    if Path(file_name + ".html").is_file():
        print_file(file_name + ".html")
    elif Path(file_name).is_file():
        convert_file(file_name)
    return

main()
