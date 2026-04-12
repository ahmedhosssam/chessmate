#include "board.h"

namespace {
    const std::vector<std::pair<int,int>> ROOK_DIRS = {
        {1,0},{-1,0},{0,1},{0,-1}
    };
    const std::vector<std::pair<int,int>> BISHOP_DIRS = {
        {1,1},{1,-1},{-1,1},{-1,-1}
    };
    const std::vector<std::pair<int,int>> KNIGHT_OFFSETS = {
        {1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}
    };
    const std::vector<std::pair<int,int>> KING_OFFSETS = {
        {1,1},{1,-1},{-1,1},{-1,-1},{0,1},{0,-1},{1,0},{-1,0}
    };

    inline bool on_board(char f, int r) {
        return f >= 'a' && f <= 'h' && r >= 1 && r <= 8;
    }
}

void Piece::update_legal_squares(Board *b) {
    // TODO: Operator overloading [] for board class
    std::map<char, std::vector<Square>> &board = b->board;

    legal_squares.clear();
    controlling_squares.clear();

    const char init_file = this->get_file();
    const int  init_rank = this->get_rank();

    // A move (df, dr) is legal under a pin only if it's collinear with pin_dir.
    auto along_pin = [&](int df, int dr) {
        if (!pinned) return true;
        return df * pin_dir.second == dr * pin_dir.first;
    };

    if (piece_type == QUEEN || piece_type == ROOK || piece_type == BISHOP) {
        std::vector<std::pair<int,int>> dirs;
        if (piece_type != BISHOP) dirs.insert(dirs.end(), ROOK_DIRS.begin(), ROOK_DIRS.end());
        if (piece_type != ROOK)   dirs.insert(dirs.end(), BISHOP_DIRS.begin(), BISHOP_DIRS.end());

        auto slide = [&](int df, int dr) {
            char f = init_file + df;
            int  r = init_rank + dr;
            while (on_board(f, r)) {
                Square& sq = board[f][r];
                if (sq.has_piece) {
                    if (b->is_same_color(&sq.piece, this)) {
                        controlling_squares.push_back(&sq);
                    } else if (sq.piece.piece_type != KING) {
                        legal_squares.push_back(&sq);
                    }
                    break;
                }
                legal_squares.push_back(&sq);
                f += df;
                r += dr;
            }
        };

        for (auto [df, dr] : dirs) {
            if (!along_pin(df, dr)) continue;
            slide(df, dr);
        }
    }

    if (piece_type == PAWN) {
        const int rank_inc = (piece_color == P_WHITE ? 1 : -1);
        const int new_rank = init_rank + rank_inc;

        if (along_pin(0, rank_inc) && on_board(init_file, new_rank)
            && !board[init_file][new_rank].has_piece) {
            legal_squares.push_back(&board[init_file][new_rank]);

            const int double_rank = new_rank + rank_inc;
            const bool first_move = (init_rank == 2 && piece_color == P_WHITE)
                                 || (init_rank == 7 && piece_color == P_BLACK);
            if (first_move && on_board(init_file, double_rank)
                && !board[init_file][double_rank].has_piece) {
                legal_squares.push_back(&board[init_file][double_rank]);
            }
        }

        for (int df = -1; df <= 1; df += 2) {
            if (!along_pin(df, rank_inc)) continue;
            char new_file = init_file + df;
            if (!on_board(new_file, new_rank)) continue;
            Square& sq = board[new_file][new_rank];
            if (sq.has_piece && !b->is_same_color(&sq.piece, this)) {
                legal_squares.push_back(&sq);
            }
        }
    }

    if (piece_type == KNIGHT && !pinned) {
        for (auto [df, dr] : KNIGHT_OFFSETS) {
            char new_file = init_file + df;
            int  new_rank = init_rank + dr;
            if (!on_board(new_file, new_rank)) continue;

            Square& sq = board[new_file][new_rank];
            if (sq.has_piece && b->is_same_color(&sq.piece, this)) {
                controlling_squares.push_back(&sq);
            } else {
                legal_squares.push_back(&sq);
            }
        }
    }

    if (piece_type == KING) {
        // Is (tf, tr) attacked by any enemy piece, treating our own king's current
        // square as empty? (The king is "moving away," so sliders see through it.)
        auto is_square_attacked = [&](char tf, int tr) {
            auto blocks = [&](char f, int r) {
                Square& sq = board[f][r];
                if (!sq.has_piece) return false;
                if (f == init_file && r == init_rank) return false; // king transparent
                return true;
            };

            auto ray_hits_slider = [&](const std::vector<std::pair<int,int>>& dirs,
                                       char first, char second) {
                for (auto [df, dr] : dirs) {
                    char f = tf + df;
                    int  r = tr + dr;
                    while (on_board(f, r)) {
                        if (blocks(f, r)) {
                            Piece& p = board[f][r].piece;
                            if (p.piece_color != piece_color
                                && (p.piece_type == first || p.piece_type == second)) {
                                return true;
                            }
                            break;
                        }
                        f += df;
                        r += dr;
                    }
                }
                return false;
            };

            if (ray_hits_slider(ROOK_DIRS,   ROOK,   QUEEN)) return true;
            if (ray_hits_slider(BISHOP_DIRS, BISHOP, QUEEN)) return true;

            for (auto [df, dr] : KNIGHT_OFFSETS) {
                char f = tf + df;
                int  r = tr + dr;
                if (!on_board(f, r)) continue;
                Piece& p = board[f][r].piece;
                if (board[f][r].has_piece && p.piece_color != piece_color
                    && p.piece_type == KNIGHT) return true;
            }

            // Enemy pawn attacks: a pawn at (tf±1, tr+enemy_dr) attacks (tf, tr).
            const int enemy_pawn_dr = (piece_color == P_WHITE) ? 1 : -1;
            for (int df : {-1, 1}) {
                char f = tf + df;
                int  r = tr + enemy_pawn_dr;
                if (!on_board(f, r)) continue;
                Piece& p = board[f][r].piece;
                if (board[f][r].has_piece && p.piece_color != piece_color
                    && p.piece_type == PAWN) return true;
            }

            // Adjacent enemy king (kings can't stand next to each other).
            for (auto [df, dr] : KING_OFFSETS) {
                char f = tf + df;
                int  r = tr + dr;
                if (!on_board(f, r)) continue;
                Piece& p = board[f][r].piece;
                if (board[f][r].has_piece && p.piece_color != piece_color
                    && p.piece_type == KING) return true;
            }

            return false;
        };

        for (auto [df, dr] : KING_OFFSETS) {
            char new_file = init_file + df;
            int  new_rank = init_rank + dr;
            if (!on_board(new_file, new_rank)) continue;

            Square& dest = board[new_file][new_rank];
            if (dest.has_piece && b->is_same_color(&dest.piece, this)) continue;

            if (!is_square_attacked(new_file, new_rank)) {
                legal_squares.push_back(&dest);
            }
        }

        // Castling.
        const int home_rank = (piece_color == P_WHITE) ? 1 : 8;
        if (init_file == 'e' && init_rank == home_rank && b->is_check != piece_color) {
            const bool can_k = (piece_color == P_WHITE) ? b->white_can_castle_k : b->black_can_castle_k;
            const bool can_q = (piece_color == P_WHITE) ? b->white_can_castle_q : b->black_can_castle_q;

            if (can_k
                && !board['f'][home_rank].has_piece
                && !board['g'][home_rank].has_piece
                && !is_square_attacked('f', home_rank)
                && !is_square_attacked('g', home_rank)) {
                legal_squares.push_back(&board['g'][home_rank]);
            }

            if (can_q
                && !board['b'][home_rank].has_piece
                && !board['c'][home_rank].has_piece
                && !board['d'][home_rank].has_piece
                && !is_square_attacked('c', home_rank)
                && !is_square_attacked('d', home_rank)) {
                legal_squares.push_back(&board['c'][home_rank]);
            }
        }
    }

    // If our king is in check, only moves that resolve it are legal.
    // The king itself is exempt — its move generation already excludes attacked squares.
    if (piece_type != KING && b->is_check == piece_color) {
        if (b->double_check) {
            legal_squares.clear();
            return;
        }
        std::vector<Square*> filtered;
        for (auto* sq : legal_squares) {
            if (b->is_in_checking_pieces_squares(sq)) {
                filtered.push_back(sq);
            }
        }
        legal_squares = std::move(filtered);
    }
}

