#!/bin/bash
echo "Используйте ./lab3.sh -m [1|2|3]"
gcc -o lab3 lab3.c -lpthread
./lab3 "$@"
