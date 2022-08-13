//Class that defines how the user interacts with the board

#include "Board.h"
#include <iosfwd>

class Terminal {
    private:
        std::istream& in;
        std::ostream& out;
        bool active = true;
    public:
        Terminal() : in(std::cin), out(std::cout) {}
        Terminal(istream& in, ostream& out) : in(in), out(out) {}

        void start();
        void input();
        void output();
        bool is_active()
};