std::vector<Square*> Piece::get_legal_squares() {
    return this->legal_squares;
}

char Piece::get_file() {
    return this->cur_square[0];
}

int Piece::get_rank() {
    return this->cur_square[1] - '0';
}

Board::Board() {
    // dummy at index 0 to make the vector 1-based
    for (char ch = 'a'; ch <= 'h'; ch++) {
        board[ch].push_back(Square{});
    }

    for (int idx = 1; idx <= 8; idx++) {
        for (char ch = 'a'; ch <= 'h'; ch++) {
            Square sq;
            sq.file = ch;
            sq.rank = idx;
            sq.color = ((ch - 'a' + idx) % 2 == 0) ? RAYWHITE : DARKPURPLE;
            sq.x = (ch - 'a') * SQUAREWIDTH;
            sq.y = (8 - idx) * SQUAREWIDTH;
            board[ch].push_back(sq);
        }
    }

    std::map<char, Texture2D> texture_map;
    const std::string piece_files[] = {"rook", "knight", "bishop", "queen", "king", "pawn"};
    const char piece_chars[] = {'r', 'n', 'b', 'q', 'k', 'p'};

    for (int i = 0; i < 6; i++) {
        Image wimg = LoadImage(("./pieces/white/" + piece_files[i] + ".png").c_str());
        Image bimg = LoadImage(("./pieces/black/" + piece_files[i] + ".png").c_str());
        texture_map[toupper(piece_chars[i])] = LoadTextureFromImage(wimg);
        texture_map[piece_chars[i]] = LoadTextureFromImage(bimg);
        UnloadImage(wimg);
        UnloadImage(bimg);
    }

    // fen parsing
    std::istringstream fen_stream(fen);
    std::string token;
    std::vector<std::string> tokens;
    while (fen_stream >> token) {
        tokens.push_back(token);
    }

    int rank = 8;
    char file = 'a';
    for (char c : tokens[0]) {
        if (c == '/') {
            rank--;
            file = 'a';
        } else if (c >= '1' && c <= '8') {
            file += (c - '0');
        } else {
            Square& sq = board[file][rank];
            sq.piece.image = texture_map[c];
            sq.piece.piece_color = (c >= 'a' && c <= 'z') ? P_BLACK : P_WHITE;
            sq.piece.piece_type = tolower(c);
            sq.piece.cur_square = std::string(1, file) + std::to_string(rank);
            sq.has_piece = true;
            file++;
        }
    }

    turn = (tokens[1] == "w" ? P_WHITE : P_BLACK);

    if (tokens.size() >= 3) {
        white_can_castle_k = tokens[2].find('K') != std::string::npos;
        white_can_castle_q = tokens[2].find('Q') != std::string::npos;
        black_can_castle_k = tokens[2].find('k') != std::string::npos;
        black_can_castle_q = tokens[2].find('q') != std::string::npos;
    }
}


