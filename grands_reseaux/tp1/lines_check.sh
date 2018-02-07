#!/bin/sh

awk '

BEGIN{
    acl_line=""
    acl_in=0
    acl_out=0
}{
    if($1=="line" || $1=="!"){
        if(acl_line != ""){
            if(acl_in == 0)
                print acl_line, "no access-in";
            if(acl_out == 0)
                print acl_line, "no access-out";
        }
        acl_line=$2;
        acl_in = acl_out = 0;
    }
    if($1 == "access-class" && $3=="in") acl_in=1;
    if($1 == "access-class" && $3=="out") acl_out=1;
}END{
}' router.txt
