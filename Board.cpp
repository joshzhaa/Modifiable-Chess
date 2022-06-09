#include "Board.h"

#include <iostream>

Board::Board() : piece_array(BOARD_HEIGHT, vector<Piece*>(BOARD_WIDTH, nullptr)),
    move_array(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false)),
    selection(-1, -1) { initialize_print_map(); }

void Board::initialize_print_map() {
    print_map['K'] = "\u2654";
    print_map['Q'] = "\u2655";
    print_map['R'] = "\u2656";
    print_map['B'] = "\u2657";
    print_map['N'] = "\u2658";
    print_map['P'] = "\u2659";
    print_map['k'] = "\u265A";
    print_map['q'] = "\u265B";
    print_map['r'] = "\u265C";
    print_map['b'] = "\u265D";
    print_map['n'] = "\u265E";
    print_map['p'] = "\u265F";
}

void Board::print_board() {
    for (auto& row : piece_array) {
        for (size_t i = 0; i < row.size(); ++i) {
            cout << "+---";
        }
        cout << "+\n";
        for (auto& piece : row) {
            cout << '|'; 
            if (piece) {
                cout << print_map.at(piece->get_id());
            }
            else {
                cout << "   ";
            }
        }
        cout << "|\n";
    }
    for (size_t i = 0; i < BOARD_WIDTH; ++i) {
        cout << "+---";
    }
    cout << "+\n";
}


