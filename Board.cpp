#include "Board.h"

#include <iostream>

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

Board::Board() : piece_array(BOARD_HEIGHT, vector<Piece*>(BOARD_WIDTH, nullptr)),
    move_array(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false)),
    selection(-1, -1) { initialize_print_map(); }

void Board::add(char piece_id, int row, int col) {
    Piece* ptr;
    bool is_white = false;
    if (isupper(piece_id)) is_white = true;
    piece_id = (char) tolower(piece_id);

    switch (piece_id) {
        case 'k':
            ptr = new King(is_white); 
            break;
        case 'q':
            ptr = new Queen(is_white);
            break;
        case 'r':
            ptr = new Rook(is_white);
            break;
        case 'b':
            ptr = new Bishop(is_white);
            break;
        case 'n':
            ptr = new Knight(is_white);
            break;
        case 'p':
            ptr = new Pawn(is_white);
            break;
        default:
            ptr = nullptr;
            cout << "Board::add; Illegal piece type\n";
    }
    piece_array.at(row).at(col) = ptr;
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
                cout << ' ' << /*print_map.at(*/piece->get_id()/*)*/ << ' ';
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

Board::~Board() {
    for (auto& row : piece_array) {
        for (auto col : row) { //At the moment, I don't actually know what happens if you delete a reference-to-pointer
            delete col;
        }
    }
}
