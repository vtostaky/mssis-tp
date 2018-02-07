#!/bin/sh

awk '

BEGIN{
    print "g=DiGraph()"
}{
    if($2!=$4){
        print "g.add_edge((\""$2"\",\""$4"\"))"
    }
}END{
    print "g.show()"
    print "g.connected_components()"
}' import_export

