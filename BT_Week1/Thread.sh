#!/bin/bash

gcc -pthread -o Thread Thread.c

for period in 100000
do
    echo "$period" > freq.txt
    timeout 10 ./Thread 
done
