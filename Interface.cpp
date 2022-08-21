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
    if (!in) { //quits game if it fails to read an int or istream is otherwise invalid
        stop();
        return;
    }
    Vector target{file - 'a', rank - 1};
    if (board.has_selection()) {
        const Vector begin = board.get_selection(); //needs to be a copy since Board::selection is modified
        if (board.move(target)) {
            log << char(begin.x + 'a') << (begin.y + 1) << " moved to " << file << rank << '\n';
        } else {
            log << "Invalid move\n";
        }
    } else {
        if (board.select(target)) {
            log << "Selected " << board.get_piece(target)->get_id() << " at " << file << rank << '\n';
        } else {
            log << "Invalid selection\n";
        }
    }
}
void Terminal::output() {
    for (int y = board.height() - 1; y >= 0; --y) {
        //upper border
        for (int x = 0; x < int(board.width()); ++x) out << "+-------";
        out << "+\n";
        //upper padding
        auto pad = [&]() {
            for (int x = 0; x < int(board.width()); ++x) {
                char highlight = board.is_valid(Vector{x, y}) ? '*' : ' ';
                out << "| " << highlight << "   " << highlight << " ";
            }
            out << "|\n";
        };
        pad();
        //middle layer: piece id
        for (int x = 0; x < int(board.width()); ++x) {
            out << "|  ";
            Vector target{x, y};
            if (board.is_occupied(target)) {
                const Piece* ptr = board.get_piece(target);
                out << ptr->get_id() << ':' << ptr->get_player().get_team();
            } else {
                out << ' ' << (board.is_valid(Vector{x, y}) ? '*' : ' ') << ' ';
            }
            out << "  ";
        }
        out << "|\n";
        //lower padding
        pad();
    }
    for (int x = 0; x < int(board.width()); ++x) out << "+-------";
    out << "+\n";
    out << "mFEN = " << board.get_fen() << '\n';
    out << log.rdbuf();
    out.clear();
}
bool Terminal::is_active() {
    return active;
}
Board& Terminal::get_board() {
    return board;
}

