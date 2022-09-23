#include "Piece.h"
#include "Board.h"
#include "Player.h"

//Piece
bool Piece::attack(const Vector& position) const noexcept {
    bool empty = !board->is_occupied(position);
    if (empty || (board->in_bounds(position) && player->is_opposed(board->get_piece(position)->get_player()))) {
        board->mark(position);
    }
    return empty;
}

void Piece::extend(const Vector& direction, Vector start) const noexcept {
    do {
        start += direction;
    } while (attack(start));
}

Piece::Piece(Board* b_in, const Player* p_in, char id_in) : id(id_in), board(b_in), player(p_in) {}

void King::show_moves(const Vector& position) const noexcept {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i || j) attack(position + Vector{i, j}); 
        }
    }
    auto directions = board->castle_conditions();
    for (const auto& direction : directions) {
        attack(position + 2 * direction);
    }
}
void King::move(const Vector& start, const Vector& end) noexcept {

}

void Queen::show_moves(const Vector& position) const noexcept {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i || j) extend({i, j}, position);
        }
    }
}

void Rook::show_moves(const Vector& position) const noexcept {
    extend({0, -1}, position); //project down 
    extend({1, 0}, position); //right 
    extend({0, 1}, position); //up 
    extend({-1, 0}, position); //left
}

void Bishop::show_moves(const Vector& position) const noexcept {
    extend({1, -1}, position); //down right
    extend({1, 1}, position); //up right
    extend({-1, 1}, position); //up left
    extend({-1, -1}, position); //down left
}

void Knight::show_moves(const Vector& position) const noexcept {
    //left 2
    attack({position.x - 2, position.y + 1});
    attack({position.x - 2, position.y - 1});
    //top 2
    attack({position.x + 1, position.y + 2});
    attack({position.x - 1, position.y + 2});
    //right 2
    attack({position.x + 2, position.y + 1});
    attack({position.x + 2, position.y - 1});
    //down 2
    attack({position.x + 1, position.y - 2});
    attack({position.x - 1, position.y - 2});
}

void Pawn::show_moves(const Vector& position) const noexcept {
    int distance = has_moved ? 1 : 2;
    Vector direction = player->get_direction();
    if (!board->is_occupied(position + direction)) {
        attack(position + direction);
        if (!board->is_occupied(position + distance * direction)) attack(position + distance * direction);
    }
    //generate an orthogonal vector to the pawn's direction only works on vectors with one 0 component
    Vector offset;
    offset.x = !direction.x;
    offset.y = !direction.y;
    //capture right
    auto conditional_attack = [&](const Vector& capture_pos, const Vector& passant_pos) {
        bool capture = board->is_occupied(capture_pos) && player->is_opposed(board->get_piece(capture_pos)->get_player());
        bool passant = board->en_passantable(passant_pos) && player->is_opposed(board->get_piece(passant_pos)->get_player());
        if (can_capture || can_passant) {
            attack(capture_pos);
        }
    };
    conditional_attack(position + direction + offset, position + offset);
    //capture left
    conditional_attack(position + direction - offset, position - offset);
}

void Pawn::move(const Vector& start, const Vector& end) noexcept {
    has_moved = true;
    Vector displacement = end - start;
    if (!board->is_occupied(end) && displacement.x && displacement.y) { //if en passant
        board->en_passant(end); 
    }
}

Piece* piece_factory(char piece_id, const Player* player, Board* board) noexcept {
    switch (piece_id) {
        case 'K':
            return new King(board, player);
        case 'Q':
            return new Queen(board, player);
        case 'R':
            return new Rook(board, player);
        case 'B':
            return new Bishop(board, player);
        case 'N':
            return new Knight(board, player);
        case 'P':
            return new Pawn(board, player);
        default:
            return nullptr;
    }
}
