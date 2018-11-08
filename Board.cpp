//
// Created by karthik on 12/9/18.
//

/*
 * Board class to maintain the state of the game internally
 */

#include "Board.h"
#include <sstream>
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;


Point::Point(const Point &p): xcord(p.xcord), ycord(p.ycord) {
    if (p.piece != nullptr)
        piece = new Piece(*p.piece);
    else
        piece = nullptr;
}

Point& Point::operator=(const Point& p){
    if (this != &p) // protect against invalid self-assignment
    {
        xcord = p.xcord;
        ycord = p.ycord;
        piece = new Piece(*p.piece);
    }
    // by convention, always return *this
    return *this;
}

bool Board::is_position_valid(pair<int, int> position) {
    return abs(position.first) <= n && abs(position.second) <= n;
}

Board::Board(const Board &b) : rings_vector(b.rings_vector), opp_rings_vector(b.opp_rings_vector), game_board(b.game_board) {
    n = b.n; m = b.m; k = b.k; l = b.l; player_color = b.player_color; other_color = b.other_color;
    num_moves_played = b.num_moves_played; num_rings_on_board = b.num_rings_on_board; num_opp_rings_on_board = b.num_opp_rings_on_board;
    num_markers = b.num_markers; num_opp_markers = b.num_opp_markers;
}


Board::Board(int n, int m, int k, int l, char player_col, char other_col) : n(n), l(l), m(m), k(k),
                                                                            player_color(player_col),
                                                                            other_color(other_col) {
    num_rings_on_board = 0;
    num_opp_rings_on_board = 0;
    num_markers = 0;
    num_opp_markers = 0;
    num_moves_played = 0;

    // Constructs the game board arrays
    for (int i = -n; i <= n; i++) {
        game_board[i];
    }
    for (int i = -n; i <= n; i++) {
        if (i == 0) {
            for (int j = -(n - 1); j <= (n - 1); j++)
                game_board[i].insert(make_pair(j, Point(i, j)));
        }
        else if (i == n) {
            for (int j = 1; j <= n - 1; j++)
                game_board[i].insert(make_pair(j, Point(i, j)));
        }
        else if (i == -n) {
            for (int j = -n + 1; j <= -1; j++)
                game_board[i].insert(make_pair(j, Point(i, j)));
        }
        else if (i > 0) {
            for (int j = -(n - i); j <= n; j++)
                game_board[i].insert(make_pair(j, Point(i, j)));
        }
        else {
            for (int j = -n; j <= n + i; j++)
                game_board[i].insert(make_pair(j, Point(i, j)));
        }
    }

}

// place piece at given position
void Board::place_piece(char type, char color, pair<int, int> position) {
    game_board.at(position.first).at(position.second).piece = new Piece(type, color, position.first, position.second);
    if (type == 'r') {
        if (color == player_color) {
            rings_vector.push_back(position);
            num_rings_on_board++;
        }
        else {
            opp_rings_vector.push_back(position);
            num_opp_rings_on_board++;
        }
    }
    else {
        if (color == player_color)
            num_markers++;
        else
            num_opp_markers++;
    }
}

// remove piece from position
void Board::remove_piece(pair<int, int> position) {
    if (game_board.at(position.first).at(position.second).piece->type == 'r') {
        if (game_board.at(position.first).at(position.second).piece->color == player_color) {
            for (int i = 0; i < num_rings_on_board; i++) {
                // if (rings_vector.at(i) == position) {
                if ((rings_vector.at(i).first == position.first) & (rings_vector.at(i).second == position.second)) {
                    rings_vector.erase(rings_vector.begin() + i);
                    break;
                }
            }
            num_rings_on_board--;
        }
        else {
            for (int i = 0; i < num_opp_rings_on_board; i++) {
                // if (rings_vector.at(i) == position) {
                if ((opp_rings_vector.at(i).first == position.first) &
                    (opp_rings_vector.at(i).second == position.second)) {
                    opp_rings_vector.erase(opp_rings_vector.begin() + i);
                    break;
                }
            }
            num_opp_rings_on_board--;
        }
    }
    else {
        if (game_board.at(position.first).at(position.second).piece->color == player_color)
            num_markers--;
        else
            num_opp_markers--;
    }
    delete game_board.at(position.first).at(position.second).piece;
    game_board.at(position.first).at(position.second).piece = nullptr;
}

