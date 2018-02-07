#!/bin/sh

awk '

BEGIN{
    i = 0;
}{
    if($1=="interface") interface[i] = $0;
    if($2 == "mode" && $3 == "access") interface_ip[i++] = $0;
}END{
    for(id in interface){
        print id, interface[id], interface_ip[id];
    }

}' cat1.txt
