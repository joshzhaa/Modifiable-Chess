//Class that defines how the user interacts with the board

#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "Board.h"
#include <iosfwd>
#include <sstream>

class Terminal {
    private:
        Board board;
        std::stringstream log;
        std::istream& in;
        std::ostream& out;
        bool active = false;
#ifdef DEBUG
        struct Input {
            char file;
            int rank;
        };
        std::vector<Input> record;
#endif
        static constexpr char select_request[] = "Select a square or enter \"quit\"\n";
        static constexpr char move_request[] = "Input a move\n";
    public:
        Terminal(size_t height, size_t width);
        Terminal(size_t height, size_t width, std::istream& in, std::ostream& out);
        void start(); //sets active to true
        void stop(); //sets active to false
        void input(); //input calls Board::select or Board::move depending on Board state
        void output(); //output displays Board state in human viewable format
        bool is_active();
        Board& get_board();
#ifdef DEBUG
        void print_record(std::ostream& out);
#endif
};

#endif //__INTERFACE_H__