// move ring from old to new position
void Board::move_ring(pair<int, int> p1, pair<int, int> p2) {
    // make changes in rings_vector
    if (game_board.at(p1.first).at(p1.second).piece->color == player_color)
        replace(rings_vector.begin(), rings_vector.end(), p1, p2);
    else
        replace(opp_rings_vector.begin(), opp_rings_vector.end(), p1, p2);

    // place ring at p2
    game_board.at(p2.first).at(p2.second).piece = game_board.at(p1.first).at(p1.second).piece;
    //update ring position
    game_board.at(p2.first).at(p2.second).piece->xcord = p2.first;
    game_board.at(p2.first).at(p2.second).piece->ycord = p2.second;
    // place same color marker at p1 (replaces ring at p1)
    game_board.at(p1.first).at(p1.second).piece = new Piece('m', game_board.at(p1.first).at(p1.second).piece->color,
                                                            p1.first, p1.second);



    // flip all markers along way
    if (p1.first == p2.first) {
        for (int i = min(p1.second, p2.second) + 1; i < max(p1.second, p2.second); i++) {
            if (game_board.at(p1.first).at(i).is_marker()) {
                num_markers += game_board.at(p1.first).at(i).piece->color == player_color ? 1 : -1;
                num_opp_markers += game_board.at(p1.first).at(i).piece->color == player_color ? -1 : 1;
                game_board.at(p1.first).at(i).piece->flip_color();
            }
        }
    }
    else if (p1.second == p2.second) {
        for (int i = min(p1.first, p2.first) + 1; i < max(p1.first, p2.first); i++) {
            if (game_board.at(i).at(p1.second).is_marker()) {
                num_markers += game_board.at(i).at(p1.second).piece->color == player_color ? 1 : -1;
                num_opp_markers += game_board.at(i).at(p1.second).piece->color == player_color ? -1 : 1;
                game_board.at(i).at(p1.second).piece->flip_color();
            }
        }
    }
    else if (p1.first - p1.second == p2.first - p2.second) {
        // line x-y = k
        int k = p1.first - p1.second;
        for (int i = min(p1.second, p2.second) + 1; i < max(p1.second, p2.second); i++) {
            if (game_board.at(i + k).at(i).is_marker()) {
                num_markers += game_board.at(i+k).at(i).piece->color == player_color ? 1 : -1;
                num_opp_markers += game_board.at(i+k).at(i).piece->color == player_color ? -1 : 1;
                game_board.at(i + k).at(i).piece->flip_color();
            }
        }
    }
}

// delete row of markers (5 starting from start on line from start - p1 to end - p2)
void Board::delete_row(pair<int, int> p1, pair<int, int> p2) {
    if (p1.first == p2.first) {
        for (int i = min(p1.second, p2.second); i < min(p1.second, p2.second) + 5; i++) {
            if (game_board.at(p1.first).at(i).is_marker())
                remove_piece(make_pair(p1.first, i));
        }
    }
    else if (p1.second == p2.second) {
        for (int i = min(p1.first, p2.first); i < min(p1.first, p2.first) + 5; i++) {
            if (game_board.at(i).at(p1.second).is_marker())
                remove_piece(make_pair(i, p1.second));
        }
    }
    else if (p1.first - p1.second == p2.first - p2.second) {
        // line x-y = k
        int k = p1.first - p1.second;
        for (int i = min(p1.second, p2.second); i < min(p1.second, p2.second) + 5; i++) {
            if (game_board.at(i + k).at(i).is_marker())
                remove_piece(make_pair(i + k, i));
        }
    }
}

// check if move is valid
bool Board::is_move_valid(pair<int, int> p1, pair<int, int> p2) {
    // make sure p1 and p2 are not same
    if (p1 == p2)
        return false;

    // check if pos2 is empty
    if (game_board.at(p2.first).at(p2.second).is_piece())
        return false;

    //check if p1 is ring
    if (!game_board.at(p1.first).at(p1.second).is_ring())
        return false;

    // check if pos1 and pos2 are on a line
    if (!(p1.first == p2.first || p1.second == p2.second || p1.first - p1.second == p2.first - p2.second))
        return false;

    // iterate from pos1 to pos2, make sure there are no rings on the way
    // todo: fix this, it is not from min to max but from p1 to p2 IS IT EVEN NECESSARY
    if (p1.first == p2.first) {
        for (int i = min(p1.second, p2.second) + 1; i <= max(p1.second, p2.second); i++) {
            if (game_board.at(p1.first).at(i).is_ring())
                return false;
        }
    }
    else if (p1.second == p2.second) {
        for (int i = min(p1.first, p2.first) + 1; i <= max(p1.first, p2.first); i++) {
            if (game_board.at(i).at(p1.second).is_ring())
                return false;
        }
    }
    else {
        // line x-y = k
        int k = p1.first - p1.second;
        for (int i = min(p1.second, p2.second) + 1; i <= max(p1.second, p2.second); i++) {
            if (game_board.at(i + k).at(i).is_ring())
                return false;
        }
    }
    return true;
}


