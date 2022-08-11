#include "Board.h"
#include <iostream>

//create board "manually"
int main() {
    Board b(8, 8);
    b.add_player(0, {0, 1});
    b.add_player(1, {0, -1});
    const Player& white = b.get_player(0);
    const Player& black = b.get_player(1);
    for (int i = 0; i < 8; ++i) {
        b.add_piece({i, 1}, 'P', white);
    }
    for (int i = 0; i < 8; ++i) {
        b.add_piece({i, 6}, 'P', black);
    }
    b.add_piece({0, 0}, 'R', white);
    b.add_piece({1, 0}, 'N', white);
    b.add_piece({2, 0}, 'B', white);
    b.add_piece({3, 0}, 'Q', white);
    b.add_piece({4, 0}, 'K', white);
    b.add_piece({5, 0}, 'B', white);
    b.add_piece({6, 0}, 'N', white);
    b.add_piece({7, 0}, 'R', white);

    b.add_piece({0, 7}, 'R', black);
    b.add_piece({1, 7}, 'N', black);
    b.add_piece({2, 7}, 'B', black);
    b.add_piece({3, 7}, 'Q', black);
    b.add_piece({4, 7}, 'K', black);
    b.add_piece({5, 7}, 'B', black);
    b.add_piece({6, 7}, 'N', black);
    b.add_piece({7, 7}, 'R', black);
#ifdef DEBUG
    b.print();
#endif
    std::cout << b.get_fen() << '\n';
}
