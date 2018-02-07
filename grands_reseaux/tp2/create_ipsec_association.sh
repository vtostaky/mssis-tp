#!/bin/sh

awk '

{
    if($0~/^crypto / && $2 == "map"){
        if(!($3 in cryptomap)){
            i = 0
            this_cryptomap = $3
            cryptomap[$3] = $0
        }
    }
    if($0~/^ set / && $2 == "peer"){
        peer[this_cryptomap, i++] = $3
    }
    if($0~/^ ip address/){
        address = $3
        next
    }
    if($0~/^ crypto / && $2 == "map") {
        if($3 in cryptomap) {
            for(i = 0; (peer[$3,i]) != ""; i++){
                print address, peer[$3,i];
                print peer[$3,i], address;
            }
        }
    }
    if($1 == "end"){
        delete cryptomap
        delete peer
    }
}' conf*.txt | sort | uniq -d |
awk '
BEGIN{
    print "g=DiGraph()"
}{
    print "g.add_edge((\""$1"\",\""$2"\"))"
}END{
    print "g.show()"
    print "g.connected_components()"
}'