// convert cartesian to hex
pair<int, int> Board::xy_to_hex(pair<int, int> point) {
    int hex = 0;
    if (point.first * point.second >= 0)
        hex = max(abs(point.first), abs(point.second));
    else
        hex = abs(point.first) + abs(point.second);
    int hex_pos = 0;
    if (point.second >= point.first) {
        if (point.first >= 0)
            hex_pos = point.first;
        else {
            if (point.second > 0)
                hex_pos = 6 * point.second - 5 * point.first;
            else
                hex_pos = -5 * point.first + point.second;
        }
    }
    else {
        if (point.second >= 0)
            hex_pos = 2 * point.first - point.second;
        else {
            if (point.first >= 0)
                hex_pos = 2 * point.first - 3 * point.second;
            else
                hex_pos = -point.first - 3 * point.second;
        }
    }
    return make_pair(hex, hex_pos);

}

// convert hex to cartesian
pair<int, int> Board::hex_to_xy(pair<int, int> hex_point) {
    int h = hex_point.first;
    int hp = hex_point.second;

    if (hp <= h)
        return make_pair(hp, h);
    else if (hp <= 2 * h)
        return make_pair(h, 2 * h - hp);
    else if (hp <= 3 * h)
        return make_pair(3 * h - hp, 2 * h - hp);
    else if (hp <= 4 * h)
        return make_pair(3 * h - hp, -1 * h);
    else if (hp <= 5 * h)
        return make_pair(-1 * h, hp - 5 * h);
    else if (hp < 6 * h)
        return make_pair(hp - 6 * h, hp - 5 * h);
}

// scoring function (based on Neza Belej #2)
double Board::score_function(vector<pair<pair<int, int>, pair<int, int> > > vec) {
    vector<pair<pair<int, int>, pair<int, int> > >::iterator ptr;

    double result = 0;
    for (ptr = vec.begin(); ptr < vec.end(); ptr++) {
        pair<pair<int, int>, pair<int, int> > tuple = *ptr;
        pair<int, int> start = tuple.first;
        pair<int, int> end = tuple.second;
        if (start.first == end.first) // x coordinate same case
            result += pow(3.0, (fabs(end.second - start.second) + 1)) - 1;
        else if (start.second == end.second) // y coordinate same case
            result += pow(3.0, (fabs(end.first - start.first) + 1)) - 1;
        else // x - y = k
            result += pow(3.0, (fabs(end.first - start.first) + 1)) - 1;
        // Can think of combining else and else if
    }
    return 0.5 * result;
}

// Calculate score of player, subtract score of opponent
double Board::calculate_score() {
    vector<pair<pair<int, int>, pair<int, int> > > player_markers = get_marker_rows(1, player_color);
    vector<pair<pair<int, int>, pair<int, int> > > opp_markers = get_marker_rows(1, other_color);
    double score = score_function(player_markers) - score_function(opp_markers);
    vector<pair<pair<int, int>, pair<int, int> > > five_or_more = get_marker_rows(5, player_color);
    if (!five_or_more.empty())
        score = 100000;
    five_or_more = get_marker_rows(5, other_color);
    if (!five_or_more.empty())
        score = -100000;
    return score;
}

