#include <iostream>
#include <vector>
#include <map>

#include "raylib.h"

#include "square.h"
#include "board.h"

using namespace std;

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    const int squareWidth = screenWidth/8;

    InitWindow(screenWidth, screenHeight, "Chessmate");

    Board board;
    Texture2D tmp;
    Texture2D empty;
    int s = 0;
    int turn = 1; // 1 is white, 2 is black

    Square emptySquare(0, 0, 0, 0, BLACK);
    Square tmpSquare(0, 0, 0, 0, BLACK);

    SetTargetFPS(60); 
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Square* square = board.getCurrentSquare(GetMouseX(), GetMouseY());

        if (square->hasP) {
            SetMouseCursor(4);
        } else {
            SetMouseCursor(0);
        }

        if (IsMouseButtonPressed(1)) { // right click
            board[tmpSquare.file][tmpSquare.rank].assign(tmpSquare);
            //tmp = empty;
            s=0;
        }

        if (IsMouseButtonPressed(0)) { // left click
            if (!s) {
                tmpSquare.assignForTmp(*square);
                square->removeTexture();
                s=1;
            } else {
                if (board.assignable) {
                    square->assign(tmpSquare);
                    board[tmpSquare.file][tmpSquare.rank].removeTexture();
                    tmp = empty;
                }
                s=0;
            }
        }

        if (IsMouseButtonReleased(0)) {
            if (square->pieceColor != tmpSquare.pieceColor && tmpSquare.pieceColor==turn && (tmpSquare.file != square->file || tmpSquare.rank != square->rank)) {
                square->assign(tmpSquare);
                turn = turn==1 ? 2 : 1;
            } else {
                board[tmpSquare.file][tmpSquare.rank].assign(tmpSquare);
            }
            tmpSquare.assignForTmp(emptySquare);
            s=0;
        }

        if (IsMouseButtonReleased(1)) {
            // TODO: The right click release now is kinda ugly, this needs to be implemented.
        }

        for(char ch = 'a'; ch <= 'h'; ch++) {
            for(int i = 1; i <= 8; i++) {
                Square sq = board[ch][i];
                DrawRectangleRec(sq.rec, sq.color);
                DrawTexture(sq.image, sq.rec.x, sq.rec.y, WHITE);
            }
        }

        DrawTexture(tmpSquare.image, GetMouseX()-50, GetMouseY()-50, WHITE);

        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}
