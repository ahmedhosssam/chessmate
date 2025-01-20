#!/bin/bash

# TODO: switch to use make file instead of compile script
g++ main.cpp src/square.cpp src/board.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./a.out

#if [ $? -eq 0 ]; then
#    ./a.out
#fi
