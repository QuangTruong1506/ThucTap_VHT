#!/bin/bash

gcc -pthread -o Thread Thread.c

for period in 1000000
do
    echo "$period" > freq.txt
    timeout 10 ./Thread 
done
