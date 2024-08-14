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
    char tmpSqr = 0;
    char tmpNum = 0;
    int tmpX = 0;
    int tmpY = 0;
    int tmpPieceColor = 0;
    int tmpPieceType = 0;

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
            board[tmpSqr][tmpNum].assign(tmp, tmpPieceColor, tmpPieceType);
            tmpPieceColor = 0;
            tmpPieceType = 0;
            //tmp = empty;
            s=0;
        }

        if (IsMouseButtonPressed(0)) { // left click
            if (!s) {
                tmpPieceType = square->pieceType;
                tmpPieceColor = square->pieceColor;
                tmp = square->image;
                board.assignable = square->hasP;
                square->removeTexture();
                tmpSqr = square->file;
                tmpNum = square->rank;
                tmpX = square->rec.x;
                tmpY = square->rec.y;
                s=1;
            } else {
                if (board.assignable) {
                    int tmpSqr = tmpSqr;
                    int tmpNum = tmpNum;

                    square->assign(tmp, tmpPieceColor, tmpPieceType);
                    board[tmpSqr][tmpNum].removeTexture();
                    tmpPieceType = 0;
                    tmpPieceColor = 0;
                    tmp = empty;
                }
                s=0;
            }
        }

        if (IsMouseButtonReleased(0)) {
            if (board.assignable && square->pieceColor != tmpPieceColor && tmpPieceColor==turn && (tmpSqr != square->file || tmpNum != square->rank)) {
                square->assign(tmp, tmpPieceColor, tmpPieceType);
                turn = turn==1 ? 2 : 1;
            } else {
                board[tmpSqr][tmpNum].assign(tmp, tmpPieceColor, tmpPieceType);
            }
            tmp = empty;
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

        DrawTexture(tmp, GetMouseX()-50, GetMouseY()-50, WHITE);

        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}
