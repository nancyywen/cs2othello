#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    minimaxPlaying = false; // true if using minimax
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
 * different in the number of pieces that each player 
 */
int Player::boardScore(Side side, Board *b){
    int my_score = 0;
    int opp_score = 0;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(b->occupied(i, j)){
                if(b->get(side, i, j)){
                    my_score += 1;
                }
                else{
                    opp_score += 1;
                }
            }
        }
    }
    return my_score - opp_score;
}


/* This struct represents a node on the minimax tree */
struct Node { 
    Board* board;
    Move* branch;
    int score;
};

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
        std::cerr << "Testing minimax" << std::endl;
        // Process opponent's move (doMove handles NULL moves)
        board->doMove(opponentsMove, opp_side); 

        // Build the minimax tree

        // Layer 1:
        // Children of root node created for possibles moves for my_side
        std::vector<Move*> valid_moves = findValid(my_side, board);
        std::vector<Move*>::iterator i; // for valid_moves
        std::vector<Node> tree1; // nodes in first layer of tree
        for(i = valid_moves.begin(); i != valid_moves.end(); i++){
            Node n1;
            n1.branch = *i;
            std::cerr << "Layer 1: " << (n1.branch)->getX() <<
                             ", " << (n1.branch)->getY() << std::endl;
            Board *b1 = board->copy();
            b1->doMove(*i, my_side);
            n1.board = b1;
            tree1.push_back(n1);
        }

        // Layer 2: possible moves for opp_side
        std::vector<Node>::iterator j; // for tree1
        std::vector<Node> tree2; // nodes in second layer of tree
        
        for (j = tree1.begin(); j != tree1.end(); j++){
            int min_score = std::numeric_limits<int>::max(); 
            Node curr_node = *j;
            Board *curr_board = curr_node.board;
            std::vector<Move*> valid_moves = findValid(opp_side, curr_board);
            std::cerr << (curr_node.branch)->getX() << ", " << (curr_node.branch)->getY() << std::endl;
            std::vector<Move*>::iterator i; // for valid_moves
            for(i = valid_moves.begin(); i != valid_moves.end(); i++){
                Node n2;
                n2.branch = *i;
                std::cerr << "Layer 2: " << (n2.branch)->getX() <<
                             ", " << (n2.branch)->getY() << std::endl;
                Board *b2 = curr_board->copy();
                b2->doMove(*i, opp_side);
                n2.score = boardScore(my_side, b2);
                if(n2.score < min_score){
                    min_score = n2.score;
                }
                n2.board = b2;
                tree2.push_back(n2);
            }
            curr_node.score = min_score;
        }

        int max_score = std::numeric_limits<int>::min(); 
        Move* best_move;
        for (j = tree1.begin(); j != tree1.end(); j++){
            std::cerr << "Choosing minimax" << std::endl;
            Node curr_node = *j;
            if(curr_node.score > max_score){
                max_score = curr_node.score;
                best_move = curr_node.branch;
            }
        }

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
