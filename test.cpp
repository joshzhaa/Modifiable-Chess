//#include "Board.h"
#include "Interface.h"
#include <iostream>


//create board "manually"
void test_manual(Board& b) {
    b.add_player(0, {0, 1});
    b.add_player(1, {0, -1});
    for (int i = 0; i < 8; ++i) {
        b.add_piece({i, 1}, 'P', 0);
    }
    for (int i = 0; i < 8; ++i) {
        b.add_piece({i, 6}, 'P', 1);
    }
    b.add_piece({0, 0}, 'R', 0);
    b.add_piece({1, 0}, 'N', 0);
    b.add_piece({2, 0}, 'B', 0);
    b.add_piece({3, 0}, 'Q', 0);
    b.add_piece({4, 0}, 'K', 0);
    b.add_piece({5, 0}, 'B', 0);
    b.add_piece({6, 0}, 'N', 0);
    b.add_piece({7, 0}, 'R', 0);

    b.add_piece({0, 7}, 'R', 1);
    b.add_piece({1, 7}, 'N', 1);
    b.add_piece({2, 7}, 'B', 1);
    b.add_piece({3, 7}, 'Q', 1);
    b.add_piece({4, 7}, 'K', 1);
    b.add_piece({5, 7}, 'B', 1);
    b.add_piece({6, 7}, 'N', 1);
    b.add_piece({7, 7}, 'R', 1);
#ifdef DEBUG
    b.print();
#endif
    std::cout << b.get_fen() << '\n';
}

//test terminal interface
int main() {
    std::ios_base::sync_with_stdio(false);
    Terminal interface(8, 8);
    interface.get_board().set_fen("0:(0,1)1:(0,-1) R:1N:1B:1Q:1K:1B:1N:1R:1/P:1P:1P:1P:1P:1P:1P:1P:1/--------/--------/--------/--------/P:0P:0P:0P:0P:0P:0P:0P:0/R:0N:0B:0Q:0K:0B:0N:0R:0 - - 0 0 1");
    //test_manual(interface.get_board());
    interface.start();
    while (interface.is_active()) {
        interface.output();
        interface.input();
    }
#ifdef DEBUG
    std::cout << "input record:\n";
    interface.print_record(std::cout);
#endif
}
