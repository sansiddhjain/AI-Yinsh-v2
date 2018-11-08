//
// Created by karthik on 12/9/18.
//

#ifndef YINSH_AI_BOARD_H
#define YINSH_AI_BOARD_H

using namespace std;

#include <utility>
#include <vector>
#include <map>

class Piece{
private:
public:
    char color; // 'o' - orange, 'b' - blue
    char type; // 'r' - ring, 'm' - marker
    int xcord;
    int ycord;

    Piece(char t, char c, int xcord, int ycord) : type(t), color(c), xcord(xcord), ycord(ycord) {}
    void flip_color() { color = color == 'o' ? 'b' : 'o';}

//    Piece(const Piece& p) : color(p.color), type(p.type), xcord(p.xcord), ycord(p.ycord) {}
//    ~Piece() = default;
//    Piece& operator=(const Piece& p) {if(this != &p) {color = p.color; type = p.type; xcord = p.xcord; ycord = p.ycord;} return *this;}
};

class Point{
private:
public:
    int ycord;
    int xcord;
    Piece *piece; // pointer to ring or marker, null if nothing present

    Point(int xcord, int ycord) : xcord(xcord), ycord(ycord), piece(nullptr) {};
    bool is_piece() { return piece != nullptr;}
    bool is_ring() {return  piece != nullptr && piece->type == 'r';}
    bool is_marker() {return piece != nullptr && piece->type == 'm';}

    Point(const Point &p);
    ~Point() {delete piece;}
    Point& operator=(const Point& p);
};

class Board{
private:
    int n; // size of board
    int m; // number of rings
    int l; // rings needed to remove to win
    int k; // markers in a line to remove ring
    pair<int,int> get_bounds(int i);
public:
    char player_color;
    char other_color;
    map<int, map<int, Point> > game_board;
    int num_rings_on_board;
    int num_opp_rings_on_board;
    vector< pair<int, int> > rings_vector;
    vector< pair<int, int> > opp_rings_vector;
    int num_markers;
    int num_opp_markers;
    int num_moves_played;
//    Piece** rings; // (!MIGHT HAVE TO DEPRECATE THIS!) array containing pointers to rings on board. IMPORTANT: check for null before accessing, after deleting a ring this maybe dangling pointer
//    Piece** opp_rings;

    Board(int n, int m, int k, int l, char player_col, char other_col);
    Board(const Board& b);

    int return_n(){ return n; }
    int return_m(){ return m; }
    int return_k(){ return k; }
    int return_l(){ return l; }
    bool is_position_valid(pair<int,int> position);
    bool is_move_valid(pair<int,int> p1, pair<int, int> p2);
    bool place_piece(char type, char color, pair<int,int> position);
    bool remove_piece(pair<int,int> position);
    bool move_ring(pair<int,int> p1, pair<int, int> p2);
    bool delete_row(pair<int,int> p1, pair<int,int> p2);
    double score_function(vector<pair<pair<int, int>, pair<int, int> > > vec);
    double calculate_score();
    vector< pair < double, pair<pair<int, int>, pair<int, int> > > > successors_score(pair<int,int> initial_pos);
    vector< pair< pair<int,int>, pair<int,int> > > successors(pair<int,int> initial_pos);
    pair<int,int> xy_to_hex(pair<int,int> point);
    pair<int,int> hex_to_xy(pair<int,int> hex_point);
    void execute_move(string move, int playerID);
    vector< pair< pair<int,int>, pair<int,int> > > get_marker_rows(int length, char color); //returns rows of length >= given
    vector< pair<double, pair<pair<int, int>, pair<int, int> > > > successors_score1(pair<int, int> initial_pos);
};



#endif //YINSH_AI_BOARD_H
