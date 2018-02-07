#!/bin/sh

awk '

BEGIN{
}{
    if($1=="hostname"){
        hostname = $2
    }
    if($1=="ip" && $2=="vrf"){
        vrf = $3
    }
    if($1=="route-target"){
        print hostname"-"vrf, $2, $3
    }
}END{
}' pe*.txt > inventaire_all

grep "export" inventaire_all | sort -k 3 > invent.export
grep "import" inventaire_all | sort -k 3 > invent.import
join -j 3 invent.export invent.import > import_export
