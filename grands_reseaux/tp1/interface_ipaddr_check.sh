#!/bin/sh

awk '

BEGIN{
    interface=""
    access_group = 0
}{
    if($1=="interface" || $1=="!"){
        if(interface != ""){
            if(access_group==0)
                print interface, "no access group"
        }
        interface=$2;
        access_group = 0;
    }
    if($1 == "no" && $2 == "ip" && $3=="address"){
        interface="";
    }
    else if($1 == "ip" && $2=="address" && !($3~/^[1-9][0-9]{1,2}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$/)){
        print interface, "no valid ip"
    }
    if(interface != "" && $1 == "ip" && $2 == "access-group"){
        access_group = 1;
    }
}END{
}' router.txt
