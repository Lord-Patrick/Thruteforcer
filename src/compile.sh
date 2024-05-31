#!/bin/sh

g++ -c -o IncreasableString.o IncreasableString.cpp -o2
g++ -c -o main.o main.cpp -o2
g++ -o main main.o IncreasableString.o -lssl -lcrypto
./main