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

    a.state.execute_move("P 1 4", 2);


//    vector<pair<pair<int, int>, pair<int, int> > > b = a.state.get_marker_rows(1, 'o');
//    double c = a.state.score_function(b);


//    Board b(a.state);
//    b.execute_move("S 0 0 M 3 13", 2);

    string move = a.get_next_move();
    return 0;
}
// S 3 1 M 4 2 RS 1 4 RE 4 23 X 5 4