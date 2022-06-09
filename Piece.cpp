#include "Piece.h"

#include <utility>
#include <iostream> //TODO: remove

//Piece
void Piece::mark_at(vector<vector<bool>>& move_array, int row, int col) {
    try {
        move_array.at(row).at(col) = true;
    }
    catch (const out_of_range &oor) {
        cout << "Piece::mark_at; caught out of range\n";
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

//King
void King::show_moves(vector<vector<bool>>& move_array, pair<int, int>& pos) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            mark_at(move_array, pos.first, pos.second);
        }
    }
    move_array.at(pos.first).at(pos.second) = false;
}


