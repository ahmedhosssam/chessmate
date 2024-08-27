#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

#include "raylib.h"

class Board;

/*
 For `pieceType`:
     0 empty
     1 pawn
     2 knight
     3 bishop
     4 rook
     5 queen
     6 king

 For `pieceColor`:
     0 Empty
     1 White
     2 Black
*/

class Square {
public:
    Rectangle rec; // The raylib rectangle object of the square
    Texture2D image;
    Color color;
    vector<pair<char, int>> legalSquares;
    char file;
    int rank;
    bool hasP = false;

    // this is mainly for pawns, to add an additional square if it has not moved
    bool hasStarted = false; 

    // 0 is Empty, 1 is white, 2 is black
    int pieceColor; 

    // Bishop, queen, or rook, etc
    int pieceType; 

    Square(float x, float y, float width, float height, Color squareColor);
    void assign(Square square);
    void assignForTmp(Square square);
    void assign(Texture2D newTexture, int newColor, int newPiece, vector<pair<char, int>> newLegalSquares = {});
    void removeTexture();
    void updateLegalSquares(Board &board);
    Square& operator=(const Square& other);
};

class Board {
public:
    map<char, vector<Square>> squares;
    map<string, Texture2D> blackPieces;
    map<string, Texture2D> whitePieces;

    Texture2D tmp;
    Texture2D empty;
    bool assignable = false; // if the selected squere doesn't have a piece, it should be skipped
    
    Board();
    vector<Square>& operator[](int index);
    Square* getCurrentSquare(int x, int y);

    ~Board();
};

#endif
