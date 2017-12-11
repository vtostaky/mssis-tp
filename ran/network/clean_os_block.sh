#!/bin/sh

sed -i ':a;N;$!ba;s/\nOS://g;s/)/)\n/g;s/%/:/g' ${1}
