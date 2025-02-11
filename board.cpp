#include "board.h"

vector<Square*> Piece::getLegalSquares(map<char, vector<Square>> &board) {
    legalSquares.clear();
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
                        legalSquares.push_back(&board[ch][init_rank]);
                    }
                    break;
                }
                legalSquares.push_back(&board[ch][init_rank]);
            }

            // vertical 
            for(int idx = init_rank+i; idx <= 8 && idx >= 1; idx+=i) {
                if (board[init_file][idx].has_piece) {
                    if (board[init_file][idx].piece.piece_color != piece_color) {
                        legalSquares.push_back(&board[init_file][idx]);
                    }
                    break;
                }
                legalSquares.push_back(&board[init_file][idx]);
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
                        legalSquares.push_back(&board[ch][init_rank]);
                    }
                    break;
                }
                legalSquares.push_back(&board[ch][init_rank]);
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
                        legalSquares.push_back(&board[ch][init_rank]);
                    }
                    break;
                }
                legalSquares.push_back(&board[ch][init_rank]);
                init_rank+=i;
                if (init_rank<=0 || init_rank>8) { break; }
            }
        }
    }

    if (piece_type == KING) {
    }

    if (piece_type == KNIGHT) {
    }

    if (piece_type == PAWN) {
    }

    return legalSquares;
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
