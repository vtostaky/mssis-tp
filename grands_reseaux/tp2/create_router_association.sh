#!/bin/sh

awk '

{
    if($2=="address"){
        addr = $3
    }
    if($1=="router" && $2=="bgp"){
        as = $3
    }
    if($3=="remote-as"){
        print as, addr, $4, $2
        print $4, $2, as, addr
    }
}' r*.txt | sort | uniq -d |
awk '
BEGIN{
    print "g=DiGraph()"
}{
    print "g.add_edges([("$1","$3")])"
}END{
    print "g.show()"
    print "g.connected_components()"
}'
