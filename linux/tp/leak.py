#!/usr/bin/python

import socket
import sys

HOST = '127.0.0.1'
PORT = 5001

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

s.send('2\n32\n')
s.shutdown(socket.SHUT_WR)

tmp = s.recv(1024)
while tmp:
    sys.stdout.write(tmp)
    tmp = s.recv(1024)