// Returns the successors for a particular initial position, along with score
// considers all possible valid yinsh successors
vector< pair<double, pair<pair<int, int>, pair<int, int> > > > Board::successors_score1(pair<int, int> initial_pos) {
    vector< pair< double, pair<pair<int, int>, pair<int, int> > > > successors;
    //todo: clean code
    // iterate y
    int i = 1;
    map<int, Point> base = game_board.find(initial_pos.first)->second;
    bool marker_flag = false;
    while (true) {
        if (base.find(initial_pos.second + i) == base.end())
            break;
        if (base.at(initial_pos.second + i).is_ring())
            break;
        if (base.at(initial_pos.second + i).is_marker())
            marker_flag = true;
        if (!base.at(initial_pos.second + i).is_piece()) {
            Board temp_board = Board(*this);
            pair<int, int> final_pos = make_pair(initial_pos.first, initial_pos.second + i);
            temp_board.move_ring(initial_pos, final_pos);
            successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
            if(marker_flag)
                break;
        }
        i++;
    }
    i = 1; marker_flag = false;
    while (true) {
        if (base.find(initial_pos.second - i) == base.end())
            break;
        if (base.at(initial_pos.second - i).is_ring())
            break;
        if (base.at(initial_pos.second - i).is_marker())
            marker_flag = true;
        if (!base.at(initial_pos.second - i).is_piece()) {
            Board temp_board = Board(*this);
            pair<int, int> final_pos = make_pair(initial_pos.first, initial_pos.second - i);
            temp_board.move_ring(initial_pos, final_pos);
            successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
            if(marker_flag)
                break;
        }
        i++;
    }

    // iterate x
    i = 1; marker_flag = false;
    while (true) {
        if (game_board.find(initial_pos.first + i) == game_board.end() ||
            game_board.find(initial_pos.first + i)->second.find(initial_pos.second) ==
            game_board.find(initial_pos.first + i)->second.end())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second).is_ring())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second).is_marker())
            marker_flag = true;
        if (!game_board.at(initial_pos.first + i).at(initial_pos.second).is_piece()) {
            Board temp_board = Board(*this);
            pair<int, int> final_pos = make_pair(initial_pos.first + i, initial_pos.second);
            temp_board.move_ring(initial_pos, final_pos);
            successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
            if(marker_flag)
                break;
        }
        i++;
    }
    i = 1; marker_flag = false;
    while (true) {
        if (game_board.find(initial_pos.first - i) == game_board.end() ||
            game_board.find(initial_pos.first - i)->second.find(initial_pos.second) ==
            game_board.find(initial_pos.first - i)->second.end())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second).is_ring())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second).is_marker())
            marker_flag = true;
        if (!game_board.at(initial_pos.first - i).at(initial_pos.second).is_piece()) {
            Board temp_board = Board(*this);
            pair<int, int> final_pos = make_pair(initial_pos.first - i, initial_pos.second);
            temp_board.move_ring(initial_pos, final_pos);
            successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
            if(marker_flag)
                break;
        }
        i++;
    }

    //iterate x=y
    i = 1; marker_flag = false;
    while (true) {
        if (game_board.find(initial_pos.first + i) == game_board.end() ||
            game_board.find(initial_pos.first + i)->second.find(initial_pos.second + i) ==
            game_board.find(initial_pos.first + i)->second.end())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second + i).is_ring())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second + i).is_marker())
            marker_flag = true;
        if (!game_board.at(initial_pos.first + i).at(initial_pos.second + i).is_piece()) {
            Board temp_board = Board(*this);
            pair<int, int> final_pos = make_pair(initial_pos.first + i, initial_pos.second + i);
            temp_board.move_ring(initial_pos, final_pos);
            successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
            if(marker_flag)
                break;
        }
        i++;
    }
    i = 1; marker_flag = false;
    while (true) {
        if (game_board.find(initial_pos.first - i) == game_board.end() ||
            game_board.find(initial_pos.first - i)->second.find(initial_pos.second - i) ==
            game_board.find(initial_pos.first - i)->second.end())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second - i).is_ring())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second - i).is_marker())
            marker_flag = true;
        if (!game_board.at(initial_pos.first - i).at(initial_pos.second - i).is_piece()) {
            Board temp_board = Board(*this);
            pair<int, int> final_pos = make_pair(initial_pos.first - i, initial_pos.second - i);
            temp_board.move_ring(initial_pos, final_pos);
            successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
            if(marker_flag)
                break;
        }
        i++;
    }

    return successors;
}

