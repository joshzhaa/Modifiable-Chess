#include "Piece.h"

#include <utility>
#include <iostream> //TODO: remove

//Determines the max distance a Q, R, or B can move
#define MAX_DISTANCE 20

bool Piece::mark_at(vector<vector<bool>>& move_array, int row, int col) {
    try {
        move_array.at(row).at(col) = true;
        return true;
    }
    catch (const out_of_range &oor) {
        cout << "Piece::mark_at; caught out of range\n";
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

void King::show_moves(vector<vector<bool>>& move_array, int row, int col) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            mark_at(move_array, row + i, col + j);
        }
    }
    move_array.at(row).at(col) = false;
}

void Queen::show_moves(vector<vector<bool>>& move_array, int row, int col) {
    int infinity = MAX_DISTANCE;
    //project down
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row + i, col);
    }
    //down right
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row + i, col + i);
    }
    //right
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row, col + i);
    }
    //up right
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row - i, col + i);
    }
    //up
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row - i, col);
    }
    //up left
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row - i, col - i);
    }
    //left
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row, col - i);
    }
    //down left
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row + i, col - i);
    }
}

void Rook::show_moves(vector<vector<bool>>& move_array, int row, int col) {
    int infinity = MAX_DISTANCE;
    //project down
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row + i, col);
    }
    //right
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row, col + i);
    }
    //up
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row - i, col);
    }
    //left
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row, col - i);
    }
}

void Bishop::show_moves(vector<vector<bool>>& move_array, int row, int col) {
    int infinity = MAX_DISTANCE;
    //down right
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row + i, col + i);
    }
    //up right
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row - i, col + i);
    }
    //up left
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row - i, col - i);
    }
    //down left
    for (int i = 1; i < infinity; ++i) {
        mark_at(move_array, row + i, col - i);
    }
}

void Knight::show_moves(vector<vector<bool>>& move_array, int row, int col) {
    //top 2
    mark_at(move_array, row - 2, col + 1);
    mark_at(move_array, row - 2, col - 1);
    //right 2
    mark_at(move_array, row + 1, col + 2);
    mark_at(move_array, row - 1, col + 2);
    //bottom 2
    mark_at(move_array, row + 2, col + 1);
    mark_at(move_array, row + 2, col - 1);
    //left 2
    mark_at(move_array, row + 1, col - 2);
    mark_at(move_array, row - 1, col - 2);
}

void Pawn::show_moves(vector<vector<bool>>& move_array, int row, int col) {
    int distance = has_moved ? 1 : 2;
    int direction = isupper(get_id()) ? -1 : 1;
    mark_at(move_array, row + (direction * distance), col);
}


