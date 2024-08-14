#ifndef SQUARE_H
#define SQUARE_H

#include "raylib.h"

class Square {
public:
    Rectangle rec; // The raylib rectangle object of the square
    Texture2D image;
    Color color;
    bool hasP = false;
    int pieceColor; // 0 is Empty, 1 is white, 2 is black
    int pieceType; // Bishop, queen, or rook, etc
    char file;
    int rank;

    Square(float x, float y, float width, float height, Color squareColor);
    void assign(Texture2D newTexture, int newColor, int newPiece);
    void removeTexture();
};

#endif