bool Board::is_same_color(Piece *piece1, Piece *piece2) {
    return piece1->piece_color == piece2->piece_color;
}

void Board::compute_checks() {
    is_check = -1;
    double_check = false;
    checking_pieces_squares.clear();

    const std::vector<std::pair<int,int>> all_dirs = {
        {1,0},{-1,0},{0,1},{0,-1},
        {1,1},{1,-1},{-1,1},{-1,-1}
    };

    for (int king_color : {P_WHITE, P_BLACK}) {
        char kf = 0;
        int  kr = 0;
        for (int idx = 1; idx <= 8 && kf == 0; idx++) {
            for (char ch = 'a'; ch <= 'h'; ch++) {
                Square& sq = board[ch][idx];
                if (sq.has_piece && sq.piece.piece_type == KING
                    && sq.piece.piece_color == king_color) {
                    kf = ch;
                    kr = idx;
                    break;
                }
            }
        }
        if (kf == 0) continue;

        int checker_count = 0;
        std::vector<Square*> rays_from_this_king;

        // Sliding attacks: ray out from the king in 8 directions.
        for (auto [df, dr] : all_dirs) {
            std::vector<Square*> ray;
            char f = kf + df;
            int  r = kr + dr;
            while (on_board(f, r)) {
                Square& sq = board[f][r];
                if (sq.has_piece) {
                    if (sq.piece.piece_color != king_color) {
                        const bool orthogonal = (df == 0 || dr == 0);
                        const char t = sq.piece.piece_type;
                        const bool attacks = (orthogonal && (t == ROOK || t == QUEEN))
                                          || (!orthogonal && (t == BISHOP || t == QUEEN));
                        if (attacks) {
                            checker_count++;
                            for (auto* s : ray) rays_from_this_king.push_back(s);
                            rays_from_this_king.push_back(&sq);
                        }
                    }
                    break;
                }
                ray.push_back(&sq);
                f += df;
                r += dr;
            }
        }

        // Knight attacks.
        for (auto [df, dr] : KNIGHT_OFFSETS) {
            char f = kf + df;
            int  r = kr + dr;
            if (!on_board(f, r)) continue;
            Square& sq = board[f][r];
            if (sq.has_piece && sq.piece.piece_color != king_color
                && sq.piece.piece_type == KNIGHT) {
                checker_count++;
                rays_from_this_king.push_back(&sq);
            }
        }

        // Pawn attacks: an enemy pawn attacking our king sits one rank toward us, diagonally.
        const int pawn_dr = (king_color == P_WHITE) ? 1 : -1;
        for (int df : {-1, 1}) {
            char f = kf + df;
            int  r = kr + pawn_dr;
            if (!on_board(f, r)) continue;
            Square& sq = board[f][r];
            if (sq.has_piece && sq.piece.piece_color != king_color
                && sq.piece.piece_type == PAWN) {
                checker_count++;
                rays_from_this_king.push_back(&sq);
            }
        }

        if (checker_count > 0) {
            is_check = king_color;
            double_check = (checker_count > 1);
            checking_pieces_squares = rays_from_this_king;
        }
    }
}

