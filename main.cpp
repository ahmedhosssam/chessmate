#include <iostream>
#include <cmath>
#include <set>
#include <utility>

#include "board.h"

#include "raylib.h"

#define TRANSPARENT_RED Color{ 230, 41, 55, 100 }
#define ARROW_COLOR     Color{ 255, 170, 0, 160 }

using Sq = std::pair<char, int>;

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

    std::set<Sq> highlights;
    std::set<std::pair<Sq, Sq>> arrows;
    bool right_dragging = false;
    Sq right_start{0, -1};

    auto draw_arrow = [&](Sq from, Sq to) {
        Square& s = board.board[from.first][from.second];
        Square& e = board.board[to.first][to.second];
        Vector2 src_center{(float)(s.x + SQUAREWIDTH/2), (float)(s.y + SQUAREWIDTH/2)};
        Vector2 end       {(float)(e.x + SQUAREWIDTH/2), (float)(e.y + SQUAREWIDTH/2)};

        Vector2 d{end.x - src_center.x, end.y - src_center.y};
        float len = sqrtf(d.x*d.x + d.y*d.y);
        if (len < 1.0f) return;
        Vector2 u{d.x/len, d.y/len};
        Vector2 perp{-u.y, u.x};

        const float start_inset = SQUAREWIDTH * 0.4f; // push start toward the edge
        Vector2 start{src_center.x + u.x*start_inset, src_center.y + u.y*start_inset};

        const float head_len = 46.0f;
        const float head_w   = 38.0f;
        const float thick    = 24.0f;
        Vector2 base { end.x - u.x*head_len, end.y - u.y*head_len };
        Vector2 left { base.x + perp.x*head_w, base.y + perp.y*head_w };
        Vector2 right{ base.x - perp.x*head_w, base.y - perp.y*head_w };

        DrawLineEx(start, base, thick, ARROW_COLOR);
        DrawTriangle(end, left, right, ARROW_COLOR);
        DrawTriangle(end, right, left, ARROW_COLOR); // draw both windings to be culling-agnostic
    };

    while (!WindowShouldClose()) {
        BeginDrawing();

        int x = GetMousePosition().x;
        int y = GetMousePosition().y;
        //std::cout << board.checking_pieces_squares.size() << std::endl;
        //std::cout << board.board['c'][3].has_piece << std::endl;

        board.compute_checks();
        board.compute_pins();

        for(int idx = 1; idx <= 8; idx++) {
            for(char ch = 'a'; ch <= 'h'; ch++) {
                Square* square = &board.board[ch][idx];
                if (square->has_piece) {
                    square->piece.update_legal_squares(&board);
                }
                int posX = square->x;
                int posY = square->y;

                if (x >= posX && x <= (posX+125) && y >= posY && y <= (posY+125)) {
                    file = square->file;
                    rank = square->rank;
                }

                DrawRectangle(posX, posY, SQUAREWIDTH, SQUAREWIDTH, square->color);
                if (highlights.count({square->file, square->rank})) {
                    DrawRectangle(posX, posY, SQUAREWIDTH, SQUAREWIDTH, TRANSPARENT_RED);
                }
                DrawTexture(square->piece.image, square->x, square->y, WHITE);
            }
        }

        if (IsMouseButtonPressed(0)) {
            highlights.clear();
            arrows.clear();
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

        if (IsMouseButtonPressed(1)) {
            if (prev_rank != -1) {
                board.board[prev_file][prev_rank].piece = temp_piece;
                temp_square = empty_square;
                temp_piece.image = empty;
                board.assign_ok = 0;
            } else {
                right_start = {file, rank};
                right_dragging = true;
            }
        }

        if (IsMouseButtonReleased(1) && right_dragging) {
            Sq end{file, rank};
            if (end == right_start) {
                if (highlights.count(end)) highlights.erase(end);
                else highlights.insert(end);
            } else {
                std::pair<Sq, Sq> arrow{right_start, end};
                if (arrows.count(arrow)) arrows.erase(arrow);
                else arrows.insert(arrow);
            }
            right_dragging = false;
        }

        for (const auto& [from, to] : arrows) {
            draw_arrow(from, to);
        }
        if (right_dragging && !(file == right_start.first && rank == right_start.second)) {
            draw_arrow(right_start, {file, rank});
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

                    // Castling: king moved two files horizontally → also move the rook.
                    if (temp_piece.piece_type == KING && prev_rank == rank
                        && (file == 'g' || file == 'c') && prev_file == 'e') {
                        char rook_from = (file == 'g') ? 'h' : 'a';
                        char rook_to   = (file == 'g') ? 'f' : 'd';
                        board.board[rook_to][rank].piece = board.board[rook_from][rank].piece;
                        board.board[rook_to][rank].piece.cur_square = rook_to + std::to_string(rank);
                        board.board[rook_to][rank].has_piece = true;
                        board.board[rook_from][rank].piece = empty_piece;
                        board.board[rook_from][rank].has_piece = false;
                    }

                    // Update castling rights.
                    if (temp_piece.piece_type == KING) {
                        if (temp_piece.piece_color == P_WHITE) {
                            board.white_can_castle_k = false;
                            board.white_can_castle_q = false;
                        } else {
                            board.black_can_castle_k = false;
                            board.black_can_castle_q = false;
                        }
                    }
                    if (temp_piece.piece_type == ROOK) {
                        if (prev_file == 'a' && prev_rank == 1) board.white_can_castle_q = false;
                        if (prev_file == 'h' && prev_rank == 1) board.white_can_castle_k = false;
                        if (prev_file == 'a' && prev_rank == 8) board.black_can_castle_q = false;
                        if (prev_file == 'h' && prev_rank == 8) board.black_can_castle_k = false;
                    }
                    // Rook captured on its starting square.
                    if (file == 'a' && rank == 1) board.white_can_castle_q = false;
                    if (file == 'h' && rank == 1) board.white_can_castle_k = false;
                    if (file == 'a' && rank == 8) board.black_can_castle_q = false;
                    if (file == 'h' && rank == 8) board.black_can_castle_k = false;

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
