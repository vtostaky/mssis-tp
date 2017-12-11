#!/bin/sh

/etc/init.d/network-manager stop
killall dhclient

BAD=eth0
LAN=eth1
DMZ=eth2

SMTP_GW="192.168.1.10"
SMTP="192.168.1.11"
HTTP_GW="192.168.1.12"

ip addr flush dev $BAD
ip addr flush dev $LAN
ip addr flush dev $DMZ
ip addr add 10.0.6.2/30 broadcast 10.0.6.3 dev $BAD
ip addr add 192.168.2.1/24 broadcast 192.168.2.255 dev $LAN
ip addr add 192.168.1.1/24 broadcast 192.168.1.255 dev $DMZ

ip route add default via 10.0.6.1 dev $BAD

echo "nameserver 8.8.8.8" > /etc/resolv.conf
echo "1" > /proc/sys/net/ipv4/ip_forward

_stop(){
    #RAZ regles de filtrage

    iptables -F
    iptables -t nat -F
    iptables -P INPUT ACCEPT
    iptables -P FORWARD ACCEPT
    iptables -P OUTPUT ACCEPT

    #Fournir internet au LAN & DMZ
    iptables -t nat -A POSTROUTING -o $BAD -j MASQUERADE
    echo "Firewall deactivated"
}

_start(){
    iptables -F
    iptables -F -t nat
    iptables -F -t mangle
    iptables -X
    
    #Forward autorisé entre interfaces
    iptables -P INPUT DROP
    iptables -P OUTPUT ACCEPT
    iptables -P FORWARD DROP
   
    iptables -N BAD-DMZ
    iptables -N DMZ-BAD
    iptables -N LAN-DMZ
    iptables -N DMZ-LAN
    iptables -N LAN-BAD

    #iptables -A INPUT -p tcp -j LOG --log-level warning
    ## Masquerading
    iptables -t nat -A POSTROUTING -o $BAD -j MASQUERADE
    
    iptables -A BAD-DMZ -p tcp                      -d $SMTP_GW --dport smtp -j ACCEPT
    iptables -A BAD-DMZ -m limit --limit 4/s -j LOG --log-prefix "Invalid packet BAD-DMZ "
    iptables -A BAD-DMZ -j DROP
    
    iptables -A DMZ-BAD -p tcp  --sport smtp -j ACCEPT
    iptables -A DMZ-BAD -p tcp                      --sport domain -j ACCEPT
    iptables -A DMZ-BAD -p tcp                      --sport http -j ACCEPT
    iptables -A DMZ-BAD -p tcp                      --sport https -j ACCEPT
    iptables -A DMZ-BAD -m limit --limit 4/s -j LOG --log-prefix "Invalid packet DMZ-BAD "
    iptables -A DMZ-BAD -j DROP
    
    iptables -A LAN-DMZ -p tcp -s $SMTP         -d $SMTP_GW --dport smtp -j ACCEPT
    iptables -A LAN-DMZ -m limit --limit 4/s -j LOG --log-prefix "Invalid packet LAN-DMZ "
    iptables -A LAN-DMZ -j DROP
    
    iptables -A DMZ-LAN -p tcp -s $SMTP_GW          --dport smtp -j ACCEPT
    iptables -A DMZ-LAN -p tcp                      --sport http -j ACCEPT
    iptables -A DMZ-LAN -p tcp                      --sport https -j ACCEPT
    iptables -A DMZ-LAN -m limit --limit 4/s -j LOG --log-prefix "Invalid packet DMZ-LAN "
    iptables -A DMZ-LAN -j DROP

    iptables -A LAN-BAD -p tcp                      --dport domain -j ACCEPT
    iptables -A LAN-BAD -p udp                      --dport domain -j ACCEPT
    iptables -A LAN-BAD -p tcp                      --dport http -j ACCEPT
    iptables -A LAN-BAD -p tcp                      --dport https -j ACCEPT
    iptables -A LAN-BAD -m limit --limit 4/s -j LOG --log-prefix "Invalid packet LAN-BAD "
    iptables -A LAN-BAD -j DROP

    iptables -A FORWARD -i $BAD -o $DMZ -j ACCEPT
    iptables -A FORWARD -i $DMZ -o $BAD -j ACCEPT
    iptables -A FORWARD -i $LAN -o $DMZ -j LAN-DMZ
    iptables -A FORWARD -i $DMZ -o $LAN -j DMZ-LAN
    iptables -A FORWARD -i $LAN -o $BAD -j LAN-BAD
    iptables -A FORWARD -i $BAD -o $LAN -j ACCEPT
    
    iptables -A FORWARD -m limit --limit 4/second -j LOG --log-prefix "Bad Forward "
    iptables -A FORWARD -j DROP
    
    #accepte les connexions déjà établies
    iptables -A INPUT -m state --state INVALID -j DROP
    iptables -A INPUT -m limit --limit 4/second -m state --state INVALID -j LOG --log-prefix "INVALID "
    iptables -A INPUT -m state --state NEW,RELATED -p tcp ! --tcp-flags ALL SYN -j DROP
    iptables -A INPUT -m limit --limit 4/second -m state --state NEW,RELATED -p tcp ! --tcp-flags ALL SYN -j LOG --log-prefix "NotSynNew "
    iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

    #Apply rules for each interface
    iptables -A INPUT -i lo -j ACCEPT
    iptables -A INPUT -m limit --limit 4/second -j LOG --log-prefix "Bad Input "
    iptables -A INPUT -j DROP
    
    echo "--------- Firewall activé ----------"
    exit
}

case "$1" in
    start)
        _start
        ;;
    stop)
        _stop
        ;;
    restart)
        _stop
        _start
        ;;
    *)
        #echo "Usage: {start|stop|restart}"
        exit 1
esac
