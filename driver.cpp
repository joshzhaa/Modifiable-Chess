#include "Board.h"

#include <iostream>
#include <string>

using namespace std;

const string EXAMPLE_FEN = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
const string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

pair<int, int> parse_square(const string& cmd) {
    char rank, file;
    int row, col;

    try {
        file = cmd.at(0);
        rank = cmd.at(1);
        col = file - 'a';
        row = BOARD_HEIGHT - (rank - '0');
    }
    catch(...) {
        cout << "Invalid Square!\n";
    }
    return pair<int, int>(row, col);
}

void console_play(Board& board) {
    string command;
    while(true) {
        board.print_board();
        cout << "Select a square, or enter \"quit\"\n";
        cin >> command;
        if (command.find("quit") != string::npos) break;
        pair<int, int> square = parse_square(command);
        if (board.has_selection()) {
            board.move(square.first, square.second);
        }
        else {
            board.select(square.first, square.second);
        }
    } 
}

int main() {
    Board b;
    b.set_board(EXAMPLE_FEN);
    console_play(b);
    return 0;
}