void Board::compute_pins() {
    for (auto& [ch, vec] : board) {
        for (auto& sq : vec) {
            sq.piece.pinned = false;
            sq.piece.pin_dir = {0, 0};
        }
    }

    const std::vector<std::pair<int,int>> all_dirs = {
        {1,0},{-1,0},{0,1},{0,-1},
        {1,1},{1,-1},{-1,1},{-1,-1}
    };

    for (int king_color : {P_WHITE, P_BLACK}) {
        char kf = 0;
        int  kr = 0;
        for (int idx = 1; idx <= 8 && kf == 0; idx++) {
            for (char ch = 'a'; ch <= 'h'; ch++) {
                Square& sq = board[ch][idx];
                if (sq.has_piece && sq.piece.piece_type == KING
                    && sq.piece.piece_color == king_color) {
                    kf = ch;
                    kr = idx;
                    break;
                }
            }
        }
        if (kf == 0) continue;

        for (auto [df, dr] : all_dirs) {
            char f = kf + df;
            int  r = kr + dr;
            Square* candidate = nullptr;

            while (on_board(f, r)) {
                Square& sq = board[f][r];
                if (sq.has_piece) {
                    if (candidate == nullptr) {
                        if (sq.piece.piece_color == king_color) {
                            candidate = &sq;
                        } else {
                            break;
                        }
                    } else {
                        if (sq.piece.piece_color != king_color) {
                            const bool orthogonal = (df == 0 || dr == 0);
                            const char t = sq.piece.piece_type;
                            const bool matches = (orthogonal && (t == ROOK || t == QUEEN))
                                              || (!orthogonal && (t == BISHOP || t == QUEEN));
                            if (matches) {
                                candidate->piece.pinned = true;
                                candidate->piece.pin_dir = {df, dr};
                            }
                        }
                        break;
                    }
                }
                f += df;
                r += dr;
            }
        }
    }
}

void Board::handle_check(int check_color) {
    //std::cout << (check_color == -1 ? "Nothing" : "\n");
    //std::cout << (check_color == 0 ? "White is in check\n" : "\n");
    //std::cout << (check_color == 1 ? "Black is in check\n" : "\n");
}

void Board::assign_check(Piece *piece) {
    //this->is_check = piece->piece_color;
    this->checking_piece = piece->piece_type;
    this->checking_square = piece->cur_square;
}

bool Board::is_in_checking_pieces_squares(Square* square) {
    auto it = std::find_if(this->checking_pieces_squares.begin(), this->checking_pieces_squares.end(), [&](Square* sq) {
        return sq->file == square->file && sq->rank == square->rank;
    });

    //for(auto sq : checking_pieces_squares) {
    //    std::cout << sq->file << sq->rank << std::endl;
    //}

    if (it != checking_pieces_squares.end()) {
        return true;
    }

    return false;
}

void Board::remove_square_from_checking_squares(Square* square) {
    this->checking_pieces_squares.erase(std::remove_if(checking_pieces_squares.begin(), checking_pieces_squares.end(), [&](Square* sq) {
        return sq->file == square->file && sq->rank == square->rank;
    }), checking_pieces_squares.end());
}
