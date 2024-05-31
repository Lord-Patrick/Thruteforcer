#!/bin/sh

g++ -c -o IncreasableString.o IncreasableString.cpp -O2
g++ -c -o main.o main.cpp -O2
g++ -o main main.o IncreasableString.o -lssl -lcrypto
./main