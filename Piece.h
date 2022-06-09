//Class declarations for all the pieces in chess as well as their necessary utilities

#ifndef __PIECE_H__
#define __PIECE_H__

#include <vector>

using namespace std;

//abstract base class from which King, Queen, etc. are derived
class Piece {
    private:
        char id; //char piece id in FEN format

    protected:
        //marks a location on move_array as valid to move into, if out of bounds does nothing
        void mark_at(vector<vector<bool>>& move_array, int row, int col);

    public:
        Piece() { id = 0; } //default constructor creates an Empty piece
        
        //assigns piece id, correcting for white or black side
        //structured this way to allow child classes to assign id in constructor
        Piece(bool is_white, char id_in);
        
        char get_id() { return id; }
        virtual void show_moves() = 0;
};

//Represents the King
class King : public Piece {
    public:
        King() : Piece(true, 'k') {}
        King(bool is_white) : Piece(is_white, 'k') {}
        
        void show_moves(vector<vector<bool>>& move_array, pair<int, int>& pos);
};




#endif
