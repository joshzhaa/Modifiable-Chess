//Class that defines how the user interacts with the board

#include "Board.h"
#include <iosfwd>

class Console {
    private:
        std::istream& in;
        std::ostream& out;
        bool active = true;
    public:
        Console() : in(std::cin), out(std::cout) {}
        Console(istream& in, ostream& out) : in(in), out(out) {}

        void start();
        void input();
        void output();
        bool is_active()
};
