#!/bin/bash

if [ "$(uname)" = "Darwin" ]; then
    g++ -std=c++17 main.cpp board.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib && ./a.out
else
    g++ -std=c++17 main.cpp board.cpp -lraylib && ./a.out
fi
