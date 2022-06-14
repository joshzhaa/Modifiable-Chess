#include "Board.h"

#include <algorithm>
#include <iostream>

Board::Board() : piece_array(BOARD_HEIGHT, vector<Piece*>(BOARD_WIDTH, nullptr)),
    move_array(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false)),
    selection(-1, -1) {}

Board::Board(const string& FEN) : piece_array(BOARD_HEIGHT, vector<Piece*>(BOARD_WIDTH, nullptr)),
    move_array(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false)),
    selection(-1, -1) { initialize_print_map(); set_board(FEN); }


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

void Board::set_rules(bool setting) noexcept {
    enforce_rules = setting;
}
void Board::set_castling(bool setting) noexcept {
    allow_castling = setting;
}
void Board::set_compatibility(bool setting) noexcept {
    compatibility = setting;
    initialize_print_map();
}

void Board::reset_selection() noexcept {
    fill_board(move_array, false);
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
        if (ptr->get_control() != turn) return false;
        ptr->show_moves(this, row, col);
        selection.first = row;
        selection.second = col;
    }
    else {
        reset_selection();
    }
    if (enforce_rules) check_moves();
    return ptr;
}

void Board::check_moves() noexcept {
    for (size_t row = 0; row < move_array.size(); ++row) {
        for(size_t col = 0; col < move_array[0].size(); ++col) {
            //if piece thinks it can move there, make the move, check if it self-checks, make adjustments
            if (move_array[row][col]) {
                Piece *temp = piece_array[row][col];
                piece_array[row][col] = piece_array.at(selection.first).at(selection.second);
                if (in_check(turn)) move_array[row][col] = false;
                piece_array[selection.first][selection.second] = piece_array[row][col];
                piece_array[row][col] = temp;
            }
        }
    }
}

//Attempts to move selected piece to position defined by parameters
//Returns true and updates piece_array, selection attribute, and turn if successful
//Returns false and updates nothing if unsuccessful
bool Board::move(int row, int col) {
    if (enforce_rules && !move_array.at(row).at(col)) {
        reset_selection();
        return false;
    }
    if (allow_castling && tolower(piece_array.at(selection.first).at(selection.second)->get_id()) == 'k' && 
    (col - selection.second == 2 || col - selection.second == -2)) {//detect castling, only move that moves 2 pieces
        //move king
        piece_array.at(row).at(col) = piece_array.at(selection.first).at(selection.second);
        piece_array.at(selection.first).at(selection.second) = nullptr;
        //move rook
        is_unmoved_rook comp; 
        if (col - selection.second == 2) { //if castle right
            auto it = piece_array.at(row).begin() + selection.second + 1;
            auto found_it = find_if(it, piece_array.at(row).end(), comp);
            piece_array.at(row).at(col - 1) = *found_it;
            *found_it = nullptr;
        }
        else { //if castle left
            auto it = piece_array.at(row).rbegin() + selection.second;
            auto found_it = find_if(it, piece_array.at(row).rend(), comp);
            piece_array.at(row).at(col + 1) = *found_it;
            *found_it = nullptr;
        }
    }
    else {
        piece_array.at(row).at(col) = piece_array.at(selection.first).at(selection.second);
        piece_array.at(selection.first).at(selection.second) = nullptr;
    }
    //check if pawn needs promotion
    if (row == BOARD_HEIGHT - 1 && piece_array.at(row).at(col)->get_id() == 'p') {
        delete piece_array.at(row).at(col);
        add('q', row, col);
    }
    if (row == 0 && piece_array.at(row).at(col)->get_id() == 'P') {
        delete piece_array.at(row).at(col);
        add('Q', row, col);
    }
    reset_selection();
    piece_array.at(row).at(col)->move(); //tell piece that it has been moved
    turn = turn == Player::white ? Player::black : Player::white;
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
                cout << highlight << print_map.at(piece->get_id()) << highlight; 
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

//iterates through all pieces to determine whether the position checks the player defined by color
bool Board::in_check(Player color) noexcept {
    vector<vector<bool>> temp(move_array); //TODO: is necessary? store move_array to switch back later
    //find appropriate king
    char target_id = color == Player::white ? 'K' : 'k';
    size_t kings_row = -1;
    size_t kings_col = -1;
    for (auto row = piece_array.begin(); row != piece_array.end(); ++row) {
        auto found = find_if(row->begin(), row->end(), [ target_id ](Piece *ptr) {
            if (ptr) return ptr->get_id() == target_id;
            return false;
        });
        if (found != row->end()) {
            kings_row = row - piece_array.begin();
            kings_col = found - row->begin();
            break;
        }
    }
    //Get each piece to show possible moves and determine if they are attacking the king
    for (int row = 0; row < (int) piece_array.size(); ++row) {
        for (int col = 0; col < (int) piece_array.at(0).size(); ++col) {
            Piece* ptr = piece_array.at(row).at(col);
            if (ptr && color != ptr->get_control()) {
                ptr->show_moves(this, row, col);
                if (move_array.at(kings_row).at(kings_col)) {
                    move_array = temp;
                    return true;
                }
            }
        }
    }
    move_array = temp;
    return false;
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

size_t Board::get_height() const noexcept {
    return piece_array.size();
}

size_t Board::get_width() const noexcept {
    return piece_array[0].size();
}

Player Board::get_turn() const noexcept {
    return turn;
}

Board::~Board() noexcept {
    for (auto& row : piece_array) {
        for (auto& piece : row) {
            delete piece;
        }
    }
}

//Converts FEN ids to special chess symbol unicode characters
//or causes print_map to do nothing
void Board::initialize_print_map() noexcept {
    if (compatibility) {
        print_map['K'] = "K";
        print_map['Q'] = "Q";
        print_map['R'] = "R";
        print_map['B'] = "B";
        print_map['N'] = "N";
        print_map['P'] = "P";
        print_map['k'] = "k";
        print_map['q'] = "q";
        print_map['r'] = "r";
        print_map['b'] = "b";
        print_map['n'] = "n";
        print_map['p'] = "p";
    }
    else {
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
}
