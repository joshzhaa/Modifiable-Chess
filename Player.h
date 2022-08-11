//Class declaration for Player representation

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Vector.h"
#include <vector>

/*
class holding information about each player. Each player must be defined by a "forward" direction for pawn movement,
a team id (such as white or black). Additionally all player objects share a static opposition matrix which defines
whether one player is opposed to another player (to determine captures and illegal movements due to collision).
*/
class Player {
    private:
        Vector pawn_direction; //Vector for pawn's normal forward motion
        int team; //essentially a player id, named team so duplicate players can easily have a meaning
        static std::vector<std::vector<bool>> opposition; //boolean matrix describing oppositions
        //increase size of opposition matrix if necessary
        //Players are default opposed
        void grow(size_t size);
    public:
        Player();
        //direction must point vertically or horizontally
        Player(int id_in, const Vector& direction);
        //returns true if this player is opposed to other
        bool is_opposed(const Player& other) const noexcept;
        //sets opposition matrix to new state, assumes opposition is a symmetric relation
        void set_opposed(const Player& other, bool setting) noexcept;
        //returns an int identifying this player's team
        int get_team() const noexcept;
        //returns this player's pawn movement direction
        Vector get_direction() const noexcept;
};


#endif //__PLAYER_H__
