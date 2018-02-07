#!/bin/sh

awk '

BEGIN{
}{
    if($1=="access-list") acl_def[$2] = $0;
    if($1 == "ip" && $2=="access-list") acl_def[$4] = $0;
    if($1 == "ip" && $2=="access-group") acl_ref[$3] = $0;
    if($1 == "snmp-server") acl_ref[$5] = $0;
    if($1 =="access-class") acl_def[$2] = $0;
}END{
    for(id in acl_def){
        if(!(id in acl_ref))
            print "Error", acl_def[id], "has no ref!";
    }
    for(id in acl_ref){
        if(!(id in acl_def))
            print "Error", acl_ref[id], "has no def!";
    }
}' router.txt