// considers next whitespace, first whitespace next to contiguous markers
// if <whitespace> <markers> <whitespace> - only returns nearest whitespace, and the one after markers
vector< pair<double, pair<pair<int, int>, pair<int, int> > > > Board::successors_score(pair<int, int> initial_pos) {
    vector< pair< double, pair<pair<int, int>, pair<int, int> > > > successors;
    //todo: clean code
    // iterate y
    bool seen_marker, seen_whitespace;
    int i = 1;
    map<int, Point> base = game_board.find(initial_pos.first)->second;
    seen_marker = false, seen_whitespace = false;
    while (true) {
        if (base.find(initial_pos.second + i) == base.end())
            break;
        if (base.at(initial_pos.second + i).is_ring())
            break;
        if (base.at(initial_pos.second + i).is_marker() & !seen_marker)
            seen_marker = true;
        if (!base.at(initial_pos.second + i).is_piece()) {
            if (seen_marker) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first, initial_pos.second + i);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                break;
            }
            if (!seen_whitespace) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first, initial_pos.second + i);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                seen_whitespace = true;
            }
        }
        i++;
    }
    i = 1;
    seen_marker = false, seen_whitespace = false;
    while (true) {
        if (base.find(initial_pos.second - i) == base.end())
            break;
        if (base.at(initial_pos.second - i).is_ring())
            break;
        if (base.at(initial_pos.second - i).is_marker() & !seen_marker)
            seen_marker = true;
        if (!base.at(initial_pos.second - i).is_piece()) {
            if (seen_marker) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first, initial_pos.second - i);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                break;
            }
            if (!seen_whitespace) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first, initial_pos.second - i);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                seen_whitespace = true;
            }
        }
        i++;
    }

    // iterate x
    i = 1;
    seen_marker = false, seen_whitespace = false;
    while (true) {
        if (game_board.find(initial_pos.first + i) == game_board.end() ||
            game_board.find(initial_pos.first + i)->second.find(initial_pos.second) ==
            game_board.find(initial_pos.first + i)->second.end())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second).is_ring())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second).is_marker() & !seen_marker)
            seen_marker = true;
        if (!game_board.at(initial_pos.first + i).at(initial_pos.second).is_piece()) {
            if (seen_marker) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first + i, initial_pos.second);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                break;
            }
            if (!seen_whitespace) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first + i, initial_pos.second);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                seen_whitespace = true;
            }
        }
        i++;
    }
    i = 1;
    seen_marker = false, seen_whitespace = false;
    while (true) {
        if (game_board.find(initial_pos.first - i) == game_board.end() ||
            game_board.find(initial_pos.first - i)->second.find(initial_pos.second) ==
            game_board.find(initial_pos.first - i)->second.end())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second).is_ring())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second).is_marker() & !seen_marker)
            seen_marker = true;
        if (!game_board.at(initial_pos.first - i).at(initial_pos.second).is_piece()) {
            if (seen_marker) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first - i, initial_pos.second);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                break;
            }
            if (!seen_whitespace) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first - i, initial_pos.second);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                seen_whitespace = true;
            }
        }
        i++;
    }

    //iterate x=y
    i = 1;
    seen_marker = false, seen_whitespace = false;
    while (true) {
        if (game_board.find(initial_pos.first + i) == game_board.end() ||
            game_board.find(initial_pos.first + i)->second.find(initial_pos.second + i) ==
            game_board.find(initial_pos.first + i)->second.end())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second + i).is_ring())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second + i).is_marker() & !seen_marker)
            seen_marker = true;
        if (!game_board.at(initial_pos.first + i).at(initial_pos.second + i).is_piece()) {
            if (seen_marker) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first + i, initial_pos.second + i);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                break;
            }
            if (!seen_whitespace) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first + i, initial_pos.second + i);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                seen_whitespace = true;
            }
        }
        i++;
    }
    i = 1;
    seen_marker = false, seen_whitespace = false;
    while (true) {
        if (game_board.find(initial_pos.first - i) == game_board.end() ||
            game_board.find(initial_pos.first - i)->second.find(initial_pos.second - i) ==
            game_board.find(initial_pos.first - i)->second.end())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second - i).is_ring())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second - i).is_marker() & !seen_marker)
            seen_marker = true;
        if (!game_board.at(initial_pos.first - i).at(initial_pos.second - i).is_piece()) {
            if (seen_marker) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first - i, initial_pos.second - i);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                break;
            }
            if (!seen_whitespace) {
                Board temp_board = Board(*this);
                pair<int, int> final_pos = make_pair(initial_pos.first - i, initial_pos.second - i);
                temp_board.move_ring(initial_pos, final_pos);
                successors.emplace_back(make_pair(temp_board.calculate_score(), make_pair(initial_pos, final_pos)));
                seen_whitespace = true;
            }
        }
        i++;
    }
    return successors;
}


