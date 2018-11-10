//
// Created by karthik on 13/9/18.
//

#ifndef YINSH_AI_AGENT_H
#define YINSH_AI_AGENT_H

#include <cmath>
#include <cfloat>
#include "Board.h"

#define EXPLORATION_PARAMETER 2

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

class NodeMCTS {
public:
    pair< pair<int, int>, pair<int, int> > move;
    double totalValue;
    int visits;

    Board *configuration;
    NodeMCTS* parent;
    vector<NodeMCTS*> children;
    bool isLeaf;
    int depth;
    char type; //M - max, m - min
    int gotoidx; //The index of child to go to
    NodeMCTS(NodeMCTS* parent) : totalValue(0), gotoidx(0), isLeaf(true), visits(0), parent(parent), type('u'), depth(0) {}
    NodeMCTS(const NodeMCTS& nodeToCopy) : totalValue(nodeToCopy.totalValue), gotoidx(nodeToCopy.gotoidx), isLeaf(nodeToCopy.isLeaf), visits(nodeToCopy.visits), parent(nodeToCopy.parent), type(nodeToCopy.type), depth(nodeToCopy.depth) {}

    double ucb(){return visits == 0 ? DBL_MAX : totalValue/visits + EXPLORATION_PARAMETER*sqrt(log(parent->visits) / visits);}
};

class Agent {
private:
    int playerID; // 1 - self, 2 - other
    double time_left;
    int n;
public:
    vector<double> feature_weights;
    double epsilon_exploration;
    double discount_factor;
    double learning_rate;

    Board state;
    Board state_tree;

    double q_value(Board& s, Board& s_prime);
    double reward_function(Board& s, Board& s_prime);
    void initialize_weights();
    pair<double, pair<pair<int,int>,pair<int,int>>> get_best_Qmove(Board& state);
    void update_weights(double difference, Board& state, Board& successor);
    bool Qiteration(Board& state);
    void save_weights(string filename);
    void load_weights(string filename);
    void print_weights();

    Agent(int playerID, int board_size, double time_left, char player_col, char other_col) :
            playerID(playerID), n(board_size), time_left(time_left), state(board_size, 5, 5, 3, player_col, other_col),
            state_tree(board_size, 5, 5, 3, player_col, other_col), epsilon_exploration(0.3), discount_factor(0.9), learning_rate(0.1) {initialize_weights();}
    void execute_move(string move, int playerID);
    void recursive_construct_tree(Board board, Node* node, int depth, int maxDepth);
    string get_next_move();
    double minimax(Node* node);
    int check_end(Board state);
    string initial_move();
    double minimax_ab(Board board, Node *node, int depth, double min, double max, int maxDepth);
    double minimax_ab_sort(Board board, Node *node, int depth, double alpha, double beta, int maxDepth);

    pair<pair<int,int>, pair<int,int>> monte_carlo(int interations);
    NodeMCTS* select(NodeMCTS* root);
    void expand(NodeMCTS* node);
    int simulate(NodeMCTS* node);
    void backpropagate(NodeMCTS* node, int score);
};


#endif //YINSH_AI_AGENT_H
