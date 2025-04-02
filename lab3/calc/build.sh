#!/bin/sh

bison -Wcounterexamples -o rpcalc.c rpcalc.y
gcc -Wall -o rpcalc rpcalc.c -lm
#rm rpcalc.c

