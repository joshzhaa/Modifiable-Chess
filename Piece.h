//Class declarations for all the pieces in chess as well as their necessary utilities

#ifndef __PIECE_H__
#define __PIECE_H__

#include "Board.h"

using namespace std;

class Board; //forward declaration

//abstract base class from which King, Queen, etc. are derived
class Piece {
    private:
        char id; //char piece id in FEN format

    protected:
        //marks a location on move_array as valid to move into, returns true if occupied by another piece
        bool mark_at(Board* board, int row, int col);

    public:
        Piece() { id = 0; } //default constructor creates an Empty piece
        
        //assigns piece id, correcting for white or black side
        //structured this way to allow child classes to assign id in constructor
        Piece(bool is_white, char id_in);
        
        //marks possible moves on board->move_array as true
        virtual void show_moves(Board* board, int row, int col) = 0;
        
        //returns the FEN format piece id
        char get_id() { return id; }
        //updates state of piece, K R and P keep track of whether they have moved yet
        virtual void move() {}
        virtual ~Piece() {};
};

class King : public Piece {
    private:
        bool has_moved = false;

    public:
        King() : Piece(true, 'k') {}
        King(bool is_white) : Piece(is_white, 'k') {}
        
        virtual void show_moves(Board* board, int row, int col);
        virtual void move() { has_moved = true; }
        virtual ~King() {}
};

class Queen : public Piece {
    public:
        Queen() : Piece(true, 'q') {}
        Queen(bool is_white) : Piece(is_white, 'q') {}
        
        virtual void show_moves(Board* board, int row, int col);
        
        virtual ~Queen() {}
};

class Rook : public Piece {
    private:
        bool has_moved = false;
        
    public:
        Rook() : Piece(true, 'r') {}
        Rook(bool is_white) : Piece(is_white, 'r') {}
        
        virtual void show_moves(Board* board, int row, int col);
        virtual void move() { has_moved = true; }
        virtual ~Rook() {}
};

class Bishop : public Piece {
    public:
        Bishop() : Piece(true, 'b') {}
        Bishop(bool is_white) : Piece(is_white, 'b') {}
        
        virtual void show_moves(Board* board, int row, int col);

        virtual ~Bishop() {}
};

class Knight : public Piece {
    public:
        Knight() : Piece(true, 'n') {}
        Knight(bool is_white) : Piece(is_white, 'n') {}
        
        virtual void show_moves(Board* board, int row, int col);

        virtual ~Knight() {}
};

class Pawn : public Piece {
    private:
        bool has_moved = false;

    public:
        Pawn() : Piece(true, 'p') {}
        Pawn(bool is_white) : Piece(is_white, 'p') {}
        
        virtual void show_moves(Board* board, int row, int col);
        void move() { has_moved = true; }

        virtual ~Pawn() {}
};

#endif
