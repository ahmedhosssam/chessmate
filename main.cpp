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
    int s = 0;

    SetTargetFPS(60); 
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (board.getCurrentSquare(GetMouseX(), GetMouseY())->hasP) {
            SetMouseCursor(4);
        } else {
            SetMouseCursor(0);
        }

        if (IsMouseButtonPressed(1)) { // right click
            int tmpSqr = board.tmpSqr;
            int tmpNum = board.tmpNum;

            board[tmpSqr][tmpNum].assign(board.tmp, board.tmpPieceType);
            board.tmpPieceType = 0;
            //tmp = empty;
            s=0;
        }

        if (IsMouseButtonPressed(0)) {
            int x = GetMouseX();
            int y = GetMouseY();

            for(char ch = 'a'; ch <= 'h'; ch++) {
                for(int i = 1; i <= 8; i++) {
                    Square sq = board[ch][i];
                    int x1 = sq.rec.x;
                    int x2 = sq.rec.x + (sq.rec.width);
                    int y1 = sq.rec.y;
                    int y2 = sq.rec.y + (sq.rec.width);
                    if (x>=x1 && x<=x2 && y>=y1 && y<=y2) {
                        if (!s) {
                            board.tmpPieceType = board[ch][i].pieceType;
                            board.tmp = board[ch][i].image;
                            board.assignable = board[ch][i].hasP;
                            board[ch][i].removeTexture();
                            board.tmpSqr = ch;
                            board.tmpNum = i;
                            board.tmpX = x1;
                            board.tmpY = y1;
                            s=1; 
                        } else {
                            if (board.assignable) {
                                int tmpSqr = tmpSqr;
                                int tmpNum = tmpNum;

                                board[ch][i].assign(board.tmp, board.tmpPieceType);
                                board[tmpSqr][tmpNum].removeTexture();
                                board.tmp = board.empty;
                            }
                            s=0;
                        }
                    }
                }
            }
        }

        if (IsMouseButtonReleased(0)) {
            int x = GetMouseX();
            int y = GetMouseY();

            for(char ch = 'a'; ch <= 'h'; ch++) {
                for(int i = 1; i <= 8; i++) {
                    Square sq = board[ch][i];
                    int x1 = sq.rec.x;
                    int x2 = sq.rec.x + (sq.rec.width);
                    int y1 = sq.rec.y;
                    int y2 = sq.rec.y + (sq.rec.width);
                    if (x>=x1 && x<=x2 && y>=y1 && y<=y2) {
                        if (board.assignable && board[ch][i].pieceType != board.tmpPieceType) {
                            board[ch][i].assign(board.tmp, board.tmpPieceType);
                            //board[tmpSqr][tmpNum].removeTexture();
                        } else {
                            int tmpSqr = board.tmpSqr;
                            int tmpNum = board.tmpNum;

                            board[tmpSqr][tmpNum].assign(board.tmp, board.tmpPieceType);
                        }
                        board.tmp = board.empty;
                        s=0;
                    }
                }
            }
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

        DrawTexture(board.tmp, GetMouseX()-50, GetMouseY()-50, WHITE);

        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}
