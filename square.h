#ifndef SQUARE_H
#define SQUARE_H

#include "raylib.h"

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
    bool hasP = false;
    bool hasStarted = false; // this is mainly for pawns, to add an additional square if it has not moved
    int pieceColor; // 0 is Empty, 1 is white, 2 is black
    int pieceType; // Bishop, queen, or rook, etc
    char file;
    int rank;

    Square(float x, float y, float width, float height, Color squareColor);
    void assign(Texture2D newTexture, int newColor, int newPiece);
    void removeTexture();
    Square& operator=(const Square& other);
};

#endif
