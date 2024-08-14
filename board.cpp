#include <iostream>
#include <vector>
#include <map>

using namespace std;

#include "raylib.h"

#include "board.h"
#include "square.h"

Board::Board() {
    const int screenWidth = 1000;
    const int squareWidth = screenWidth/8;
    int inc = screenWidth/8;

    // Build the board
    int j = 7;
    bool f = false;
    for(char ch = 'h'; ch >= 'a'; ch--) {
        Square sq(0, 0, 0, 0, WHITE);
        squares[ch].push_back(sq); // push dummy object to make the array 1-based index
        
        Color color = DARKPURPLE;
        int rank = 1;

        for(int i = 7; i >= 0; i--) {
            if ((f && i%2==0) || (!f && i%2==1)) {
                color = RAYWHITE;
            } else {
                color = DARKPURPLE;
            }
            Square square(j*inc, i*inc, squareWidth, squareWidth, color);
            square.file = ch;
            square.rank = rank;
            rank++;
            squares[ch].push_back(square);
        }
        f = !f;
        j--;
    }

    Image b1 = LoadImageSvg("./pieces/black/king.svg", inc, inc);
    Texture2D king = LoadTextureFromImage(b1); 
    blackPieces["king"] = king;

    Image b2 = LoadImageSvg("./pieces/black/pawn.svg", inc, inc);
    Texture2D pawn = LoadTextureFromImage(b2);
    blackPieces["pawn"] = pawn;

    Image b3 = LoadImageSvg("./pieces/black/rook.svg", inc, inc);
    Texture2D rook = LoadTextureFromImage(b3);
    blackPieces["rook"] = rook;

    Image b4 = LoadImageSvg("./pieces/black/bishop.svg", inc, inc);
    Texture2D bishop = LoadTextureFromImage(b4); 
    blackPieces["bishop"] = bishop;

    Image b5 = LoadImageSvg("./pieces/black/queen.svg", inc, inc);
    Texture2D queen = LoadTextureFromImage(b5); 
    blackPieces["queen"] = queen;

    Image b6 = LoadImageSvg("./pieces/black/knight.svg", inc, inc);
    Texture2D knight = LoadTextureFromImage(b6); 
    blackPieces["knight"] = knight;

    UnloadImage(b1);
    UnloadImage(b2);
    UnloadImage(b3);
    UnloadImage(b4);
    UnloadImage(b5);
    UnloadImage(b6);

    // board setup
    /*
     1 pawn
     2 knight
     3 bishop
     4 rook
     5 queen
     6 king
    */
    squares['a'][8].assign(blackPieces["rook"], 2, 4); 
    squares['b'][8].assign(blackPieces["knight"], 2, 2);
    squares['c'][8].assign(blackPieces["bishop"], 2, 3); 
    squares['d'][8].assign(blackPieces["queen"], 2, 5); 
    squares['e'][8].assign(blackPieces["king"], 2, 6); 
    squares['f'][8].assign(blackPieces["bishop"], 2, 3); 
    squares['g'][8].assign(blackPieces["knight"], 2, 2); 
    squares['h'][8].assign(blackPieces["rook"], 2, 4);
    for(char ch = 'a'; ch <= 'h'; ch++) {
        // for the pawns
        squares[ch][7].assign(blackPieces["pawn"], 2, 1);
    }

    Image w1 = LoadImageSvg("./pieces/white/king.svg", inc, inc);
    king = LoadTextureFromImage(w1); 
    whitePieces["king"] = king;

    Image w2 = LoadImageSvg("./pieces/white/pawn.svg", inc, inc);
    pawn = LoadTextureFromImage(w2);
    whitePieces["pawn"] = pawn;

    Image w3 = LoadImageSvg("./pieces/white/rook.svg", inc, inc);
    rook = LoadTextureFromImage(w3); 
    whitePieces["rook"] = rook;

    Image w4 = LoadImageSvg("./pieces/white/bishop.svg", inc, inc);
    bishop = LoadTextureFromImage(w4);
    whitePieces["bishop"] = bishop;

    Image w5 = LoadImageSvg("./pieces/white/queen.svg", inc, inc);
    queen = LoadTextureFromImage(w5); 
    whitePieces["queen"] = queen;

    Image w6 = LoadImageSvg("./pieces/white/knight.svg", inc, inc);
    knight = LoadTextureFromImage(w6); 
    whitePieces["knight"] = knight;

    UnloadImage(w1);
    UnloadImage(w2);
    UnloadImage(w3);
    UnloadImage(w4);
    UnloadImage(w5);
    UnloadImage(w6);

    // board setup
    squares['a'][1].assign(whitePieces["rook"], 1, 4);
    squares['b'][1].assign(whitePieces["knight"], 1, 2);
    squares['c'][1].assign(whitePieces["bishop"], 1, 3);
    squares['d'][1].assign(whitePieces["queen"], 1, 5);
    squares['e'][1].assign(whitePieces["king"], 1, 6);
    squares['f'][1].assign(whitePieces["bishop"], 1, 3);
    squares['g'][1].assign(whitePieces["knight"], 1, 2);
    squares['h'][1].assign(whitePieces["rook"], 1, 4);
    for(char ch = 'a'; ch <= 'h'; ch++) {
        // for the pawns
        squares[ch][2].assign(whitePieces["pawn"], 1, 1);
    }
}

vector<Square>& Board::operator[](int index) {
    return Board::squares[index];
}

Square* Board::getCurrentSquare(int x, int y) {
    Square* square = &squares['g'][5];
    for(char ch = 'a'; ch <= 'h'; ch++) {
        for(int i = 1; i <= 8; i++) {
            Square &sq = squares[ch][i];
            int x1 = sq.rec.x;
            int x2 = sq.rec.x + sq.rec.width;
            int y1 = sq.rec.y;
            int y2 = sq.rec.y + sq.rec.width;
            if (x>=x1 && x<=x2 && y>=y1 && y<=y2) {
                square = &sq;
                break;
            }
        }
    }
    return square;
}

Board::~Board() {
    for (auto &piece: blackPieces) {
        UnloadTexture(piece.second);
    }
    for (auto &piece: whitePieces) {
        UnloadTexture(piece.second);
    }
}