// Returns the successors for a particular initial position - DEPRECATED
vector<pair<pair<int, int>, pair<int, int> > > Board::successors(pair<int, int> initial_pos) {
    vector<pair<pair<int, int>, pair<int, int> > > successors;
    //todo: clean code
    // iterate y
    int i = 1;
    map<int, Point> base = game_board.find(initial_pos.first)->second;
    while (true) {
        if (base.find(initial_pos.second + i) == base.end())
            break;
        if (base.at(initial_pos.second + i).is_ring())
            break;
        if (!base.at(initial_pos.second + i).is_piece()) {
            successors.emplace_back(make_pair(initial_pos, make_pair(initial_pos.first, initial_pos.second + i)));
            break;
        }
        i++;
    }
    i = 1;
    while (true) {
        if (base.find(initial_pos.second - i) == base.end())
            break;
        if (base.at(initial_pos.second - i).is_ring())
            break;
        if (!base.at(initial_pos.second - i).is_piece()) {
            successors.emplace_back(make_pair(initial_pos, make_pair(initial_pos.first, initial_pos.second - i)));
            break;
        }
        i++;
    }

    // iterate x
    i = 1;
    while (true) {
        if (game_board.find(initial_pos.first + i) == game_board.end() ||
            game_board.find(initial_pos.first + i)->second.find(initial_pos.second) ==
            game_board.find(initial_pos.first + i)->second.end())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second).is_ring())
            break;
        if (!game_board.at(initial_pos.first + i).at(initial_pos.second).is_piece()) {
            successors.emplace_back(make_pair(initial_pos, make_pair(initial_pos.first + i, initial_pos.second)));
            break;
        }
        i++;
    }
    i = 1;
    while (true) {
        if (game_board.find(initial_pos.first - i) == game_board.end() ||
            game_board.find(initial_pos.first - i)->second.find(initial_pos.second) ==
            game_board.find(initial_pos.first - i)->second.end())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second).is_ring())
            break;
        if (!game_board.at(initial_pos.first - i).at(initial_pos.second).is_piece()) {
            successors.emplace_back(make_pair(initial_pos, make_pair(initial_pos.first - i, initial_pos.second)));
            break;
        }
        i++;
    }

    //iterate x=y
    i = 1;
    while (true) {
        if (game_board.find(initial_pos.first + i) == game_board.end() ||
            game_board.find(initial_pos.first + i)->second.find(initial_pos.second + i) ==
            game_board.find(initial_pos.first + i)->second.end())
            break;
        if (game_board.at(initial_pos.first + i).at(initial_pos.second + i).is_ring())
            break;
        if (!game_board.at(initial_pos.first + i).at(initial_pos.second + i).is_piece()) {
            successors.emplace_back(make_pair(initial_pos, make_pair(initial_pos.first + i, initial_pos.second + i)));
            break;
        }
        i++;
    }
    i = 1;
    while (true) {
        if (game_board.find(initial_pos.first - i) == game_board.end() ||
            game_board.find(initial_pos.first - i)->second.find(initial_pos.second - i) ==
            game_board.find(initial_pos.first - i)->second.end())
            break;
        if (game_board.at(initial_pos.first - i).at(initial_pos.second - i).is_ring())
            break;
        if (!game_board.at(initial_pos.first - i).at(initial_pos.second - i).is_piece()) {
            successors.emplace_back(make_pair(initial_pos, make_pair(initial_pos.first - i, initial_pos.second - i)));
            break;
        }
        i++;
    }
    return successors;
}

vector<string> split(const std::string &str, char delim = ' ') {
    vector<string> splitString;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim))
        splitString.push_back(token);
    return splitString;
}

