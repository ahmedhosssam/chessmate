#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

#include "raylib.h"

#include "square.h"

class Board {
public:
    map<char, vector<Square>> squares;
    map<string, Texture2D> blackPieces;
    map<string, Texture2D> whitePieces;

    Texture2D tmp;
    Texture2D empty;
    char tmpSqr;
    char tmpNum;
    int tmpX;
    int tmpY;
    int tmpPieceType = 0;
    bool assignable = false; // if the selected squere doesn't have a piece, it should be skipped
    
    Board();
    vector<Square>& operator[](int index);
    Square* getCurrentSquare(int x, int y);

    ~Board();
};

#endif
