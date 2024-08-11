#!/bin/bash

g++ main.cpp square.cpp board.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

if [ $? -eq 0 ]; then
    ./a.out
fi
