#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    minimaxPlaying = true; // true if using minimax
    dumbPlaying = false; // true if playing randomly, false if using heuristic

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

/* Compute the valid moves for player on a given board
   Returns a vector of pointers to valid Moves */
std::vector<Move*> Player::findValid(Side side, Board *b){
    std::vector<Move*> valid_moves;
    Move *curr_move;
    if (b->hasMoves(side)){
        // go through the entire board to check if each 
        // position is a potentially valid move
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                curr_move = new Move(i, j);
                if(b->checkMove(curr_move, side)){
                    valid_moves.push_back(curr_move);
                }
            }
        }
    }
    return valid_moves; 
}
/*
 * Returns true if the move is in corner
 */
bool Player::isCorner(Move *m){
    if ((m->getX() == 0 && m->getY() == 0) ||
        (m->getX() == 0 && m->getY() == 7) ||
        (m->getX() == 7 && m->getY() == 0) ||
        (m->getX() == 7 && m->getY() == 7)) {
        return true;
    }
    return false;
}


/* Compute heuristic score for a given move by finding
 * the difference in the weighted sum between the two players
 */
int Player::heuristicScore(Move *move, Side side){
    int my_score = 0;
    int opp_score = 0;
    Board *b = board->copy();
    b->doMove(move, side);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(b->occupied(i, j)){
                if(b->get(side, i, j)){
                    my_score += heuristic[i][j];
                }
                else{
                    opp_score += heuristic[i][j];
                }
            }
        }
    }
    return my_score - opp_score;
}

/*
 * Compute score for a given board which is  the 
 * difference in the heuristic score 
 */
int Player::boardScore(Side side, Board *b){
    int my_score = 0;
    int opp_score = 0;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(b->occupied(i, j)){
                if(b->get(side, i, j)){
                    my_score += heuristic[i][j];
                }
                else{
                    opp_score += heuristic[i][j];
                }
            }
        }
    }
    return my_score - opp_score;
}


/* Given the current state of the board, the depth being searched, and
   the maximum depth to be search, minimax returns the chosen move */
std::pair<int, Move*> Player::minimax(Board *b, int depth, int max_depth, bool maximizingPlayer){
    std::pair<int, Move*> best_choice;
    int best_score;
    Move *best_move = NULL;
    if (depth == max_depth){
        int score = boardScore(my_side, b);
        best_choice = std::make_pair(score, best_move);
        return best_choice;
    }
    else{
        std::vector<Move*> valid_moves;
        if(maximizingPlayer){
            valid_moves = findValid(my_side, b);
        }
        else{
            valid_moves = findValid(opp_side, b);
        }
        if(valid_moves.empty()){
            int score = boardScore(my_side, b);
            best_choice = std::make_pair(score, best_move);
            return best_choice;
        }
        else{
            if(maximizingPlayer){
                std::vector<Move*>::iterator i; 
                best_score = std::numeric_limits<int>::min(); 
                for(i = valid_moves.begin(); i != valid_moves.end(); i++){
                    Board *b2 = b->copy();
                    b2->doMove(*i, my_side);
                    std::pair<int, Move*> choice = 
                        minimax(b2, depth + 1, max_depth, false);
                    if(choice.first > best_score){  
                        best_score = choice.first;
                        best_move = *i;
                    } 
                }
                best_choice = std::make_pair(best_score, best_move);
                return best_choice; 
            }
            else{
                std::vector<Move*>::iterator j; 
                best_score = std::numeric_limits<int>::max(); 
                for(j = valid_moves.begin(); j != valid_moves.end(); j++){
                    Board *b2 = b->copy();
                    b2->doMove(*j, opp_side);
                    std::pair<int, Move*> choice = 
                        minimax(b2, depth + 1, max_depth, true);
                    if(choice.first < best_score){
                        best_score = choice.first;
                        best_move = *j;
                    } 
                }
                best_choice = std::make_pair(best_score, best_move);
                return best_choice;
            }
        } 
    }
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
    std::clock_t start;
    double duration;
    start = std::clock();

    Move *myMove = NULL;
    if(testingMinimax || minimaxPlaying){
        // Process opponent's move (doMove handles NULL moves)
        board->doMove(opponentsMove, opp_side); 

        // Call minimax
        std::pair<int, Move*> best_choice = 
            minimax(board, 0, 2, true);

        Move *best_move = best_choice.second;

        // set board to the best board
        board->doMove(best_move, my_side);
        return best_move;

    }
    else{
        // Process opponent's move (doMove handles NULL moves)
        board->doMove(opponentsMove, opp_side);

        // Calculate my valid moves
        std::vector<Move*> valid_moves = findValid(my_side, board);

        if(!valid_moves.empty() && dumbPlaying){ // Randomly choose a valid move to play
            int index = rand() % valid_moves.size();
            myMove = valid_moves[index];
        }

        if(!valid_moves.empty() && !dumbPlaying){ // choose valid move using heuristic
            //find the best move out of all the valid moves
            std::vector<Move*>::iterator i;
            int best_score = std::numeric_limits<int>::min(); 
            Move *curr;
            int curr_score;
            for(i = valid_moves.begin(); i != valid_moves.end(); i++){
                curr = *i;
                curr_score = heuristicScore(curr, my_side);
                if(curr_score > best_score){
                    best_score = curr_score;
                    myMove = curr;
                }
            }
        }
        // Process my own move
        board->doMove(myMove, my_side);
        fprintf(stderr,"Using %s, Iago (%s) chose Move: (%d, %d)\n",
            (dumbPlaying) ? "random choice": "heuristic",
            (my_side == BLACK) ? "Black": "White", 
            myMove->getX(), myMove->getY());
    }// closes else

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC * MSEC;
    fprintf(stderr, "Time to make move: %f milliseconds\n", duration);
    return myMove;
}
