//Class declaration for a class representing the Board

#ifndef __BOARD_H__
#define __BOARD_H__

#include "Piece.h"

#include <vector>
#include <utility>
#include <string>

#include <unordered_map>

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
        //Keeps track of whose turn it is, updated by move
        Player turn = Player::white;

        //game options
        bool enforce_rules = false;
        bool allow_castling = true;
        bool compatibility = true; //print_board only prints FEN ids instead of special chess characters
        
        //Printing special chess symbol characters only works on some terminals
        //Currently tested: VSCode terminal
        unordered_map<char, string> print_map;
        void initialize_print_map() noexcept;
        //Allows piece to mark its own possible moves by accessing move_array
        //More crucially facilitates castling
        friend class Piece;
        friend void King::show_moves(Board*, int, int) const; 
    public:
        Board(); //Constructs an empty board
        Board(const string& FEN); //Calls set_board(FEN)
        
        //Board setup
        void add(char piece_id, int row, int col) noexcept;
        void set_board(const string& FEN);
        void set_rules(bool setting) noexcept;
        void set_castling(bool setting) noexcept;
        void set_compatibility(bool setting) noexcept;
        //Gameplay
        bool select(int row, int col);
        void reset_selection() noexcept;
        bool move(int row, int col);
        //Get information about instance of Board
        bool in_check(Player color) const noexcept;
        bool has_selection() const noexcept;
        bool has_moved(int row, int col) const;
        bool is_occupied(int row, int col) const noexcept;
        char get_piece(int row, int col) const;
        size_t get_width() const noexcept;
        size_t get_height() const noexcept;
         
        void print_board() const noexcept;
       
        ~Board();
};

#endif
