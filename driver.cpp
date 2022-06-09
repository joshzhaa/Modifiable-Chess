#include "Board.h"

#include <string>

const string EXAMPLE_FEN = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
const string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main() {
    Board b;
    b.set_board(EXAMPLE_FEN);
    b.print_board();
    return 0;
}
