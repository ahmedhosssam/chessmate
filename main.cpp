#include <iostream>

#include "board.h"

#include "raylib.h"

#define TRANSPARENT_RED Color{ 230, 41, 55, 100 }

int main() {
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Chessmate");

    SetTargetFPS(60);

    Board board;

    char file;
    int rank;

    Square temp_square;
    Square empty_square;
    Piece temp_piece;
    Piece empty_piece;
    Texture2D empty;

    while (!WindowShouldClose()) {
        BeginDrawing();

        int x = GetMousePosition().x;
        int y = GetMousePosition().y;

        for(int idx = 1; idx <= 8; idx++) {
            for(char ch = 'a'; ch <= 'h'; ch++) {
                Square* square = &board.board[ch][idx];
                square->piece.update_legal_squares(&board);
                int posX = square->x;
                int posY = square->y;

                if (x >= posX && x <= (posX+125) && y >= posY && y <= (posY+125)) {
                    file = square->file;
                    rank = square->rank;
                }

                DrawRectangle(posX, posY, SQUAREWIDTH, SQUAREWIDTH, square->color);
                DrawTexture(square->piece.image, square->x, square->y, WHITE);
            }
        }

        if (IsMouseButtonPressed(0)) {
            if (board.board[file][rank].piece.piece_color == board.turn) {
                temp_square = board.board[file][rank];
                temp_piece = board.board[file][rank].piece;
                board.board[file][rank].piece.image = empty;
                board.assign_ok = 1;
            } else {
                board.assign_ok = 0;
            }
        }

        // for the last selected piece
        char prev_file = temp_square.file;
        int prev_rank = temp_square.rank;

        if (IsMouseButtonDown(0)) {
            if (prev_rank != -1) {
                std::vector<Square*> sq_vec = board.board[prev_file][prev_rank].piece.get_legal_squares();

                for(int i = 0; i < sq_vec.size(); i++) {
                    Square* square = sq_vec[i];
                    int posX = square->x;
                    int posY = square->y;
                    DrawRectangle(posX, posY, SQUAREWIDTH, SQUAREWIDTH, TRANSPARENT_RED);
                }

                DrawTexture(temp_piece.image, x-60, y-60, WHITE);
            }
        }

        if (IsMouseButtonReleased(0) && board.assign_ok) {
            std::vector<Square*> sq_vec = board.board[prev_file][prev_rank].piece.get_legal_squares();
            bool assigned = false;

            for(int i = 0; i < sq_vec.size(); i++) {
                Square* square = sq_vec[i];
                char l_file = square->file;
                int l_rank = square->rank;

                if (l_file == file && l_rank == rank) {
                    board.board[prev_file][prev_rank].has_piece = false;
                    board.board[prev_file][prev_rank].piece = empty_piece;

                    board.board[file][rank].piece = temp_piece;
                    board.board[file][rank].piece.cur_square = file+std::to_string(rank);
                    board.board[file][rank].has_piece = true;
                    board.turn = board.turn == P_WHITE ? P_BLACK : P_WHITE;

                    temp_square = empty_square;
                    temp_piece.image = empty;
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                board.board[prev_file][prev_rank].piece = temp_piece;
                temp_square = empty_square;
                temp_piece.image = empty;
            }
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
