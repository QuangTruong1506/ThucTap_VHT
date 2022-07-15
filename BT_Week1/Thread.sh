#!/bin/bash

gcc -pthread -o Thread Thread.c

for period in 100
do
    echo "$period" > freq.txt
    timeout 30 ./Thread 
done
