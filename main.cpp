#include <iostream>
#include <vector>
#include <map>
#include "raylib.h"

using namespace std;

class Square {
    public:
        Rectangle rec;
        Texture2D image;
        Color color;
        bool hasP = false;

    Square(float x, float y, float width, float height, Color squareColor) {
        rec.x = x;
        rec.y = y;
        rec.width = width;
        rec.height = height;
        color = squareColor;
    }

    void assign(Texture2D newTexture) {
        image = newTexture;
        hasP = true;
    }

    void removeTexture() {
        Texture2D empty;
        image = empty;
        hasP = false;
    }
};

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    const int squareWidth = screenWidth/8;
    int inc = screenWidth/8;

    // Build the board
    map<char, vector<Square>> squares;
    int j = 7;
    bool f = false;
    for(char ch = 'h'; ch >= 'a'; ch--) {
        Square sq(0, 0, 0, 0, WHITE);
        squares[ch].push_back(sq); // push dummy object to make the array 1-based index
        
        Color color = DARKPURPLE;
        for(int i = 7; i >= 0; i--) {
            if ((f && i%2==0) || (!f && i%2==1)) {
                color = RAYWHITE;
            } else {
                color = DARKPURPLE;
            }
            Square square(j*inc, i*inc, squareWidth, squareWidth, color);
            squares[ch].push_back(square);
        }
        f = !f;
        j--;
    }


    InitWindow(screenWidth, screenHeight, "Chessmate");
    Image b1 = LoadImageSvg("./pieces/black/king.svg", inc, inc);
    Texture2D t1 = LoadTextureFromImage(b1); 

    Image b2 = LoadImageSvg("./pieces/black/pawn.svg", inc, inc);
    Texture2D t2 = LoadTextureFromImage(b2);

    Image b3 = LoadImageSvg("./pieces/black/rook.svg", inc, inc);
    Texture2D t3 = LoadTextureFromImage(b3);

    Image b4 = LoadImageSvg("./pieces/black/bishop.svg", inc, inc);
    Texture2D t4 = LoadTextureFromImage(b4); 

    Image b5 = LoadImageSvg("./pieces/black/queen.svg", inc, inc);
    Texture2D t5 = LoadTextureFromImage(b5); 

    Image b6 = LoadImageSvg("./pieces/black/knight.svg", inc, inc);
    Texture2D t6 = LoadTextureFromImage(b6); 
    UnloadImage(b1);
    UnloadImage(b2);
    UnloadImage(b3);
    UnloadImage(b4);
    UnloadImage(b5);
    UnloadImage(b6);

    // board setup
    squares['a'][8].assign(t3); // rook
    squares['b'][8].assign(t6); // knight
    squares['c'][8].assign(t4); // bishop
    squares['d'][8].assign(t5); // queen
    squares['e'][8].assign(t1); // king
    squares['f'][8].assign(t4); // bishop
    squares['g'][8].assign(t6); // knight
    squares['h'][8].assign(t3); // rook
    for(char ch = 'a'; ch <= 'h'; ch++) {
        // for the pawns
        squares[ch][7].assign(t2);
    }

    Image w1 = LoadImageSvg("./pieces/white/king.svg", inc, inc);
    t1 = LoadTextureFromImage(w1); 

    Image w2 = LoadImageSvg("./pieces/white/pawn.svg", inc, inc);
    t2 = LoadTextureFromImage(w2);

    Image w3 = LoadImageSvg("./pieces/white/rook.svg", inc, inc);
    t3 = LoadTextureFromImage(w3); 

    Image w4 = LoadImageSvg("./pieces/white/bishop.svg", inc, inc);
    t4 = LoadTextureFromImage(w4);

    Image w5 = LoadImageSvg("./pieces/white/queen.svg", inc, inc);
    t5 = LoadTextureFromImage(w5); 

    Image w6 = LoadImageSvg("./pieces/white/knight.svg", inc, inc);
    t6 = LoadTextureFromImage(w6); 
    UnloadImage(w1);
    UnloadImage(w2);
    UnloadImage(w3);
    UnloadImage(w4);
    UnloadImage(w5);
    UnloadImage(w6);

    // board setup
    squares['a'][1].assign(t3); // rook
    squares['b'][1].assign(t6); // knight
    squares['c'][1].assign(t4); // bishop
    squares['d'][1].assign(t5); // queen
    squares['e'][1].assign(t1); // king
    squares['f'][1].assign(t4); // bishop
    squares['g'][1].assign(t6); // knight
    squares['h'][1].assign(t3); // rook
    for(char ch = 'a'; ch <= 'h'; ch++) {
        // for the pawns
        squares[ch][2].assign(t2);
    }

    Texture2D tmp;
    Texture2D empty;
    char tmpSqr;
    char tmpNum;
    int tmpX;
    int tmpY;
    bool assignable = false; // if the selected squere doesn't have a piece, it should be skipped
    int s = 0;

    SetTargetFPS(60); 
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsMouseButtonPressed(1)) {
            squares[tmpSqr][tmpNum].assign(tmp);
            tmp = empty;
            s=0;
        }
        if (IsMouseButtonPressed(0) || IsMouseButtonReleased(0)) {
            int x = GetMouseX();
            int y = GetMouseY();

            for(char ch = 'a'; ch <= 'h'; ch++) {
                for(int i = 1; i <= 8; i++) {
                    Square sq = squares[ch][i];
                    int x1 = sq.rec.x;
                    int x2 = sq.rec.x + (sq.rec.width);
                    int y1 = sq.rec.y;
                    int y2 = sq.rec.y + (sq.rec.width);
                    if (x>=x1 && x<=x2 && y>=y1 && y<=y2) {
                        if (tmpX == x1 && tmpY == y1) {
                            break;
                        }
                        if (!s) {
                            tmp = squares[ch][i].image;
                            assignable = squares[ch][i].hasP;
                            squares[ch][i].removeTexture();
                            tmpSqr = ch;
                            tmpNum = i;
                            tmpX = x1;
                            tmpY = y1;
                            s=1;
                        } else {
                            if (assignable) {
                                squares[ch][i].assign(tmp);
                                squares[tmpSqr][tmpNum].removeTexture();
                                tmp = empty;
                            }
                            s=0;
                        }
                    }
                }
            }
        }

        for(char ch = 'a'; ch <= 'h'; ch++) {
            for(int i = 1; i <= 8; i++) {
                Square sq = squares[ch][i];
                DrawRectangleRec(sq.rec, sq.color);
                DrawTexture(sq.image, sq.rec.x, sq.rec.y, WHITE);
            }
        }
        DrawTexture(tmp, GetMouseX()-50, GetMouseY()-50, WHITE);

        EndDrawing();
    }

    UnloadTexture(t1);
    UnloadTexture(t2);
    UnloadTexture(t3);
    UnloadTexture(t4);
    UnloadTexture(t5);
    UnloadTexture(t6);
    CloseWindow(); 

    return 0;
}
