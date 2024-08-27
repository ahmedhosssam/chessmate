#include "raylib.h"

#include "../include/board.h"

Square::Square(float x, float y, float width, float height, Color squareColor) {
    rec.x = x;
    rec.y = y;
    rec.width = width;
    rec.height = height;
    color = squareColor;
    pieceType = 0;
    pieceColor = 0;
    hasP = false;
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
    //hasStarted = true;
    this->hasP = square.hasP;
}

void Square::assign(Texture2D newTexture, int newColor, int newPiece, vector<pair<char, int>> newLegalSquares) {
    image = newTexture;
    hasP = true;
    //hasStarted = true;
    pieceColor = newColor;
    pieceType = newPiece;
    legalSquares = newLegalSquares;
}

void Square::assignForTmp(Square square) {
    *this = square;
    this->file = square.file;
    this->rank = square.rank;
    this->legalSquares = square.legalSquares;
    this->hasP = square.hasP;
    //hasStarted = true;
}

void Square::updateLegalSquares(Board &board) {
    if (pieceType == 1) { // pawn
        if (pieceColor == 1) { // white
            if (!board[file][rank+2].hasP && !hasStarted) {
                legalSquares.push_back({char(file), rank+2});
                hasStarted=true;
            }
            if (!board[file][rank+1].hasP) {
                legalSquares.push_back({char(file), rank+1});
            }
            if (board[char(file+1)][rank+1].hasP && board[char(file+1)][rank+1].pieceColor!=pieceColor) {
                legalSquares.push_back({char(file+1), rank+1});
            }
            if (board[char(file-1)][rank+1].hasP && board[char(file-1)][rank+1].pieceColor!=pieceColor) {
                legalSquares.push_back({char(file-1), rank+1});
            }
        } else { // black
            if (!board[file][rank-2].hasP && !hasStarted) {
                legalSquares.push_back({char(file), rank-2});
                hasStarted=true;
            }
            if (!board[file][rank-1].hasP) {
                legalSquares.push_back({char(file), rank-1});
            }
            if (board[char(file+1)][rank-1].hasP && board[char(file-1)][rank+1].pieceColor!=pieceColor) {
                legalSquares.push_back({char(file+1), rank-1});
            }
            if (board[char(file-1)][rank-1].hasP && board[char(file-1)][rank+1].pieceColor!=pieceColor) {
                legalSquares.push_back({char(file-1), rank-1});
            }
        }
    }
}

void Square::removeTexture() {
    Texture2D empty;
    image = empty;
    hasP = false;
    pieceType = 0;
    pieceColor = 0;
}

