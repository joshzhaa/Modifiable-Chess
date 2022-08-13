#include "Board.h"
#include <iostream>

//create board "manually"
int main() {
    Board b(8, 8);
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
