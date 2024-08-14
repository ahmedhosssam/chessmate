#include "raylib.h"

#include "square.h"

Square::Square(float x, float y, float width, float height, Color squareColor) {
    rec.x = x;
    rec.y = y;
    rec.width = width;
    rec.height = height;
    color = squareColor;
    pieceType = 0;
}

void Square::assign(Texture2D newTexture, int newColor, int newPiece) {
    image = newTexture;
    hasP = true;
    pieceColor = newColor;
    pieceType = newPiece;
}

void Square::removeTexture() {
    Texture2D empty;
    image = empty;
    hasP = false;
    pieceType = 0;
    pieceColor = 0;
}
