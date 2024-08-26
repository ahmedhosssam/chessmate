#include "raylib.h"

#include "../include/square.h"

Square::Square(float x, float y, float width, float height, Color squareColor) {
    rec.x = x;
    rec.y = y;
    rec.width = width;
    rec.height = height;
    color = squareColor;
    pieceType = 0;
    pieceColor = 0;
}

Square& Square::operator=(const Square& other) {
    this->image = other.image;
    this->hasP = other.hasP;
    this->hasStarted = other.hasStarted;
    this->pieceColor = other.pieceColor;
    this->pieceType = other.pieceType;

    return *this;
}

void Square::assign(Square square) {
    *this = square;
    hasStarted = true;
}

void Square::assign(Texture2D newTexture, int newColor, int newPiece) {
    image = newTexture;
    hasP = true;
    hasStarted = true;
    pieceColor = newColor;
    pieceType = newPiece;
}

void Square::assignForTmp(Square square) {
    *this = square;
    this->file = square.file;
    this->rank = square.rank;
    hasStarted = true;
}

void Square::removeTexture() {
    Texture2D empty;
    image = empty;
    hasP = false;
    pieceType = 0;
    pieceColor = 0;
}

