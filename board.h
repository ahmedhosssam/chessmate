#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"

#include <iostream>
#include <algorithm>
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

    void update_legal_squares(Board *board);
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
    //std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // standard fen
    //std::string fen = "rnbqkbnr/pp1p1ppp/2p1p3/8/8/3PPP2/PPP3PP/RNBQKBNR b KQkq - 0 3"; // standard fen
    std::string fen = "8/8/8/2q5/8/6K1/8/2B5 b - - 0 1"; // standard fen
    
    int turn = P_WHITE;
    int assign_ok = 0; // to check if we can assign the selected piece in the selected square

    int is_check = -1;
    char checking_piece; // the piece that made a check.
    std::string checking_square; // the square of the piece that made a check.
    std::vector<Square*> checking_pieces_squares; // legal squares of the checking piece that should be covered.

    Board();
    bool is_same_color(Piece *piece1, Piece *piece2);

    void handle_check(int check_color);
    void assign_check(Piece *piece);
    bool is_in_checking_pieces_squares(Square* square);
    void remove_square_from_checking_squares(Square* square);
};

#endif 
