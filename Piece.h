/*Class declarations for all the pieces as well as their necessary utilities
Accomodates the easy addition of new non-standard chess pieces
TO ADD NEW PIECES:
    (1) create new class which defines piece behavior
    (2) write show_moves to determine how piece moves, attack and extend are provided for this
    (3) add to piece_factory
*/

#ifndef __PIECE_H__
#define __PIECE_H__

class Vector;
class Player;
class Board;

//abstract base class from which King, Queen, etc. are derived
class Piece {
    private:
        char id; //char piece id in FEN format

    protected:
        Board* board; //tells piece which board it is on
        const Player* player; //the controlling player
        /* Mark a square as valid to move to, returns false when blocked
        out of bounds -> false, don't mark
        empty -> true, mark
        piece same team -> false, don't mark
        piece other team -> false, mark */
        bool attack(const Vector& position) const noexcept;
        //attack spaces in a direction repeatedly until blocks
        //direction must move one square on a grid
        void extend(const Vector& direction, Vector start) const noexcept;
    
    public:
        //assigns piece id, correcting for white or black side
        //structured this way to allow child classes to assign id in constructor
        Piece(Board* b_in, const Player* p_in, char id_in);
        
        //marks possible moves on Board::move_array as true
        virtual void show_moves(const Vector& position) const noexcept = 0;
        //returns the FEN format piece id
        char get_id() const noexcept { return id; }
        //returns the Player that controls this piece
        const Player& get_player() const noexcept { return *player; }
        //updates state of piece, K R and P and calls move side effects (which occur before the main movement of move)
        virtual void move(const Vector& /*start*/, const Vector& /*end*/) noexcept {}
        //all pieces pretend they haven't moved by default as long as it doesn't matter
        virtual bool unmoved() const noexcept { return true; }
        virtual ~Piece() {};
};

//Standard Chess Pieces
class King : public Piece {
    private:
        bool has_moved = false; //for castling

    public:
        King(Board* b_in, const Player* p_in) : Piece(b_in, p_in, 'K') {}

        virtual void show_moves(const Vector& position) const noexcept;
        virtual void move(const Vector& start, const Vector& end) noexcept;
        virtual bool unmoved() const noexcept { return !has_moved; }

        virtual ~King() {}
};

class Queen : public Piece {
    public: 
        Queen(Board* b_in, const Player* p_in) : Piece(b_in, p_in, 'Q') {}
        
        virtual void show_moves(const Vector& position) const noexcept;
        
        virtual ~Queen() {}
};

class Rook : public Piece {
    private:
        bool has_moved = false; //for castling

    public: 
        Rook(Board* b_in, const Player* p_in) : Piece(b_in, p_in, 'R') {}
        
        virtual void show_moves(const Vector& position) const noexcept;
        virtual void move(const Vector& /*start*/, const Vector& /*end*/) noexcept { has_moved = true; }
        virtual bool unmoved() const noexcept { return !has_moved; }

        virtual ~Rook() {}
};

class Bishop : public Piece {
    public: 
        Bishop(Board* b_in, const Player* p_in) : Piece(b_in, p_in, 'B') {} 
        
        virtual void show_moves(const Vector& position) const noexcept;

        virtual ~Bishop() {}
};

class Knight : public Piece {
    public: 
        Knight(Board* b_in, const Player* p_in) : Piece(b_in, p_in, 'N') {} 
        
        virtual void show_moves(const Vector& position) const noexcept;

        virtual ~Knight() {}
};

class Pawn : public Piece {
    private:
        bool has_moved = false; //for initial 2 space move
    
    public: 
        Pawn(Board* b_in, const Player* p_in) : Piece(b_in, p_in, 'P') {} 
        
        virtual void show_moves(const Vector& position) const noexcept;
        virtual void move(const Vector& start, const Vector& end) noexcept;
        virtual bool unmoved() const noexcept { return !has_moved; }


        virtual ~Pawn() {}
};

//return pointer to dynamically allocated Piece, nullptr if invalid piece_id
Piece* piece_factory(char piece_id, const Player* player, Board* board) noexcept;

#endif //__PIECE_H__
