#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include "Board.h"
#include "Agent.h"

using namespace std;

// Sample C++ Code
int main(int argc, char** argv) {

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
       cerr << "Move " << move << endl;
       a.execute_move(move, 2);

       cerr << "1 " << move << "\n";
       while(true) {
//            cerr << "Reached here" << endl;
//           move = a.get_next_move();
            if(a.state.num_moves_played < 10) {
            // if(true) {
                move = a.get_next_move();
                cout << move << endl;
            }
            else {
                pair<pair<int,int>,pair<int,int>> monte_carlo_move = a.monte_carlo(100);
    //            a.execute_move(move, 1);
                cout << a.state.execute_move_and_return_server_string(monte_carlo_move) << endl;
            }
           move.clear();
           while(move == "") {
               getline(cin, move);
           }
           a.execute_move(move, 2);
//            cerr << "2 " << move << "\n";
       }
   }
   else if(player_id == 1) {
       while(true) {
           if(a.state.num_moves_played < 10) {
           // if(true) {
               move = a.get_next_move();
               cout << move << endl;
           }
           else {
//           move = a.get_next_move();
               pair<pair<int,int>,pair<int,int>> monte_carlo_move = a.monte_carlo(100);
               std::cout << monte_carlo_move.first.first << ", " << monte_carlo_move.first.second << " to " <<  monte_carlo_move.second.first << ", " << monte_carlo_move.second.second << '\n';
               cout << a.state.execute_move_and_return_server_string(monte_carlo_move) << endl;
           }
//            a.execute_move(move, 1);
           move.clear();
           while(move == "") {
               getline(cin, move);
           }
           a.execute_move(move, 2);
//            cerr << "3 " << move << "\n";
       }
   }
   return 0;
}

// int main(int argc, char ** argv){
//
//     Agent a(1, 5, 120, 'b', 'o');
//
//     a.state.execute_move("P 2 2", 2);
//     a.state.execute_move("P 0 0", 1);
//     a.state.execute_move("P 5 21", 2);
//     a.state.execute_move("P 1 0", 1);
//     a.state.execute_move("P 4 11", 2);
//     a.state.execute_move("P 1 1", 1);
//     a.state.execute_move("P 5 2", 2);
//     a.state.execute_move("P 1 2", 1);
//     a.state.execute_move("P 2 0", 2);
//     a.state.execute_move("P 1 3", 1);
//     a.state.execute_move("S 5 2 M 4 2", 2);
//     a.state.execute_move("S 0 0 M 1 5", 1);
//     a.state.execute_move("S 4 11 M 4 9", 2);
//     a.state.execute_move("S 1 5 M 2 11", 1);
//     a.state.execute_move("S 4 9 M 5 11", 2);
//     a.state.execute_move("S 2 11 M 3 17", 1);
//     a.state.execute_move("S 2 2 M 2 4", 2);
//     a.state.execute_move("S 3 17 M 4 23", 1);
//     a.state.execute_move("S 5 11 M 3 7", 2);
//     a.state.execute_move("S 4 23 M 5 29", 1);
//     a.state.execute_move("S 3 7 M 4 7", 2);
//
//     a.state.execute_move("S 5 29 M 1 4", 1);
//     a.state.execute_move("S 2 4 M 3 5", 2);
//     a.state.execute_move("S 1 4 M 2 7", 1);
//     a.state.execute_move("S 5 21 M 5 23", 2);
//     a.state.execute_move("S 2 7 M 3 10", 1);
//     a.state.execute_move("S 5 23 M 2 8", 2);
//     a.state.execute_move("S 3 10 M 4 13", 1);
//     a.state.execute_move("S 3 5 M 5 3", 2);
//     a.state.execute_move("S 4 13 M 5 16", 1);
//     a.state.execute_move("S 2 8 M 2 10", 2);
//     a.state.execute_move("S 5 16 M 5 17 RE 5 16 RS 1 4 X 1 3", 1);
//
//
//     string move = a.get_next_move();
//     return 0;
// }
