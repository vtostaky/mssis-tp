#!/bin/bash

/etc/init.d/network-manager stop
killall dhclient

ifconfig eth0 10.2.2.1/24
route add default gw 10.2.2.254
echo "nameserver 8.8.8.8" > /etc/resolv.conf
