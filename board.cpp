#include "board.h"

vector<Square*> Piece::get_legal_squares(map<char, vector<Square>> &board) {
    legal_squares.clear();
    if (piece_type == QUEEN || piece_type == ROOK) {
        // horizontal and vertical tracking
        char init_file = cur_square[0];
        int init_rank = cur_square[1] - '0';

        for(int i = -1; i <= 1; i++) {
            if (i == 0) { continue; }

            // horizontal 
            for(char ch = init_file+i; ch <= 'h' && ch >= 'a'; ch+=i) {
                if (board[ch][init_rank].has_piece) {
                    if (board[ch][init_rank].piece.piece_color != piece_color) {
                        legal_squares.push_back(&board[ch][init_rank]);
                    }
                    break;
                }
                legal_squares.push_back(&board[ch][init_rank]);
            }

            // vertical 
            for(int idx = init_rank+i; idx <= 8 && idx >= 1; idx+=i) {
                if (board[init_file][idx].has_piece) {
                    if (board[init_file][idx].piece.piece_color != piece_color) {
                        legal_squares.push_back(&board[init_file][idx]);
                    } else {
                        controlling_squares.push_back(&board[init_file][idx]);
                    }
                    break;
                }
                legal_squares.push_back(&board[init_file][idx]);
            }
        }
    }

    if (piece_type == QUEEN || piece_type == BISHOP) {
        // diagonal tracking
        char init_file = cur_square[0];
        int init_rank = cur_square[1] - '0';

        for(int i = -1; i <= 1; i++) {
            if (i == 0) { continue; }
            init_file = cur_square[0];
            init_rank = (cur_square[1] - '0')+i;
            if (init_rank<=0 || init_rank>8) { continue; }
            for(char ch = init_file+1; ch <= 'h'; ch++) {
                if (board[ch][init_rank].has_piece) {
                    if (board[ch][init_rank].piece.piece_color != piece_color) {
                        legal_squares.push_back(&board[ch][init_rank]);
                    } else {
                        controlling_squares.push_back(&board[ch][init_rank]);
                    }
                    break;
                }
                legal_squares.push_back(&board[ch][init_rank]);
                init_rank+=i;
            }
        }

        for(int i = -1; i <= 1; i++) {
            if (i == 0) { continue; }
            init_file = cur_square[0]-1;
            init_rank = (cur_square[1] - '0')+i;
            if (init_rank<=0 || init_rank>8) { continue; }
            for(char ch = init_file; ch >= 'a'; ch--) {
                if (board[ch][init_rank].has_piece) {
                    if (board[ch][init_rank].piece.piece_color != piece_color) {
                        legal_squares.push_back(&board[ch][init_rank]);
                    } else {
                        controlling_squares.push_back(&board[ch][init_rank]);
                    }
                    break;
                }
                legal_squares.push_back(&board[ch][init_rank]);
                init_rank+=i;
                if (init_rank<=0 || init_rank>8) { break; }
            }
        }
    }

    if (piece_type == PAWN) {
        int rank_inc = (piece_color == P_WHITE ? 1 : -1);

        char file = cur_square[0];
        int rank = (cur_square[1] - '0');

        int new_rank = (cur_square[1] - '0') + rank_inc;

        if (!board[file][new_rank].has_piece) {
            legal_squares.push_back(&board[file][new_rank]);
        }

        if ((rank == 2 && piece_color == P_WHITE) || (rank == 7 && piece_color == P_BLACK) && !board[file][new_rank+rank_inc].has_piece) {
            // if the pawn has not yet moved, we should include a second legal square.
            legal_squares.push_back(&board[file][new_rank+rank_inc]);
        }
        
        // capturing
        for(int i = -1; i <= 1; i++) {
            if (i == 0) { continue; }
            char new_file = (char) (file+i);
            if (new_file < 'a' || new_file > 'h') { continue; }
            if (board[new_file][new_rank].has_piece && board[new_file][new_rank].piece.piece_color != piece_color) {
                legal_squares.push_back(&board[new_file][new_rank]);
            }
        }
    }

    if (piece_type == KNIGHT) {
        vector<pair<int, int>> knight_moves = {
            {1, 2},
            {1, -2},
            {-1, 2},
            {-1, -2},
            {2, 1},
            {2, -1},
            {-2, 1},
            {-2, -1},
        };

        char file = cur_square[0];
        int rank = (cur_square[1] - '0');

        for(int i = 0; i < 8; i++) {
            char new_file = (char)(file+knight_moves[i].first);
            int new_rank = rank+knight_moves[i].second;

            if (new_file < 'a' || new_file > 'h' || new_rank < 1 || new_rank > 8) {
                continue;
            }
            if ((board[new_file][new_rank].has_piece && board[new_file][new_rank].piece.piece_color == piece_color)) {
                controlling_squares.push_back(&board[new_file][new_rank]);
            } else {
                legal_squares.push_back(&board[new_file][new_rank]);
            }
        }
    }

    if (piece_type == KING) {
        char file = cur_square[0];
        int rank = (cur_square[1] - '0');

        vector<pair<int, int>> king_moves = {
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
                || (board[new_file][new_rank].has_piece && board[new_file][new_rank].piece.piece_color == piece_color)) {
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

                    if (square->has_piece && square->piece.piece_color != piece_color) {
                        vector<Square*> legal_sq_vec = board[ch][idx].piece.get_legal_squares(board);
                        vector<Square*> ctrl_sq_vec = board[ch][idx].piece.controlling_squares;
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
                legal_squares.push_back(&board[new_file][new_rank]);
            }
        }
    }

    return legal_squares;
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
            fileInc += squareWidth;
        }
        rankInc -= squareWidth;
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

    map<char, Texture2D> texture_map;
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

    int rank = 8;
    char file = 'a';

    // fen parsing
    for(int i = 0; i < fen.length(); i++) {
        if (fen[i] >= 'a' && fen[i] <= 'z' || fen[i] >= 'A' && fen[i] <= 'Z') {
            board[file][rank].piece.image = texture_map[fen[i]];
            board[file][rank].piece.piece_color = (fen[i] >= 'a' && fen[i] <= 'z') ? P_BLACK : P_WHITE;
            board[file][rank].piece.piece_type = tolower(fen[i]);
            board[file][rank].piece.cur_square = file+to_string(rank);
            board[file][rank].has_piece = true;
        }
        file++;
        if (fen[i]=='/') {
            rank--;
            file = 'a';
        }
    }
}
