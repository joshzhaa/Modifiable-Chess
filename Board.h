//Class declaration for a class representing the Board

#ifndef __BOARD_H__
#define __BOARD_H__

#include "Player.h"

#include <vector>
#include <string>

class Piece;
/*
Class stores pieces on the board geometrically inside a std::vector of the same shape as the board.
Board uses the Vector struct declared above to uniquely identify each square. The board is the 1st quadrant
Board can be in 2 states, has_selection = false or = true. In these states, the Board expects to see a move
or expects to see a piece selection. Selected piece is identified by Vector selection. Valid moves for selection 
are declared by each Piece in piece_array by writing a true into move_array. Turns advance in the order of
the std::vector of Player objects.
*/
class Board {
    private: 
        std::vector<std::vector<Piece*>> piece_array; //geometric representation of board, nullptr represents empty
        std::vector<std::vector<bool>> move_array; //array showing allowed moves 
        std::vector<Player> players; //this lists players in turn order
        std::vector<Player>::iterator turn; //turn points to current Player with active turn
        //Position of user's most recently selected square
        //selection.x = -1 is a special value which signifies no selection
        Vector selection; 
        
    public:
        Board(size_t height, size_t width); //Constructs an empty board
        Board(const std::string& mFEN, size_t height, size_t width); //Calls set_fen(FEN) 
        //Board setup
        const Piece& add_piece(const Vector& position, char piece_id, const Player& player); //construct Piece at position
        const Player& add_player(int team_id, const Vector& pawn_direction);
        void set_fen(const std::string& mFEN); //set board to state according to mFEN
        std::string get_fen(); //set board to state according to mFEN
        //Gameplay
        bool select(const Vector& position); //select Piece at position
        void deselect() noexcept; //set selection.x = -1
        bool move(const Vector& position); //move Piece at selection to position
        void mark(const Vector& position); //mark move_array at position to be true
        //Get information about instance of Board
        bool has_selection() const noexcept; //get state of Board
        bool is_occupied(const Vector& position) const;
        bool in_bounds(const Vector& position) const noexcept;
        bool is_attacked(const Vector& postition, const Player& attacker); //is position being attacked by player
        size_t get_width() const; //REQUIRES more than one rank to be on the board
        size_t get_height() const noexcept;
        const Piece* get_piece(const Vector& position) const;
        const Player& get_player(size_t i) const;
        const Vector& get_selection() const noexcept;
        //Special conditions/actions
        std::vector<Vector> castle_conditions() noexcept; //returns vectors in direction of rook
        void castle() const noexcept;
        bool en_passantable(const Piece* pawn) const noexcept;
        void en_passant() const noexcept;
#ifdef DEBUG
        void print() const noexcept;
#endif
        ~Board();
};

#endif //__BOARD_H__
