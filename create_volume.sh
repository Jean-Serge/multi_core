#!/bin/bash


export CURRENT_VOLUME=0
./frmt.bin ; ./mkvol.bin -fs 1 -fc 0 -s 10000 ; ./mknfs.bin
