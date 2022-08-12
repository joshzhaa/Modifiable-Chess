#include "Board.h"
#include "Piece.h"
#include "Player.h"

#include <string>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
#endif

//Board
Board::Board(size_t height, size_t width) : piece_array(height, std::vector<Piece*>(width, nullptr)),
    move_array(height, std::vector<bool>(width, false)), players(), selection{-1, -1} {}

Board::Board(const std::string& mFEN, size_t height, size_t width) : Board(height, width) {
    set_fen(mFEN);
}
//Board setup
const Piece& Board::add_piece(const Vector& position, char piece_id, const Player& player) {
    return *(piece_array.at(position.y).at(position.x) = piece_factory(piece_id, &player, this));
}
const Player& Board::add_player(int team_id, const Vector& pawn_direction) {
    players.emplace_back(team_id, pawn_direction);
    return players.back();
}
void Board::set_fen(const std::string& mFEN) {
    //player list
    
    //piece placements
    auto it = mFEN.begin();
    for (auto row = piece_array.rbegin(); row != piece_array.rend(); ++row) {
        for (auto& piece : *row) {
            //read one piece
            char piece_id;
            char c = '0';
            std::string player_id;
            piece_id = *it++;
            ++it; //skip colon
            while (c != ',') {
                c = *it++;
                player_id.push_back(c);
            }
            ++it;
            //create piece
        }
    }
    //sort player array
}
std::string Board::get_fen() {
    // (1) player list
    std::string fen;
    for (auto& player : players) {
        fen += std::to_string(player.get_team()) + std::string(":(") + std::to_string(player.get_direction().x) 
            + std::string(",") + std::to_string(player.get_direction().y) + std::string("),");
    }
    fen.back() = ' ';
    // (2) piece_placements 
    for (auto row = piece_array.rbegin(); row != piece_array.rend(); ++row) {
        int empty_count = 0;
        for (auto& piece : *row) {
            if (piece) {
                if (empty_count) {
                    fen += std::to_string(empty_count) + std::string(",");
                    empty_count = 0;
                } else {
                    fen.push_back(piece->get_id());
                    fen += std::string(":") + std::to_string(piece->get_player().get_team()) + std::string(",");
                }
            } else {
                ++empty_count;
            }
        }
        if (empty_count) {
            fen += std::to_string(empty_count) + std::string(",");
        }
        fen.back() = '/';
    }
    fen.back() = ' ';
    // (3) Move status
    std::string move_fen;
    std::string passant_fen;
    for (int y = int(get_height()) - 1; y >= 0; --y) {
        for (int x = 0; x < int(get_width()); ++x) {
            Piece* piece = piece_array.at(y).at(x);
            if (piece && !piece->unmoved()) {
                move_fen += "(" + std::to_string(x) + "," + std::to_string(y) + "),";
            }
            if (piece && en_passantable({x, y})) {
                passant_fen += "(" + std::to_string(x) + "," + std::to_string(y) + "),";
            }
        }
    }
    if (move_fen.empty()) {
        fen += "- ";
    } else {
        fen += move_fen;
        fen.back() = ' ';
    }
    // (4) En Passant attacking square
    if (passant_fen.empty()) {
        fen += "- ";
    } else {
        fen += passant_fen;
        fen.back() = ' ';
    }
    // (5) Current turn
    fen += std::to_string(turn) + " ";
    // (6) Half-move clock
    fen += std::to_string(halfmove_clock()) + " ";
    // (7) Full-move clock
    fen += std::to_string(fullmove_clock());
    return fen;
}
//Gameplay
bool Board::select(const Vector& position) {
    Piece* ptr = piece_array.at(position.y).at(position.x);
    if (!in_bounds(position) || ptr->get_player().get_team() != players[turn].get_team()) {
        deselect();
        return false;
    }
    ptr->show_moves(position);
    selection = position;
    return true;
}
void Board::deselect() noexcept {
    for (auto&& row : move_array) {
        for (auto&& col : row) {
            col = false;
        }
    }
    selection.x = -1;
}
bool Board::move(const Vector& position) {
    if (!in_bounds(position) || !move_array.at(position.y).at(position.x)) {
        deselect();
        return false;
    }
    Piece*& target = piece_array.at(selection.y).at(selection.x);
    target->move(selection, position); //hides other state-altering function calls
    piece_array.at(position.y).at(position.x) = target;
    target = nullptr;
    deselect(); 
    turn = (turn + 1) % players.size();
    history.emplace_back(get_fen(), selection.x, selection.y, position.x, position.y, players[turn].get_team(), 
        target->get_id());
    return true;
}
void Board::mark(const Vector& position) {
    move_array.at(position.y).at(position.x) = true;
}
//Get information about instance of Board
bool Board::has_selection() const noexcept {
    return selection.x == -1;
}
bool Board::is_occupied(const Vector& position) const {
    return piece_array.at(position.y).at(position.x);
}
bool Board::in_bounds(const Vector& position) const noexcept {
    return position.x < int(get_width()) && position.y < int(get_height()) && position.x >= 0 && position.y >= 0;
}
bool Board::is_attacked(const Vector& position, const Player& attacker) {
    std::vector<std::vector<bool>> temp(move_array);
    for (int y = int(get_height()) - 1; y >= 0; --y) {
        for (int x = 0; x < int(get_width()); ++x) {
            Piece* piece = piece_array[y][x];
            if (piece->get_player().get_team() == attacker.get_team()) {
                piece->show_moves({x, y});
                if (move_array.at(position.y).at(position.x)) return true;
            }
        }
    }
    move_array.swap(temp);
    return false;
}
size_t Board::get_width() const {
    return piece_array.at(0).size();
}
size_t Board::get_height() const noexcept {
    return piece_array.size();
}
const Piece* Board::get_piece(const Vector& position) const {
    return piece_array.at(position.y).at(position.x);
}
const Player& Board::get_player(size_t i) const {
    return players.at(i);
}
const Vector& Board::get_selection() const noexcept {
    return selection;
}
const std::vector<Move>& Board::get_history() const noexcept {
    return history;
}
int Board::halfmove_clock() const noexcept {
    int count = -1;
    auto it = std::find_if(history.rbegin(), history.rend(), [&](const Move& move) {
        ++count;
        Board past(move.state, get_height(), get_width());
        return move.piece == 'P' || past.occupancy() > occupancy();
    });
    return it == history.rend() ? int(history.size()) : count;
}
int Board::fullmove_clock() const noexcept {
    return int((history.size() / 2) + 1);
}
int Board::occupancy() const noexcept {
    int count = 0;
    for (auto& row : piece_array) {
        for (auto& piece : row) {
            count += bool(piece);
        }
    }
    return count;
}
//Special conditions/actions
std::vector<Vector> Board::castle_conditions() noexcept {
    std::vector<Vector> direction_list;
    direction_list.reserve(4);
    std::vector<Vector> candidates = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    Piece* king = piece_array.at(selection.y).at(selection.x);
    //search in all directions
    for (const auto& candidate : candidates) {
        Vector search = selection;
        bool attacked_flag = false;
        auto is_null = [&] (const Vector& location) {
            return !piece_array.at(location.y).at(location.x);
        };
        while(is_null(search += candidate)) {
            for (auto& player : players) { //make sure no attacked squares are crossed
                if (player.is_opposed(king->get_player()) && is_attacked(search, player)) {
                    attacked_flag = true;
                    break;
                }
            }
        }
        if (!attacked_flag && piece_array.at(search.y).at(search.x)->get_id() == 'R') {
            direction_list.push_back(candidate);
        }
    }
    return direction_list;
}
void Board::castle() noexcept {
    
}
bool Board::en_passantable(const Vector& position) const noexcept {
    if (!in_bounds(position) || !get_piece(position) || get_piece(position)->get_id() != 'P' || history.empty()) {
        return false;
    }
    //if this pawn has been double moved in the last players.size() moves, return true, if not, false
    int target_team = get_piece(position)->get_player().get_team();
    auto start = history.rbegin();
    auto end = start + players.size();
    if (end >= history.rend()) end = history.rend();
    auto it = std::find_if(start, end, [&](const Move& move) {
        return move.piece == 'P' && move.team == target_team && inf_norm(move.end - move.start) == 2;
    });
    return it != end;
}
void Board::en_passant(const Vector& position) noexcept {
    Vector spot = position - players[turn].get_direction();
    piece_array.at(spot.y).at(spot.x) = nullptr;
}
//debug
#ifdef DEBUG
void Board::print() const noexcept {
    size_t rank = get_height();
    for (auto row = piece_array.rbegin(); row != piece_array.rend(); ++row) {
        std::cout << "   "; //left margin
        for (size_t i = 0; i < get_width(); ++i) std::cout << "+---";
        std::cout << "+\n " << rank-- << " "; //end of border and beginning of next line margin
        for (size_t i = 0; i < get_width(); ++i) { //fill in squares
            Piece* piece = row->at(i);
            char highlight = move_array.at(get_height() - rank - 1).at(i) ? '*' : ' ';
            std::cout << "|"; //space border
            if (piece) { //if a piece is occupying
                std::cout << highlight << piece/*->get_id()*/->get_player().get_team() << highlight; 
            } else { //if there is no occupying piece
                std::cout << ' ' << highlight << ' ';
            }
        }
        std::cout << "|\n"; //space border
    }
    std::cout << "   ";
    for (size_t i = 0; i < get_width(); ++i) {
        std::cout << "+---";
    }
    std::cout << "+\n  ";
    for (size_t i = 0; i < get_width(); ++i) {
        std::cout << "   " << char('a' + i);
    }
    std::cout << "\n";

}
#endif

Board::~Board() {
    for (auto& row : piece_array) {
        for (auto& piece : row) {
            if (piece) delete piece;
        }
    }
}
