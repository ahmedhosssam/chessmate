#!/bin/bash

g++ main.cpp square.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

if [ $?==0 ]; then
    ./a.out
fi
