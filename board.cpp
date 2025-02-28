#include "board.h"

//vector<Square*> Piece::get_legal_squares(map<char, vector<Square>> &board) {
void Piece::update_legal_squares(Board *b) {
    // TODO: Operator overloading [] for board class
    std::map<char, std::vector<Square>> &board = b->board;

    legal_squares.clear();
    std::vector<Square*> tmp; // This is a temp legal_squares vector. The goal: to check if the king is in check.

    if (this->piece_color == b->is_check) {
        b->handle_check(this->piece_color);
    }

    if (piece_type == QUEEN || piece_type == ROOK) {
        // horizontal and vertical tracking
        char init_file = this->get_file();
        int init_rank = this->get_rank();

        for(int i = -1; i <= 1; i++) {
            if (i == 0) { continue; }

            // horizontal 
            for(char ch = init_file+i; ch <= 'h' && ch >= 'a'; ch+=i) {
                if (board[ch][init_rank].has_piece) {
                    if (!b->is_same_color(&board[ch][init_rank].piece, this)) {
                        if (board[ch][init_rank].piece.piece_type == KING) {
                            b->assign_check(&board[ch][init_rank].piece);
                        } else {
                            tmp.push_back(&board[ch][init_rank]);
                        }
                    }
                    break;
                }

                tmp.push_back(&board[ch][init_rank]);
            }

            if (b->is_check > -1) {
                for(auto square : tmp) {
                    b->checking_pieces_squares.push_back(square);
                }
                return;
            }

            // vertical 
            for(int idx = init_rank+i; idx <= 8 && idx >= 1; idx+=i) {
                if (board[init_file][idx].has_piece) {
                    if (!b->is_same_color(&board[init_file][idx].piece, this)) {
                        if (board[init_file][idx].piece.piece_type == KING) {
                            b->is_check = board[init_file][idx].piece.piece_color;
                        } else {
                            tmp.push_back(&board[init_file][idx]);
                        }
                    } else {
                        controlling_squares.push_back(&board[init_file][idx]);
                    }
                    break;
                }

                tmp.push_back(&board[init_file][idx]);
            }

            if (b->is_check == -1) {
                for(auto sq : tmp) {
                    legal_squares.push_back(sq);
                }

                tmp.clear();
            } else {
                for(auto square : tmp) {
                    std::cout << square->file << square->rank << std::endl;
                    b->checking_pieces_squares.push_back(square);
                }
                return;
            }
        }
    }

    if (piece_type == QUEEN || piece_type == BISHOP) {
        // diagonal tracking
        if (b->is_check > -1) { return; }
        char init_file = this->get_file();
        int init_rank = this->get_rank();

        for(int i = -1; i <= 1; i++) {
            if (i == 0) { continue; }

            init_file = cur_square[0];
            init_file = this->get_file();
            init_rank = this->get_rank()+i;

            if (init_rank <= 0 || init_rank > 8) { continue; }

            bool check_found = false;

            for(char ch = init_file+1; ch <= 'h'; ch++) {
                if (b->is_check
                    && !b->is_same_color(&board[ch][init_rank].piece, this)
                    && b->is_in_checking_pieces_squares(&board[ch][init_rank]))
                {
                    std::cout << "gg\n";
                }

                if (board[ch][init_rank].has_piece) {
                    if (!b->is_same_color(&board[ch][init_rank].piece, this)) {
                        if (board[ch][init_rank].piece.piece_type == KING) {
                            b->is_check = board[ch][init_rank].piece.piece_color;
                            b->is_check = board[ch][init_rank].piece.piece_color;
                            for(auto sq : tmp) {
                                b->checking_pieces_squares.push_back(sq);
                            }

                            tmp.clear();
                        } else {
                            tmp.push_back(&board[ch][init_rank]);
                        }
                    } else {
                        controlling_squares.push_back(&board[ch][init_rank]);
                    }
                    break;
                }

                if (check_found) {
                    for(auto sq : tmp) {
                        b->checking_pieces_squares.push_back(sq);
                    }

                    tmp.clear();
                    break;
                }

                tmp.push_back(&board[ch][init_rank]);

                init_rank+=i;
            }
        }

        if (b->is_check > -1) {
            for(auto square : b->checking_pieces_squares) {
                std::cout << square->file << square->rank << std::endl;
            }
            return;
        }

        for(int i = -1; i <= 1; i++) {
            if (i == 0) { continue; }

            init_file = cur_square[0]-1;
            init_rank = (cur_square[1] - '0')+i;

            if (init_rank <= 0 || init_rank > 8) { continue; }

            bool check_found = false;

            for(char ch = init_file; ch >= 'a'; ch--) {
                if (b->is_check
                    && !b->is_same_color(&board[ch][init_rank].piece, this)
                    && b->is_in_checking_pieces_squares(&board[ch][init_rank]))
                {
                    std::cout << "gg\n";
                }

                if (board[ch][init_rank].has_piece) {
                    if (!b->is_same_color(&board[ch][init_rank].piece, this)) {
                        if (board[ch][init_rank].piece.piece_type == KING) {
                            b->is_check = board[ch][init_rank].piece.piece_color;
                            for(auto sq : tmp) {
                                b->checking_pieces_squares.push_back(sq);
                            }

                            tmp.clear();
                        } else {
                            tmp.push_back(&board[ch][init_rank]);
                        }
                    } else {
                        controlling_squares.push_back(&board[ch][init_rank]);
                    }
                    break;
                }


                tmp.push_back(&board[ch][init_rank]);

                init_rank+=i;

                if (init_rank<=0 || init_rank>8) { break; }
            }
        }

        if (b->is_check > -1) {
            for(auto square : b->checking_pieces_squares) {
                std::cout << square->file << square->rank << std::endl;
            }
            return;
        }

    }

    if (piece_type == PAWN) {
        int rank_inc = (piece_color == P_WHITE ? 1 : -1);

        char file = this->get_file();
        int rank = this->get_rank();

        int new_rank = (rank) + rank_inc;

        if (b->is_check
            && b->is_in_checking_pieces_squares(&board[file][new_rank]))
        {
            std::cout << this->cur_square << std::endl;
        }
        if (!board[file][new_rank].has_piece) {
            tmp.push_back(&board[file][new_rank]);
        }

        if ((rank == 2 && piece_color == P_WHITE) || (rank == 7 && piece_color == P_BLACK) && !board[file][new_rank+rank_inc].has_piece) {
            // if the pawn has not yet moved, we should include a second legal square.
            tmp.push_back(&board[file][new_rank+rank_inc]);
        }
        
        // capturing
        for(int i = -1; i <= 1; i++) {
            if (i == 0) { continue; }

            char new_file = (char) (file+i);

            if (new_file < 'a' || new_file > 'h') { continue; }

            if (board[new_file][new_rank].has_piece && !b->is_same_color(&board[new_file][new_rank].piece, this)) {
                tmp.push_back(&board[new_file][new_rank]);
            }
        }
    }

    if (piece_type == KNIGHT) {
        std::vector<std::pair<int, int>> knight_moves = {
            {1, 2},
            {1, -2},
            {-1, 2},
            {-1, -2},
            {2, 1},
            {2, -1},
            {-2, 1},
            {-2, -1},
        };

        char file = this->get_file();
        int rank = this->get_rank();

        for(int i = 0; i < 8; i++) {
            char new_file = (char)(file+knight_moves[i].first);
            int new_rank = rank+knight_moves[i].second;

            if (new_file < 'a' || new_file > 'h' || new_rank < 1 || new_rank > 8) {
                continue;
            }

            if ((board[new_file][new_rank].has_piece && b->is_same_color(&board[new_file][new_rank].piece, this))) {
                controlling_squares.push_back(&board[new_file][new_rank]);
            } else {
                tmp.push_back(&board[new_file][new_rank]);
            }
        }
    }

    if (piece_type == KING) {
        char file = this->get_file();
        int rank = this->get_rank();

        std::vector<std::pair<int, int>> king_moves = {
            {1, 1},
            {1, -1},
            {-1, 1},
            {-1, -1},
            {0, 1},
            {0, -1},
            {1, 0},
            {-1, 0},
        };

        for(int i = 0; i < 8; i++) {
            char new_file = (char)(file+king_moves[i].first);
            int new_rank = rank+king_moves[i].second;
            if (new_file < 'a'
                || new_file > 'h'
                || new_rank < 1
                || new_rank > 8
                || (board[new_file][new_rank].has_piece && b->is_same_color(&board[new_file][new_rank].piece, this))) {
                continue;
            }

            int ok = 1;

            for(int idx = 1; idx <= 8; idx++) {
                for(char ch = 'a'; ch <= 'h'; ch++) {
                    if (board[ch][idx].piece.piece_color == piece_color) {
                        continue;
                    }

                    Square* square = &board[ch][idx];

                    if (square->piece.piece_type == KING) {
                        continue;
                    }

                    if (square->piece.piece_type == PAWN) {
                        int rank_inc = (square->piece.piece_color == P_WHITE ? 1 : -1);
                        char pawn_file = square->file;
                        int pawn_rank = square->rank;

                        if (((char)(pawn_file+1) == new_file || (char)(pawn_file-1) == new_file) && (pawn_rank+rank_inc) == new_rank) {
                            ok = 0;
                        }

                        continue;
                    }

                    if (square->has_piece && !b->is_same_color(&board[ch][idx].piece, this)) {
                        std::vector<Square*> legal_sq_vec = board[ch][idx].piece.get_legal_squares();
                        std::vector<Square*> ctrl_sq_vec = board[ch][idx].piece.controlling_squares;

                        for(int j = 0; j < legal_sq_vec.size(); j++) {
                            Square* sq = legal_sq_vec[j];

                            if (sq->file == new_file && sq->rank == new_rank) {
                                ok = 0;
                            }
                        }
                        for(int j = 0; j < ctrl_sq_vec.size(); j++) {
                            Square* sq = ctrl_sq_vec[j];

                            if (sq->file == new_file && sq->rank == new_rank) {
                                ok = 0;
                            }
                        }
                    }
                }
            }

            if (ok) {
                tmp.push_back(&board[new_file][new_rank]);
            }
        }
    }
    for(auto sq : tmp) {
        legal_squares.push_back(sq);
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
    int rankInc = 875;

    // to make the vector 1-based index
    for(char ch = 'a'; ch <= 'h'; ch++) {
        Square dummy;
        board[ch].push_back(dummy);
    }

    for(int idx = 1; idx <= 8; idx++) {
        int fileInc = 0;

        for(char ch = 'a'; ch <= 'h'; ch++) {
            int x = ch - 97 + 1;
            int y = idx;
            int s = x+y;

            Square sq;

            sq.file = ch;
            sq.rank = idx;

            sq.color = (s%2==1) ? RAYWHITE : DARKPURPLE;
            sq.x = fileInc;
            sq.y = rankInc;

            board[ch].push_back(sq);

            fileInc += SQUAREWIDTH;
        }

        rankInc -= SQUAREWIDTH;
    }

    Image r = LoadImage("./pieces/white/rook.png");
    Image n = LoadImage("./pieces/white/knight.png");
    Image b = LoadImage("./pieces/white/bishop.png");
    Image q = LoadImage("./pieces/white/queen.png");
    Image k = LoadImage("./pieces/white/king.png");
    Image p = LoadImage("./pieces/white/pawn.png");

    Texture2D tr = LoadTextureFromImage(r);
    Texture2D tn = LoadTextureFromImage(n);
    Texture2D tb = LoadTextureFromImage(b);
    Texture2D tq = LoadTextureFromImage(q);
    Texture2D tk = LoadTextureFromImage(k);
    Texture2D tp = LoadTextureFromImage(p);

    std::map<char, Texture2D> texture_map;
    texture_map['R'] = tr;
    texture_map['N'] = tn;
    texture_map['B'] = tb;
    texture_map['Q'] = tq;
    texture_map['K'] = tk;
    texture_map['P'] = tp;


    r = LoadImage("./pieces/black/rook.png");
    n = LoadImage("./pieces/black/knight.png");
    b = LoadImage("./pieces/black/bishop.png");
    q = LoadImage("./pieces/black/queen.png");
    k = LoadImage("./pieces/black/king.png");
    p = LoadImage("./pieces/black/pawn.png");

    tr = LoadTextureFromImage(r);
    tn = LoadTextureFromImage(n);
    tb = LoadTextureFromImage(b);
    tq = LoadTextureFromImage(q);
    tk = LoadTextureFromImage(k);
    tp = LoadTextureFromImage(p);

    texture_map['r'] = tr;
    texture_map['n'] = tn;
    texture_map['b'] = tb;
    texture_map['q'] = tq;
    texture_map['k'] = tk;
    texture_map['p'] = tp;

    // fen parsing
    std::istringstream fen_stream(fen);
    std::string token;
    std::vector<std::string> tokens;

    while (fen_stream >> token) {
        tokens.push_back(token);
    }

    int rank = 8;
    char file = 'a';

    for(int i = 0; i < tokens[0].length(); i++) {
        if (fen[i] >= 'a' && fen[i] <= 'z' || fen[i] >= 'A' && fen[i] <= 'Z') {
            board[file][rank].piece.image = texture_map[fen[i]];
            board[file][rank].piece.piece_color = (fen[i] >= 'a' && fen[i] <= 'z') ? P_BLACK : P_WHITE;
            board[file][rank].piece.piece_type = tolower(fen[i]);
            board[file][rank].piece.cur_square = file+std::to_string(rank);
            board[file][rank].has_piece = true;
        } else if (fen[i] >= '1' && fen[i] <= '8') {
            file = (char)(file + (fen[i] - '0' - 1));
            if (file > 'h') { file = 'a'; }
        }
        
        file++;

        if (fen[i]=='/') {
            rank--;
            file = 'a';
        }
    }

    turn = (tokens[1] == "w" ? P_WHITE : P_BLACK);
}


bool Board::is_same_color(Piece *piece1, Piece *piece2) {
    return piece1->piece_color == piece2->piece_color;
}

void Board::handle_check(int check_color) {
    //std::cout << (check_color == 0 ? "White is in check\n" : "Black is in check\n");
}

void Board::assign_check(Piece *piece) {
    this->is_check = piece->piece_color;
    this->checking_piece = piece->piece_type;
    this->checking_square = piece->cur_square;
}

bool Board::is_in_checking_pieces_squares(Square* square) {
    for(auto sq : checking_pieces_squares) {
        std::cout << sq->file << sq->rank << std::endl;
    }

    auto it = std::find_if(this->checking_pieces_squares.begin(), this->checking_pieces_squares.end(), [&](Square* sq) {
        return sq->file == square->file && sq->rank == square->rank;
    });

    if (it != checking_pieces_squares.end()) {
        return true;
    }

    return false;
}
