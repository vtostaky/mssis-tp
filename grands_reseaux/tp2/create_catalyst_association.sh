#!/bin/sh

awk '

{
    if($1 == "hostname"){
        delete trunk
        hostname = $2
        next
    }
    if($1 == "interface"){
        interface = $2
        next
    }
    if($0 ~/switchport trunk/ && $0 ~/allowed vlan/){
        n=split($5,tmp,",")
        for(i=1;i<=n;i++){
            trunk[tmp[i]] = tmp[i]
        }
        next
    }
    if($0 ~/switchport access vlan/){
        if($4 in trunk){
            print hostname" "interface,"switchport-access",$4,"transported"
        }
        else{
            print hostname" "interface,"switchport-access",$4,"isolated"
        }
        next
    }
}' cat*.txt | sort > catalyst_all

grep "transported" catalyst_all | sort -k 3 > invent.trans
grep "isolated" catalyst_all | sort -k 3 > invent.isola
join -j 4 invent.trans invent.trans |
awk '
BEGIN{
    print "g=DiGraph()"
}{
    if($2==$6 && $3 == $7){
        print "g.add_vertex(\""$2"-"$3"-"$1"\")"
    }
    else {
        print "g.add_edges([(\""$2"-"$3"-"$1"\",\""$6"-"$7"-"$1"\")])"
    }
}'
join -j 4 invent.isola invent.isola |
awk '
{
    if($2==$6){
        if($3 == $7){
            print "g.add_vertex(\""$2"-"$3"-"$1"\")"
        }
        else {
            print "g.add_edges([(\""$2"-"$3"-"$1"\",\""$6"-"$7"-"$1"\")])"
        }
    }
}END{
    print "g.show()"
    print "g.connected_components()"
}'
