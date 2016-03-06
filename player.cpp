#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    board = new Board();
    my_side = side;
    if (my_side == BLACK){
        opp_side = WHITE;
    }
    else{
        opp_side = BLACK;
    }
}

/*
 * Destructor for the player.
 */
Player::~Player() {

}

/* Compute the valid moves for player on the entire board
   Returns a vector of pointers to valid Moves */
std::vector<Move*> Player::findValid(Side side){
    std::vector<Move*> valid_moves;
    Move *curr_move;
    if (board->hasMoves(side)){
        // go through the entire board to check if each 
        // position is a potentially valid move
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                curr_move = new Move(i, j);
                if(board->checkMove(curr_move, side)){
                    valid_moves.push_back(curr_move);
                }
            }
        }
    }
    return valid_moves; 
}



/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 

    if(testingMinimax){
        std::cerr << "Minimax not implemented yet" << std::endl;
        return NULL;
    }
    else{
        Move *myMove = NULL;

        // Process opponent's move (doMove handles NULL moves)
        board->doMove(opponentsMove, opp_side);

        // Calculate my valid moves
        std::vector <Move*> valid_moves = findValid(my_side);

        if(!valid_moves.empty()){ // Randomly choose a valid move to play
            int index = rand() % valid_moves.size();
            myMove = valid_moves[index];
        }
        // Process my own move
        board->doMove(myMove, my_side);
        fprintf(stderr,"Using heuristic, Iago (%s) chose Move: (%d, %d)\n",
            (my_side == BLACK) ? "Black": "White", 
            myMove->getX(), myMove->getY());
        return myMove;
    } 
}
