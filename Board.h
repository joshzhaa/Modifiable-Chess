//Class declaration for a class representing the Board

#ifndef __BOARD_H__
#define __BOARD_H__

#include "Piece.h"

#include <unordered_map>
#include <vector>
#include <utility>
#include <string>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

using namespace std;

class Piece; //forward declaration

//Class stores pieces on the board geometrically inside a vector of the same shape as the board
//Board uses a (row, col) format such as (3, 4) to identify squares as a typical 2D array in computer science
//as opposed to the chess convention of rank and file
//Board can be in 2 states, has_selection = true or = false. Either a square has just been selected
//or a move has just been made (equivalent to starting state)
class Board {
    private:
        //array containing piece objects, nullptr represents empty
        vector<vector<Piece*>> piece_array;
        //array showing allowed moves
        vector<vector<bool>> move_array;
        //Position of user's most recently selected square
        pair<int, int> selection; //selection = (-1, -1) is a special value which means no selection
        
        bool enforce_rules = false;

        unordered_map<char, string> print_map; //string because char is not big enough for unicode character
        void initialize_print_map() noexcept;
        
        //Allows piece to mark its own possible moves by accessing move_array
        friend class Piece;
    public:
        Board(); //Constructs an empty board
        Board(const string& FEN); //Calls set_board(FEN)

        void add(char piece_id, int row, int col) noexcept;
        void set_board(const string& FEN);
        void set_rules(bool setting);
       
        bool select(int row, int col);
        void reset_selection() noexcept;
        bool move(int row, int col);
        
        bool has_selection() const noexcept;
        bool is_occupied(int row, int col) const noexcept;
        char get_piece(int row, int col) const;
        
        void print_board() const noexcept;
       
        ~Board();
};

#endif