void Board::execute_move(string move, int playerID) {
    //todo: clean code
    vector<string> splitString = split(move, ' ');
    num_moves_played++;
    if (splitString.size() == 3) { // Place a ring
        pair<int, int> pos_hex = make_pair(stoi(splitString.at(1)),
                                           stoi(splitString.at(2))); //position in hex coordinates
        pair<int, int> pos_xy = hex_to_xy(pos_hex);
        place_piece('r', playerID == 1 ? player_color : other_color, pos_xy);
    }
    else if (splitString.size() == 6) { // Move a ring
        pair<int, int> start_hex = make_pair(stoi(splitString.at(1)),
                                             stoi(splitString.at(2))); //start position in hex coordinates
        pair<int, int> end_hex = make_pair(stoi(splitString.at(4)),
                                           stoi(splitString.at(5))); //end postion in hex coordinates
        pair<int, int> start_xy = hex_to_xy(start_hex);
        pair<int, int> end_xy = hex_to_xy(end_hex);
        std::cerr << start_xy.first << ", " << start_xy.second << " to ";
        std::cerr << end_xy.first << ", " << end_xy.second << '\n';
        move_ring(start_xy, end_xy);
    }
    else { // Move Ring, Remove Row, Remove Ring
        // Move piece
        pair<int, int> start_hex, end_hex, row_start_hex, row_end_hex, ring_hex;
        for (int i = 0; i < 15; i+=3)
        {
            if (splitString.at(i) == "S")
                start_hex = make_pair(stoi(splitString.at(i+1)), stoi(splitString.at(i+2)));
            else if (splitString.at(i) == "M")
                end_hex = make_pair(stoi(splitString.at(i+1)), stoi(splitString.at(i+2)));
            else if (splitString.at(i) == "RS")
                row_start_hex = make_pair(stoi(splitString.at(i+1)), stoi(splitString.at(i+2)));
            else if (splitString.at(i) == "RE")
                row_end_hex = make_pair(stoi(splitString.at(i+1)), stoi(splitString.at(i+2)));
            else
                ring_hex = make_pair(stoi(splitString.at(i+1)), stoi(splitString.at(i+2)));
        }
        move_ring(hex_to_xy(start_hex), hex_to_xy(end_hex));
        delete_row(hex_to_xy(row_start_hex), hex_to_xy(row_end_hex));
        remove_piece(hex_to_xy(ring_hex));
    }
}

pair<int, int> Board::get_bounds(int i) {
    if (i == 0)
        return pair<int, int>(-(n - 1), n - 1);
    else if (i > 0)
        return pair<int, int>(-(n - i), n);
    else
        return pair<int, int>(-n, (n + i));
}


vector<pair<pair<int, int>, pair<int, int> > > Board::get_marker_rows(int length, char color) {
    //todo: clean code
    vector<pair<pair<int, int>, pair<int, int> > > result;
    // check all x=k lines
    for (int x = -(n - 1); x <= (n - 1); x++) {
        pair<int, int> bounds = get_bounds(x);
        for (int y = bounds.first; y <= bounds.second;) {
            if (game_board.at(x).at(y).is_marker() && game_board.at(x).at(y).piece->color == color) {
                int i = 1;
                while (true) {
                    if (y + i > bounds.second || !game_board.at(x).at(y + i).is_marker() ||
                        game_board.at(x).at(y + i).piece->color != color)
                        break;
                    i++;
                }
                if (i > length - 1)
                    result.emplace_back(make_pair(make_pair(x, y), make_pair(x, y + i - 1)));
                y += i;
            }
            else
                y++;
        }
    }

    // check all y=k lines
    for (int y = -(n - 1); y <= (n - 1); y++) {
        pair<int, int> bounds = get_bounds(y);
        for (int x = bounds.first; x <= bounds.second;) {
            if (game_board.at(x).at(y).is_marker() && game_board.at(x).at(y).piece->color == color) {
                int i = 1;
                while (true) {
                    if (x + i > bounds.second || !game_board.at(x + i).at(y).is_marker() ||
                        game_board.at(x + i).at(y).piece->color != color)
                        break;
                    i++;
                }
                if (i > length - 1)
                    result.emplace_back(make_pair(make_pair(x, y), make_pair(x + i - 1, y)));
                x += i;
            }
            else
                x++;
        }
    }

    // check all x-y = k lines
    for (int k = -(n - 1); k <= (n - 1); k++) {
        pair<int, int> bounds = get_bounds(k);
        for (int x = bounds.first; x <= bounds.second;) {
            if (game_board.at(x).at(x - k).is_marker() && game_board.at(x).at(x - k).piece->color == color) {
                int i = 1;
                while (true) {
                    if (x + i > bounds.second || !game_board.at(x + i).at(x - k + i).is_marker() ||
                        game_board.at(x + i).at(x - k + i).piece->color != color)
                        break;
                    i++;
                }
                if (i > length - 1)
                    result.emplace_back(make_pair(make_pair(x, x - k), make_pair(x + i - 1, x - k + i - 1)));
                x += i;
            }
            else
                x++;
        }
    }
    return result;
}


/*
 * functions to make:
 *  move_ring(old_pos, new_pos)
 *  delete_row
 */
