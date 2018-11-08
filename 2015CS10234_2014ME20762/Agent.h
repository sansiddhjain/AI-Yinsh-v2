//
// Created by karthik on 13/9/18.
//

#ifndef YINSH_AI_AGENT_H
#define YINSH_AI_AGENT_H

#include <cmath>
#include "Board.h"

class Node {
public:
  pair< pair<int, int>, pair<int, int> > move;
  double score;
  Node** children;
  bool isLeaf;
  double alpha;
  double beta;
  char type; //M - max, m - min
  int gotoidx; //The index of child to go to
  Node() : score(0), gotoidx(0), isLeaf(false), alpha(-INFINITY), beta(INFINITY), type('u') {}
};

class Agent {
private:
    int playerID; // 1 - self, 2 - other
    double time_left;
    int n;
public:
    Board state;
    Board state_tree;

    Agent(int playerID, int board_size, double time_left, char player_col, char other_col) : playerID(playerID), n(board_size), time_left(time_left), state(board_size, 5, 5, 3, player_col, other_col), state_tree(board_size, 5, 5, 3, player_col, other_col) {}
    void execute_move(string move, int playerID);
//    void copy_board();
//    double score_function(vector< pair< pair<int,int>, pair<int,int> > > vec);
//    double calculate_score(Board& board);
    void recursive_construct_tree(Board board, Node* node, int depth, int maxDepth);
    string get_next_move();
    double minimax(Node* node);
    bool check_won();
    string initial_move();
    double minimax_ab(Board board, Node *node, int depth, double min, double max, int maxDepth);
    double minimax_ab_sort(Board board, Node *node, int depth, double alpha, double beta, int maxDepth);
};


#endif //YINSH_AI_AGENT_H
