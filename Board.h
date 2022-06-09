//Class declaration for a class representing the Board, which includes all the pieces

#ifndef __BOARD_H__
#define __BOARD_H__

#include "Piece.h"

#include <unordered_map>
#include <vector>
#include <utility>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

class Board {
    private:
        //array containing piece objects, nullptr represents empty
        vector<vector<Piece*>> piece_array;
        //array showing allowed moves
        vector<vector<bool>> move_array;
        //Position of user's most recently selected square
        pair<int, int> selection; //selection = (-1, -1) is a special value which means no selection
        
        unordered_map<char, string> print_map; //string because char is not big enough for unicode character
        void initialize_print_map();
    public:
        Board();
        void print_board();
};


#endif
