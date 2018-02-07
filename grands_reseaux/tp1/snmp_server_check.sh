#!/bin/sh

awk '

BEGIN{
}{
if($1=="snmp-server" && ($4!="RO" || !($5~/^[0-9]+$/))){
        print "Error : snmp server should be RO and be associated with an access-list number";
        print $0;
    }
}END{
}' router.txt
