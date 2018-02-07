#!/bin/sh

awk '

BEGIN{
}{
    if($1=="interface"){
        interface = $0;
    }
    if($1 == "ip" && $2 == "address" && $3~/192[.]/){
        print interface, $0;
    }
}END{
}' conf1.txt
