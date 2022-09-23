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
void Board::add_piece(const Vector& position, char piece_id, int team_id) {
    piece_array.at(position.y).at(position.x) = piece_factory(piece_id, &get_player(team_id), this);
}
void Board::add_player(int team_id, const Vector& pawn_direction) {
    players.emplace_back(team_id, pawn_direction);
}

void Board::set_fen(const std::string& mFEN) {
    //start is a reference, so this advances it, end can be a temporary
    auto parse_int = [](std::string::const_iterator& start, std::string::const_iterator end) {
        std::string number;
        while (start != end && (std::isdigit(*start) || *start == '-')) {
            number.push_back(*start++);
        }
        return std::stoi(number);
    };
#ifdef DEBUG
    auto debug = [](std::string::const_iterator start, std::string::const_iterator end) {
        std::cout << std::string(start, end) << '\n';
    };
#endif
    //player list
    auto it = mFEN.begin();
    while(*it != ' ') {
        int team_id = parse_int(it, mFEN.end());
        it += 2; //:(
        Vector pawn_direction;
        pawn_direction.x = parse_int(it, mFEN.end());
        ++it; //,
        pawn_direction.y = parse_int(it, mFEN.end());
        ++it; //)
        players.emplace_back(team_id, pawn_direction);
    }
    ++it; //space
    //piece placements
    for (int y = int(height()) - 1; y >= 0; --y) {
        for (int x = 0; x < int(width()); ++x) {
            //read one piece
            char piece_id = *it++;
            if (piece_id == '-') {
                piece_array.at(y).at(x) = nullptr;
                continue;
            }
            ++it; //:
            int team_id = parse_int(it, mFEN.end());
            //create piece
            add_piece(Vector{x, y}, piece_id, team_id);
        }
        ++it; //forward slash or in the last iteration space
    }
}
std::string Board::get_fen() {
    // (1) player list
    std::string fen;
    for (auto& player : players) {
        fen += std::to_string(player.get_team()) + std::string(":(") + std::to_string(player.get_direction().x) 
            + std::string(",") + std::to_string(player.get_direction().y) + std::string(")");
    }
    fen.push_back(' ');
    // (2) piece_placements 
    for (auto row = piece_array.rbegin(); row != piece_array.rend(); ++row) {
        for (auto& piece : *row) {
            if (piece) {
                fen.push_back(piece->get_id());
                fen += std::string(":") + std::to_string(piece->get_player().get_team());
            } else {
                fen += "-";
            }
        }
        fen.push_back('/');
    }
    fen.back() = ' ';
    // (3) Move status
    std::string move_fen;
    std::string passant_fen;
    for (int y = int(height()) - 1; y >= 0; --y) {
        for (int x = 0; x < int(width()); ++x) {
            Piece* piece = piece_array.at(y).at(x);
            if (piece && !piece->unmoved()) {
                move_fen += "(" + std::to_string(x) + "," + std::to_string(y) + ")";
            }
            if (piece && en_passantable({x, y})) {
                passant_fen += "(" + std::to_string(x) + "," + std::to_string(y) + ")";
            }
        }
    }
    if (move_fen.empty()) {
        fen += "- ";
    } else {
        fen += move_fen;
        fen.push_back(' ');
    }
    // (4) En Passant attacking square
    if (passant_fen.empty()) {
        fen += "- ";
    } else {
        fen += passant_fen;
        fen.push_back(' ');
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
    if (!in_bounds(position)) {
        deselect();
        return false;
    }
    Piece* ptr = piece_array.at(position.y).at(position.x);
    if (!ptr || ptr->get_player().get_team() != players[turn].get_team()) {
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
    int starting_occupancy = occupancy();
    char target_id = target->get_id();
    target->move(selection, position); //hides other state-altering function calls
    piece_array.at(position.y).at(position.x) = target; //actually move piece
    target = nullptr;
    deselect();
    history.push_back({get_fen(), selection, position, players[turn].get_team(), target_id, 
        occupancy() < starting_occupancy});
    //history.emplace_back(get_fen(), selection, position, players[turn].get_team(), target_id, 
        //occupancy() < starting_occupancy); confused why this line doesn't work, but pragmatic concerns are more 
        //important. (C++20 aggregrate initialization with parentheses)
    turn = (turn + 1) % players.size();
    return true;
}
void Board::mark(const Vector& position) {
    move_array.at(position.y).at(position.x) = true;
}
//Get information about instance of Board
bool Board::has_selection() const noexcept {
    return selection.x != -1;
}
bool Board::is_occupied(const Vector& position) const {
    return in_bounds(position) && piece_array.at(position.y).at(position.x);
}
bool Board::in_bounds(const Vector& position) const noexcept {
    return position.x < int(width()) && position.y < int(height()) && position.x >= 0 && position.y >= 0;
}
bool Board::is_attacked(const Vector& position, const Player& attacker) {
    std::vector<std::vector<bool>> temp(move_array);
    for (int y = int(height()) - 1; y >= 0; --y) {
        for (int x = 0; x < int(width()); ++x) {
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
bool Board::is_valid(const Vector& position) const noexcept {
    return in_bounds(position) && move_array[position.y][position.x];
}
size_t Board::width() const {
    return piece_array.at(0).size();
}
size_t Board::height() const noexcept {
    return piece_array.size();
}
const Piece* Board::get_piece(const Vector& position) const {
    return piece_array.at(position.y).at(position.x);
}
const Player& Board::get_player(int team_id) const {
    return *std::find_if(players.begin(), players.end(), [=](const Player& player) {
        return player.get_team() == team_id;
    });
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
        return move.piece == 'P' || move.capture;
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
    auto end = start;
    for (size_t i = 0; i < players.size(); ++i) { //adding iterator past end is undefined behavior
        if (end == history.rend()) break;
        ++end;
    }
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
    size_t rank = height();
    for (auto row = piece_array.rbegin(); row != piece_array.rend(); ++row) {
        std::cout << "   "; //left margin
        for (size_t i = 0; i < width(); ++i) std::cout << "+---";
        std::cout << "+\n " << rank-- << " "; //end of border and beginning of next line margin
        for (size_t i = 0; i < width(); ++i) { //fill in squares
            Piece* piece = row->at(i);
            char highlight = move_array.at(height() - rank - 1).at(i) ? '*' : ' ';
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
    for (size_t i = 0; i < width(); ++i) {
        std::cout << "+---";
    }
    std::cout << "+\n  ";
    for (size_t i = 0; i < width(); ++i) {
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
