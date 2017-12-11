#!/bin/bash

for rep in `ls -d */ 2> /dev/null`
    do
        cp ${0} "$rep"
        cd "$rep"
        ${0}
        cd ..
    done
