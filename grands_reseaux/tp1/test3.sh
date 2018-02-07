#!/bin/sh

awk '

BEGIN{
}{
    if($1=="interface"){
        if($0 in interface){
            print $0, "doublon";
        }else{
            interface[$0] = "";
            this = $0;
        }
    }
    if($2 == "mode" && $3 == "access") interface_ip[this] = $0;
}END{
    for(id in interface){
        if(id in interface_ip)
            print id, interface_ip[id];
    }

}' cat1.txt
