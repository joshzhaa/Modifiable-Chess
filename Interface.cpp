#include "Interface.h"
#include "Piece.h"
#include <iostream>

Terminal::Terminal(size_t height, size_t width) : board(height, width), in(std::cin), out(std::cout) {}
Terminal::Terminal(size_t height, size_t width, std::istream& in, std::ostream& out) : board(height, width), in(in), out(out) {}

void Terminal::start() {
    active = true;
}
void Terminal::stop() {
    active = false;
}
void Terminal::input() {
    out << (board.has_selection() ? move_request : select_request);
    char file;
    int rank;
    in >> file;
    in >> rank;
    if (!in) { //quits game if it fails to read an int
        stop();
        return;
    }
    Vector target{file - 'a', rank};
    if (board.has_selection()) {
        if (board.move(target)) {
            const Vector& begin = board.get_selection();
            out << char(begin.x + 'a') << begin.y << " moved to " << file << rank << '\n';
        } else {
            out << "Invalid move\n";
        }
    } else {
        if (board.select(target)) {
            out << "Selected " << board.get_piece(target)->get_id() << " at " << file << rank << '\n';
        } else {
            out << "Invalid selection\n";
        }
    }
}
void Terminal::output() {
    for (int y = board.height() - 1; y >= 0; --y) {
        //upper border
        for (int x = 0; x < int(board.width()); ++x) out << "+-------";
        out << "+\n";
        //upper padding
        for (int x = 0; x < int(board.width()); ++x) out << "|       ";
        out << "|\n";
        //middle layer: piece id
        for (int x = 0; x < int(board.width()); ++x) {
            out << "|  ";
            Vector target{x, y};
            if (board.is_occupied(target)) {
                const Piece* ptr = board.get_piece(target);
                out << ptr->get_id() << ':' << ptr->get_player().get_team();
            } else {
                out << "   ";
            }
            out << "  ";
        }
        out << "|\n";
        //lower padding
        for (int x = 0; x < int(board.width()); ++x) out << "|       ";
        out << "|\n";
    }
    for (int x = 0; x < int(board.width()); ++x) out << "+-------";
    out << "+\n";
}
bool Terminal::is_active() {
    return active;
}
Board& Terminal::get_board() {
    return board;
}

