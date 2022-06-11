#include "Board.h"

#include <iostream>
#include <string>

using namespace std;

//FEN examples
const string EXAMPLE_FEN = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
const string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

//Parsing a selection
pair<int, int> parse_selection(const string& cmd) noexcept {
    char rank, file;
    int row, col;

    try {
        file = cmd.at(0);
        rank = cmd.at(1);
        col = file - 'a';
        row = BOARD_HEIGHT - (rank - '0');
    }
    catch(...) {
        row = col = -1;
        cout << "Wrong selection format (e.g. d4)!\n";
    }
    return pair<int, int>(row, col);
}

//User facing console messages
const string SELECTION_REQUEST = "Select a square or enter \"quit\"\n";
const string MOVE_REQUEST = "Input a move\n";

//Function to facilitate a gameplay loop at the command line
void console_play(Board* board) {
    string command;
    while(true) {
        board->print_board();
        if (board->has_selection()) {
            cout << MOVE_REQUEST;
        }
        else {
            cout << SELECTION_REQUEST;
        }
        cin >> command;
        if (command.find("quit") != string::npos) break;

        pair<int, int> square = parse_selection(command);
        if (!(square.first == -1 && square.second == -1)) {
            if (board->has_selection()) {
                if (board->move(square.first, square.second)) {
                    cout << "Moved to " << command << "\n";
                }
                else {
                    cout << "Invalid Move!\n";
                }
            }
            else {
                if (board->select(square.first, square.second)) {
                    cout << "Selected " << board->get_piece(square.first, square.second) << "\n";
                }
                else {
                    cout << "Invalid Selection!\n";
                }
            }
        }
    } 
}

int main() {
    Board* b = new Board(EXAMPLE_FEN);
    //b->set_rules(true);
    console_play(b);
    delete b;
    return 0;
}
