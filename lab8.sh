#!/bin/bash
gcc serv.c -o serv -lpthread
gcc client.c -o client -lpthread
