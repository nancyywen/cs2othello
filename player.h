#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <stdio.h>
#include <ctime>
#include <limits>
#include "common.h"
#include "board.h"

#define MSEC (1000)
using namespace std;

// used reference for the state weights heuristic fuunction
// https://courses.cs.washington.edu/courses/cse573/04au/Project/mini1/RUSSIA/Final_Paper.pdf
const int heuristic[8][8] = { {4, -3, 2, 2, 2, 2, -3, 4},
                    {-3, -4, -1, -1, -1, -1, -4, -3},
                    {2, -1, 1, 0, 0, 1, -1, 2}, 
                    {2, -1, 0, 1, 1, 0, -1, 2},
                    {2, -1, 0, 1, 1, 0, -1, 2},
                    {2, -1, 1, 0, 0, 1, -1, 2}, 
                    {-3, -4, -1, -1, -1, -1, -4, -3},
                    {4, -3, 2, 2, 2, 2, -3, 4}}; 

class Player {

public:
    Player(Side side);
    ~Player();
    
    std::vector<Move*> findValid(Side side, Board *b);
    bool isCorner(Move *m);
    int heuristicScore(Move *move, Side side);
    int boardScore(Side side, Board *b);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

    // flag to indicate if using minimax
    bool minimaxPlaying;

    // flag to indicate whether playing with heuristic or randomly
    bool dumbPlaying; 

    // declare member variables
    Board *board;
    Side my_side;
    Side opp_side;

    
};

#endif
