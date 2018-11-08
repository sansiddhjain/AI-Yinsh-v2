//
// Created by karthik on 17/9/18.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include "Board.h"
#include "Agent.h"

using namespace std;

int main(int argc, char ** argv){

    Agent a(1, 5, 120, 'o', 'b');

    a.state.execute_move("P 0 0", 2);
    a.state.execute_move("P 1 0", 1);
    a.state.execute_move("P 1 1", 2);
    a.state.execute_move("P 1 2", 1);
    a.state.execute_move("P 1 3", 2);
    a.state.execute_move("P 1 4", 1);
    a.state.execute_move("P 1 5", 2);
    a.state.execute_move("P 2 0", 1);
    a.state.execute_move("P 2 1", 2);
    a.state.execute_move("P 2 2", 1);
    a.state.execute_move("S 1 1 M 2 3", 2);
    a.state.execute_move("S 2 0 M 3 0", 1);
    a.state.execute_move("S 2 3 M 3 5", 2);
    a.state.execute_move("S 2 2 M 2 4", 1);
    a.state.execute_move("S 3 5 M 4 6", 2);
    a.state.execute_move("S 2 4 M 3 6", 1);
    a.state.execute_move("S 4 6 M 2 5", 2);
    a.state.execute_move("S 3 0 M 4 0", 1);
    a.state.execute_move("S 2 5 M 3 8", 2);
    a.state.execute_move("S 4 0 M 4 1", 1);
    a.state.execute_move("S 2 1 M 3 17", 2);
    a.state.execute_move("S 1 0 M 3 2", 1);
    a.state.execute_move("S 3 17 M 4 23", 2);
    a.state.execute_move("S 3 2 M 4 3", 1);
    a.state.execute_move("S 4 23 M 3 1", 2);
    a.state.execute_move("S 4 1 M 3 16", 1);
    a.state.execute_move("S 3 1 M 3 3", 2);
    a.state.execute_move("S 3 16 M 2 11", 1);
    a.state.execute_move("S 3 3 M 5 28", 2);
    a.state.execute_move("S 3 6 M 4 7", 1);
    a.state.execute_move("S 5 28 M 2 10", 2);
    a.state.execute_move("S 4 7 M 4 5", 1);
    a.state.execute_move("S 3 8 M 5 8", 2);
    a.state.execute_move("S 4 5 M 4 8", 1);
    a.state.execute_move("S 5 8 M 3 7", 2);
    a.state.execute_move("S 4 8 M 4 4", 1);
    a.state.execute_move("S 3 7 M 4 2", 2);
    a.state.execute_move("S 1 2 M 3 4", 1);
    a.state.execute_move("S 4 2 M 5 29", 2);
    a.state.execute_move("S 3 4 M 5 6", 1);
    a.state.execute_move("S 5 29 M 5 27", 2);
    a.state.execute_move("S 5 6 M 5 7 RS 1 2 RE 5 6 X 4 4", 1);
    a.state.execute_move("S 5 27 M 3 4", 2);
    a.state.execute_move("S 5 7 M 5 9", 1);
    a.state.execute_move("S 3 4 M 4 9", 2);
    a.state.execute_move("S 5 9 M 2 3", 1);
    a.state.execute_move("S 4 9 M 5 12", 2);
    a.state.execute_move("S 2 3 M 5 2", 1);
    a.state.execute_move("S 2 10 M 3 15", 2);
    a.state.execute_move("S 2 11 M 4 21", 1);
    a.state.execute_move("S 5 12 M 4 10", 2);
    a.state.execute_move("S 5 2 M 5 1", 1);
    a.state.execute_move("S 4 10 M 5 13", 2);
    a.state.execute_move("S 5 1 M 1 2 RS 3 16 RE 2 3 X 4 21", 1);
    a.state.execute_move("S 5 13 M 4 11", 2);
    a.state.execute_move("S 4 3 M 4 4", 1);
    a.state.execute_move("S 1 5 M 5 4", 2);
    a.state.execute_move("S 4 4 M 1 1", 1);
    a.state.execute_move("S 5 4 M 4 5", 2);
    a.state.execute_move("S 1 1 M 1 0", 1);
    a.state.execute_move("S 4 11 M 5 14", 2);
    a.state.execute_move("S 1 0 M 2 9", 1);
    a.state.execute_move("S 5 14 M 4 12", 2);
    a.state.execute_move("S 2 9 M 3 13", 1);
    a.state.execute_move("S 4 12 M 3 9", 2);
    a.state.execute_move("S 3 13 M 4 17", 1);
    a.state.execute_move("S 4 5 M 5 11 RS 4 8 RE 4 12 X 0 0", 2);


//    vector<pair<pair<int, int>, pair<int, int> > > b = a.state.get_marker_rows(1, 'o');
//    double c = a.state.score_function(b);


//    Board b(a.state);
//    b.execute_move("S 0 0 M 3 13", 2);

    string move = a.get_next_move();
    return 0;
}
// S 3 1 M 4 2 RS 1 4 RE 4 23 X 5 4