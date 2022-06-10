#include "Board.h"

#include <algorithm>
#include <iostream>

Board::Board() : piece_array(BOARD_HEIGHT, vector<Piece*>(BOARD_WIDTH, nullptr)),
    move_array(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false)),
    selection(-1, -1) { /*initialize_print_map();*/ }

Board::Board(const string& FEN) : piece_array(BOARD_HEIGHT, vector<Piece*>(BOARD_WIDTH, nullptr)),
    move_array(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false)),
    selection(-1, -1) { /*initialize_print_map();*/ set_board(FEN); }


void Board::add(char piece_id, int row, int col) noexcept { //apparently new can throw, but would it actually?
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
            cout << "Board::add; Illegal piece type = " << piece_id << "\n";
    }
    piece_array.at(row).at(col) = ptr;
}

//intended usage is for setting values of a board-sized 2d vector to 0
template <typename Board, typename val_t>
void fill_board(Board& board, val_t val) noexcept {
    for (auto& row : board) {
        fill(row.begin(), row.end(), val);
    }
}

//clears board then sets board to state described by FEN notation
//if ill-formed notation, behavior is undefined
void Board::set_board(const string& FEN) {
    fill_board(piece_array, nullptr);
    fill_board(move_array, false);
    auto it = FEN.begin();

    for (int row = 0; row < BOARD_HEIGHT; ++row) {
        for (int col = 0; col < BOARD_WIDTH; ++col) {
            if (isdigit(*it)) {
                col += (*it - '1');
                ++it;
            }
            else {
                add(*it++, row, col);
            }
        }
        ++it; //expected to be '/'
    }
}

void Board::set_rules(bool setting) {
    enforce_rules = setting;
}

void Board::reset_selection() noexcept {
    selection.first = selection.second = -1;
}

//Attempts to select piece at position defined by parameters
//Returns true if the selection is valid, false if invalid
bool Board::select(int row, int col) {
    Piece* ptr = nullptr;
    try {
        ptr = piece_array.at(row).at(col);
    }
    catch (const out_of_range& oor) {
        ptr = nullptr;
    }

    if (ptr) {
        ptr->show_moves(this, row, col);
        selection.first = row;
        selection.second = col;
    }
    else {
        reset_selection();
        fill_board(move_array, false); 
    }
    return ptr;
}

//Attempts to move selected piece to position defined by parameters
//Returns true and updates piece_array and selection attribute if successful
//Returns false and updates nothing if unsuccessful
bool Board::move(int row, int col) {
    if (enforce_rules && !move_array.at(row).at(col)) {
        return false;
    }
    piece_array.at(row).at(col) = piece_array.at(selection.first).at(selection.second);
    piece_array.at(selection.first).at(selection.second) = nullptr;
    reset_selection();
    fill_board(move_array, false);
    piece_array.at(row).at(col)->move(); //tell piece that it has been moved
    return true;
}



//Prints board to cout using standard ASCII characters
void Board::print_board() const noexcept {

    int rank = BOARD_HEIGHT;
    for (auto& row : piece_array) {
        cout << "   "; //left margin
        for (size_t i = 0; i < row.size(); ++i) { //space border
            cout << "+---";
        }
        cout << "+\n " << rank-- << " "; //end of border and beginning of next line margin
        for (size_t i = 0; i < row.size(); ++i) { //fill in squares
            Piece* piece = row.at(i);
            char highlight = move_array.at(BOARD_HEIGHT - rank - 1).at(i) ? '*' : ' ';

            cout << "|"; //space border
            if (piece) { //if a piece is occupying
                cout << highlight << /*print_map.at(*/piece->get_id()/*)*/ << highlight; 
            }
            else { //if there is no occupying piece
                cout << ' ' << highlight << ' ';
            }
        }
        cout << "|\n"; //space border
    }
    cout << "   ";
    for (size_t i = 0; i < BOARD_WIDTH; ++i) {
        cout << "+---";
    }
    cout << "+\n  ";
    for (size_t i = 0; i < BOARD_WIDTH; ++i) {
        cout << "   " << char('a' + i);
    }
    cout << "\n";
}

bool Board::has_selection() const noexcept {
    return selection.first != -1 || selection.second != -1;
}

bool Board::is_occupied(int row, int col) const noexcept {
    try {
        return piece_array.at(row).at(col);
    }
    catch (const out_of_range& oor) {
        return false;
    }
}

char Board::get_piece(int row, int col) const {
    Piece* ptr = piece_array.at(row).at(col);
    if (!ptr) {
        throw invalid_argument("Board::get_piece; empty square");
    }
    return ptr->get_id();
}

Board::~Board() noexcept {
    for (auto& row : piece_array) {
        for (auto& piece : row) {
            delete piece;
        }
    }
}

//Currently useless as terminal doesn't support special chess symbol unicode characters
void Board::initialize_print_map() noexcept {
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
