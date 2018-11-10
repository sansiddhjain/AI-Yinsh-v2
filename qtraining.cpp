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

//    Agent trainee(1, 5, 120, 'o', 'b');
//
//    Agent trainer(1, 5, 120, 'o', 'b');
//
//    int iterations = 100;
//
//    for(int i = 0; i < iterations; i++){
//        if(trainee.state.num_moves_played < 10) {
//            string move = trainee.get_next_move();
//            cout << "Trainee played " << move << endl;
//        }
//
//    }


    // let's watch the training in action against opponent

    int player_id, board_size, time_limit, seq_length;
    string move;
    // Get input from server about game specifications
    cin >> player_id >> board_size >> time_limit >> seq_length;
    cerr << player_id << " " << board_size << " " << time_limit << " " << seq_length << endl;
    Agent a(player_id, board_size, time_limit, 'o', 'b');

    if(player_id == 2) {
        // Get other player's move
        while(move == "") {
            getline(cin, move);
        }
        a.execute_move(move, 2);

        while(true) {
            if(a.state.num_moves_played < 10) {
                move = a.get_next_move();
                cout << move << endl;
            }
            else {
                // execute one q iteration to train
                bool ended = a.Qiteration(a.state);
                a.print_weights();
                if(ended) {
                    a.save_weights("weights.txt");
                }
                pair<pair<int,int>, pair<int,int>> best_qmove = a.get_best_Qmove(a.state).second;
                cout << a.state.execute_move_and_return_server_string(best_qmove) << endl;
            }
            move.clear();
            while(move == "") {
                getline(cin, move);
            }
            a.execute_move(move, 2);
            cerr << "Opp: " << move << endl;
//            a.save_weights("weights.txt");
        }
    }
    else if(player_id == 1) {
        while(true) {
            if(a.state.num_moves_played < 10) {
                move = a.get_next_move();
                cout << move << endl;
            }
            else {
                bool ended = a.Qiteration(a.state);
                a.print_weights();
                if(ended) {
                    a.save_weights("weights.txt");
                }
                pair<pair<int,int>, pair<int,int>> best_qmove = a.get_best_Qmove(a.state).second;
                cout << a.state.execute_move_and_return_server_string(best_qmove) << endl;
            }
            move.clear();
            while(move == "") {
                getline(cin, move);
            }
            a.execute_move(move, 2);
            cerr << "Opp: " << move << endl;
//            a.save_weights("weights.txt");
        }
    }
    return 0;
}