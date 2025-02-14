#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"

#include <iostream>
#include <sstream>
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
    vector<Square*> legal_squares;
    vector<Square*> controlling_squares;

    vector<Square*> get_legal_squares(map<char, vector<Square>> &board);
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
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // standard fen
    
    /* These are testing fen notations:
    string fen = "r1bq1rk1/pp1p1ppp/4pn2/2b5/2PN4/6P1/PP2PPBP/RN1Q1RK1 b - - 4 9";
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string fen = "r1bqkb1r/pp1p1ppp/2n1pn2/8/2PB4/5NP1/PP2PP1P/RN1QKB1R w KQkq - 0 6";
    string fen = "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR";
    */
    
    int turn = P_WHITE;
    int assign_ok = 0; // to check if we can assign the selected piece in the selected square

    Board();
};

#endif 
