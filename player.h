#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <stdio.h>
#include <ctime>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    std::vector<Move*> findValid(Side side);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

    // flag to indicate whether playing with heuristic or randomly
    bool dumbPlaying; 

    // declare member variables
    Board *board;
    Side my_side;
    Side opp_side;
};

#endif
