#include "Piece.h"

#include <utility>

//Determines the max distance a Q, R, or B can move
#define MAX_DISTANCE 20

bool Piece::mark_at(Board* board, int row, int col) {
    try {
        bool occupancy = board->is_occupied(row, col);
        if (!occupancy || isupper(id) != isupper(board->get_piece(row, col))) {
            board->move_array.at(row).at(col) = true;
        }
        return occupancy;
    }
    catch (const out_of_range &oor) {
        return false;
    }
}

Piece::Piece(bool is_white, char id_in) {
    if (is_white) {
        id = (char) toupper(id_in);
    }
    else {
        id = (char) tolower(id_in);
    }
}

void King::show_moves(Board* board, int row, int col) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i || j) {
                mark_at(board, row + i, col + j);
            }
        }
    }
}

void Queen::show_moves(Board* board, int row, int col) {
    int infinity = MAX_DISTANCE;
    //project down
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row + i, col)) break;
    }
    //down right
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row + i, col + i)) break;
    }
    //right
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row, col + i)) break;
    }
    //up right
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row - i, col + i)) break;
    }
    //up
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row - i, col)) break;
    }
    //up left
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row - i, col - i)) break;
    }
    //left
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row, col - i)) break;
    }
    //down left
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row + i, col - i)) break;
    }
}

void Rook::show_moves(Board* board, int row, int col) {
    int infinity = MAX_DISTANCE;
    //project down
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row + i, col)) break;
    }
    //right
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row, col + i)) break;
    }
    //up
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row - i, col)) break;
    }
    //left
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row, col - i)) break;
    }
}

void Bishop::show_moves(Board* board, int row, int col) {
    int infinity = MAX_DISTANCE;
    //down right
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row + i, col + i)) break;
    }
    //up right
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row - i, col + i)) break;
    }
    //up left
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row - i, col - i)) break;
    }
    //down left
    for (int i = 1; i < infinity; ++i) {
        if (mark_at(board, row + i, col - i)) break;
    }
}

void Knight::show_moves(Board* board, int row, int col) {
    //top 2
    mark_at(board, row - 2, col + 1);
    mark_at(board, row - 2, col - 1);
    //right 2
    mark_at(board, row + 1, col + 2);
    mark_at(board, row - 1, col + 2);
    //bottom 2
    mark_at(board, row + 2, col + 1);
    mark_at(board, row + 2, col - 1);
    //left 2
    mark_at(board, row + 1, col - 2);
    mark_at(board, row - 1, col - 2);
}

void Pawn::show_moves(Board* board, int row, int col) {
    int distance = has_moved ? 1 : 2;
    int direction = isupper(get_id()) ? -1 : 1;
    if (!board->is_occupied(row + (direction * distance), col)) mark_at(board, row + (direction * distance), col);
    //capture right
    int cap_row = row + direction;
    int cap_col = col + 1;
    if (board->is_occupied(cap_row, cap_col) && isupper(board->get_piece(cap_row, cap_col)) != isupper(get_id()))
        mark_at(board, cap_row, cap_col);
    //capture left
    cap_col = col - 1;
    if (board->is_occupied(cap_row, cap_col) && isupper(board->get_piece(cap_row, cap_col)) != isupper(get_id()))
        mark_at(board, cap_row, cap_col);
}


