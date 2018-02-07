#!/bin/sh

awk '

BEGIN{

}{
    if($1=="interface") interface = $0;
    if($2 == "mode" && $3 == "access") print interface, $0;
}END{

}' cat1.txt
