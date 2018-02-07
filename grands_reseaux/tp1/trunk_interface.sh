#!/bin/sh

awk '

BEGIN{
    interface=""
    trunk_encaps = 0
    trunk_native_vlan = 0
    trunk_allowed_vlan = 0
    trunk_mode = 0
    port_security = 0;
    access_mode = 0;
}{
    if($1=="interface" || $1=="!"){
        if(interface != ""){
            if(trunk_mode==1){
                if(trunk_encaps == 0) print FILENAME, NR, interface, "no encapsulation"
                if(trunk_native_vlan == 0) print FILENAME, NR, interface, "no native vlan"
                if(trunk_allowed_vlan == 0) print FILENAME, NR, interface, "no allowed vlan"
                if(access_mode == 1) print FILENAME, NR, interface, "access mode activated"
                if(port_security == 1) print FILENAME, NR, interface, "port security activated"
            }
            if(access_mode==1){
                if(trunk_encaps == 1) print FILENAME, NR, interface, "encapsulation"
                if(trunk_native_vlan == 1) print FILENAME, NR, interface, "native vlan"
                if(trunk_allowed_vlan == 1) print FILENAME, NR, interface, "allowed vlan"
                if(trunk_mode == 1) print FILENAME, NR, interface, "trunk mode activated"
                if(port_security == 0) print FILENAME, NR, interface, "port security not activated"
            }
        }
        interface=$2;
        trunk_mode = 0;
        trunk_encaps = 0;
        trunk_native_vlan = 0
        trunk_allowed_vlan = 0
        access_mode = 0
        port_security = 0
    }
    if($2=="trunk" && $3=="encapsulation"){
        trunk_encaps = 1;
    }
    if($2=="trunk" && $3=="native" && $4=="vlan"){
        trunk_native_vlan = 1;
    }
    if($2=="trunk" && $3=="allowed" && $4=="vlan"){
        trunk_allowed_vlan = 1;
    }
    if($2=="mode" && $3=="access"){
        access_mode = 1;
    }
    if($2=="port-security"){
        port_security = 1;
    }
    if($2=="mode" && $3=="trunk"){
        trunk_mode = 1;
    }
}END{
}' cat*.txt
