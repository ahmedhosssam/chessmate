#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"

#include <iostream>
#include <vector>
#include <map>

#define screenWidth 1000
#define screenHeight 1000
#define squareWidth 125

#define KING 'k'
#define PAWN 'p'
#define KNIGHT 'n'
#define BISHOP 'b'
#define ROOK 'r'
#define QUEEN 'q'

#define P_WHITE 0
#define P_BLACK 1

class Piece;
class Square;
class Board;

using namespace std;

class Piece {
public:
    Texture2D image;
    int piece_color;
    char piece_type;
    string cur_square;
    vector<Square*> legalSquares;

    vector<Square*> getLegalSquares(map<char, vector<Square>> &board);
};

class Square {
public:
    Color color;
    Piece piece;

    char file = 'n';
    int rank = -1;
    bool has_piece = false;

    int x; // for rendering
    int y;

};

class Board {
public:
    map<char, vector<Square>> board;
    //string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"; // standard fen
    string fen = "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR";  // for debugging wihout pawns
    int turn = P_WHITE;
    int assign_ok = 0; // to check if we can assign the selected piece in the selected square

    Board();
};

#endif 
