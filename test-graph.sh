#!/bin/bash

BIN='./test-graph'
GVI='./graphviz.gv'
IMG='./graph.gif'

if [ ! -e $BIN ]; then
    scons
fi

$BIN $1 $2 $3 $4 $5

if [ -e $GVI ]; then
    dot -Tgif -o$IMG $GVI
fi

if [ -e $IMG ]; then
    #kuickshow $IMG
    #pqiv -i $IMG
    open -a Preview $IMG
fi

