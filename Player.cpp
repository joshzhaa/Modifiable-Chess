#include "Player.h"
#include "Board.h"

std::vector<std::vector<bool>> Player::opposition;

Player::Player() : pawn_direction{0, 1}, team(0) { grow(1); }

Player::Player(int id_in, const Vector& direction) : pawn_direction(direction), team(id_in) { grow(id_in + 1); }

void Player::grow(size_t size) {
    if (opposition.size() < size) {
        for (auto& row : opposition) row.resize(size, true);
        opposition.resize(size, std::vector<bool>(size, true)); 
    }
}

bool Player::is_opposed(const Player& other) const noexcept {
    return opposition[get_team()][other.get_team()];
}

void Player::set_opposed(const Player& other, bool setting) noexcept {
    opposition[get_team()][other.get_team()] = opposition[other.get_team()][get_team()] = setting;
}

int Player::get_team() const noexcept {
    return team;
}

Vector Player::get_direction() const noexcept {
    return pawn_direction;    
}
