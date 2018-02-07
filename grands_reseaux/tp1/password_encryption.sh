#!/bin/sh

awk '

BEGIN{
}{
    if($1=="service" && $2=="password-encryption"){
        print "password encryption activated";
    }
}END{
}' router.txt
