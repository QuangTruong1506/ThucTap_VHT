#!/bin/bash

gcc -pthread -o Thread Thread.c

for period in 1000000 100000 10000 1000 100
do
    echo "$period" > freq.txt
    timeout 60 ./Thread 
done
