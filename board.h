#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#define SCREENWIDTH 1000
#define SCREENHEIGHT 1000
#define SQUAREWIDTH 125

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

class Piece {
public:
    Texture2D image;
    int piece_color;
    char piece_type;
    std::string cur_square;
    std::vector<Square*> legal_squares;
    std::vector<Square*> controlling_squares; // contains pieces from the same color in the legal squares 
                                              // of the piece.

    void update_legal_squares(Board &board);
    std::vector<Square*> get_legal_squares();
    char get_file(); // return the file of the current square
    int get_rank(); // return the rank of the current square
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
    std::map<char, std::vector<Square>> board;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // standard fen
    
    /* These are testing fen notations:
    string fen = "pppppppp/pppppppp/8/8/8/8/PPPPPPPP/PPPPPPPP w KQkq - 0 1"; 
    string fen = "nnnnnnnn/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; 
    string fen = "r1bq1rk1/pp1p1ppp/4pn2/2b5/2PN4/6P1/PP2PPBP/RN1Q1RK1 b - - 4 9";
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string fen = "r1bqkb1r/pp1p1ppp/2n1pn2/8/2PB4/5NP1/PP2PP1P/RN1QKB1R w KQkq - 0 6";
    string fen = "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR";
    */
    
    int turn = P_WHITE;
    int assign_ok = 0; // to check if we can assign the selected piece in the selected square
    int is_check = -1;

    Board();
    bool is_same_color(Piece *piece1, Piece *piece2);
    void handle_check(int check_color);
};

#endif 
