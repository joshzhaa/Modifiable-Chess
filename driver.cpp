#include "Board.h"

int main() {
    Board b;
    b.add('K', 3, 3);b.add('K', 2, 3);b.add('K', 1, 3);b.add('K', 0, 3);b.add('K', 4, 3);b.add('K', 5, 3);b.add('K', 6, 3);
    b.print_board();
    return 0;
}
