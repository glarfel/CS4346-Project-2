#include <iostream>
#include <vector>
#include <limits>
#include <chrono>

using namespace std;

// ------------------- Constants -------------------

const char EMPTY    = ' ';
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const int  MAX_DEPTH = 5;   // depth cutoff so eval functions matter

// ------------------- Board -----------------------

struct Board {
    char cells[3][3];
    Board() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                cells[i][j] = EMPTY;
    }
};

void printBoard(const Board &b) {
    cout << "\n";
    for (int i = 0; i < 3; ++i) {
        cout << " ";
        for (int j = 0; j < 3; ++j) {
            cout << b.cells[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "---|---|---\n";
    }
    cout << "\n";
}

bool isMovesLeft(const Board &b) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (b.cells[i][j] == EMPTY)
                return true;
    return false;
}

// Returns 'X' if X wins, 'O' if O wins, 'D' for draw, 'N' for no result yet.
char checkGameState(const Board &b) {
    // Rows & columns
    for (int i = 0; i < 3; ++i) {
        if (b.cells[i][0] != EMPTY &&
            b.cells[i][0] == b.cells[i][1] &&
            b.cells[i][1] == b.cells[i][2])
            return b.cells[i][0];

        if (b.cells[0][i] != EMPTY &&
            b.cells[0][i] == b.cells[1][i] &&
            b.cells[1][i] == b.cells[2][i])
            return b.cells[0][i];
    }

    // Diagonals
    if (b.cells[0][0] != EMPTY &&
        b.cells[0][0] == b.cells[1][1] &&
        b.cells[1][1] == b.cells[2][2])
        return b.cells[0][0];

    if (b.cells[0][2] != EMPTY &&
        b.cells[0][2] == b.cells[1][1] &&
        b.cells[1][1] == b.cells[2][0])
        return b.cells[0][2];

    // Draw?
    if (!isMovesLeft(b))
        return 'D';

    return 'N';  // game not finished
}

// ------------------- Evaluation helpers -------------------

bool isLineOpen(char a, char b, char c, char player, char opp) {
    // Line is "open" for player if opponent has NO marks there
    return (a != opp && b != opp && c != opp);
}

int countTwoInRowLine(char a, char b, char c, char player, char opp) {
    int p = 0, o = 0;
    if (a == player) p++;
    if (b == player) p++;
    if (c == player) p++;

    if (a == opp) o++;
    if (b == opp) o++;
    if (c == opp) o++;

    if (p == 2 && o == 0) return 1;
    return 0;
}

// ------------------- EV1: prof's open-lines heuristic -------------------
//
// EV1 = (# of open lines for aiPlayer) - (# of open lines for opponent)
// with big +/- scores for win/loss and 0 for draw.

int evaluateEV1(const Board &b, char aiPlayer) {
    char opp = (aiPlayer == PLAYER_X ? PLAYER_O : PLAYER_X);

    char state = checkGameState(b);
    if (state == aiPlayer) return 100000;   // win
    if (state == opp)     return -100000;   // loss
    if (state == 'D')     return 0;         // draw

    int openForAI  = 0;
    int openForOpp = 0;

    // Rows
    for (int i = 0; i < 3; ++i) {
        if (isLineOpen(b.cells[i][0], b.cells[i][1], b.cells[i][2], aiPlayer, opp))
            openForAI++;
        if (isLineOpen(b.cells[i][0], b.cells[i][1], b.cells[i][2], opp, aiPlayer))
            openForOpp++;
    }

    // Columns
    for (int j = 0; j < 3; ++j) {
        if (isLineOpen(b.cells[0][j], b.cells[1][j], b.cells[2][j], aiPlayer, opp))
            openForAI++;
        if (isLineOpen(b.cells[0][j], b.cells[1][j], b.cells[2][j], opp, aiPlayer))
            openForOpp++;
    }

    // Diagonals
    if (isLineOpen(b.cells[0][0], b.cells[1][1], b.cells[2][2], aiPlayer, opp))
        openForAI++;
    if (isLineOpen(b.cells[0][0], b.cells[1][1], b.cells[2][2], opp, aiPlayer))
        openForOpp++;

    if (isLineOpen(b.cells[0][2], b.cells[1][1], b.cells[2][0], aiPlayer, opp))
        openForAI++;
    if (isLineOpen(b.cells[0][2], b.cells[1][1], b.cells[2][0], opp, aiPlayer))
        openForOpp++;

    return openForAI - openForOpp;
}

// ------------------- EV2: open lines + strong 2-in-a-row bonus -------------------
//
// EV2 = 5 * (twoInRow_AI - twoInRow_Opp) + 1 * (open_AI - open_Opp)

int evaluateEV2(const Board &b, char aiPlayer) {
    char opp = (aiPlayer == PLAYER_X ? PLAYER_O : PLAYER_X);

    char state = checkGameState(b);
    if (state == aiPlayer) return 100000;
    if (state == opp)     return -100000;
    if (state == 'D')     return 0;

    int openForAI  = 0;
    int openForOpp = 0;
    int twoForAI   = 0;
    int twoForOpp  = 0;

    // Rows
    for (int i = 0; i < 3; ++i) {
        char a = b.cells[i][0];
        char c = b.cells[i][1];
        char d = b.cells[i][2];

        if (isLineOpen(a, c, d, aiPlayer, opp))
            openForAI++;
        if (isLineOpen(a, c, d, opp, aiPlayer))
            openForOpp++;

        twoForAI  += countTwoInRowLine(a, c, d, aiPlayer, opp);
        twoForOpp += countTwoInRowLine(a, c, d, opp, aiPlayer);
    }

    // Columns
    for (int j = 0; j < 3; ++j) {
        char a = b.cells[0][j];
        char c = b.cells[1][j];
        char d = b.cells[2][j];

        if (isLineOpen(a, c, d, aiPlayer, opp))
            openForAI++;
        if (isLineOpen(a, c, d, opp, aiPlayer))
            openForOpp++;

        twoForAI  += countTwoInRowLine(a, c, d, aiPlayer, opp);
        twoForOpp += countTwoInRowLine(a, c, d, opp, aiPlayer);
    }

    // Diagonal 1
    {
        char a = b.cells[0][0];
        char c = b.cells[1][1];
        char d = b.cells[2][2];

        if (isLineOpen(a, c, d, aiPlayer, opp))
            openForAI++;
        if (isLineOpen(a, c, d, opp, aiPlayer))
            openForOpp++;

        twoForAI  += countTwoInRowLine(a, c, d, aiPlayer, opp);
        twoForOpp += countTwoInRowLine(a, c, d, opp, aiPlayer);
    }

    // Diagonal 2
    {
        char a = b.cells[0][2];
        char c = b.cells[1][1];
        char d = b.cells[2][0];

        if (isLineOpen(a, c, d, aiPlayer, opp))
            openForAI++;
        if (isLineOpen(a, c, d, opp, aiPlayer))
            openForOpp++;

        twoForAI  += countTwoInRowLine(a, c, d, aiPlayer, opp);
        twoForOpp += countTwoInRowLine(a, c, d, opp, aiPlayer);
    }

    int scoreOpen = openForAI - openForOpp;
    int scoreTwo  = twoForAI  - twoForOpp;

    return 5 * scoreTwo + scoreOpen;
}

// EV3
int evaluateEV3(const Board &b, char aiPlayer) {
    char opp = (aiPlayer == PLAYER_X ? PLAYER_O : PLAYER_X);

    // Handle terminal states first
    char state = checkGameState(b);
    if (state == aiPlayer) return 100000;
    if (state == opp)     return -100000;
    if (state == 'D')     return 0;

    // Start from EV1 (open-lines heuristic)
    int score = evaluateEV1(b, aiPlayer);

    // Positional weights:
    // center = 3, corners = 2, edges = 1
    int positional = 0;

    auto addPositional = [&](int i, int j, int weight) {
        if (b.cells[i][j] == aiPlayer)      positional += weight;
        else if (b.cells[i][j] == opp)      positional -= weight;
    };

    // Center
    addPositional(1, 1, 3);

    // Corners
    addPositional(0, 0, 2);
    addPositional(0, 2, 2);
    addPositional(2, 0, 2);
    addPositional(2, 2, 2);

    // Edges
    addPositional(0, 1, 1);
    addPositional(1, 0, 1);
    addPositional(1, 2, 1);
    addPositional(2, 1, 1);

    return score + positional;
}

// EV4
int evaluateEV4(const Board &b, char aiPlayer) {
    char opp = (aiPlayer == PLAYER_X ? PLAYER_O : PLAYER_X);

    // Terminal states
    char state = checkGameState(b);
    if (state == aiPlayer) return 100000;
    if (state == opp)     return -100000;
    if (state == 'D')     return 0;

    int openForAI  = 0;
    int openForOpp = 0;
    int twoForAI   = 0;
    int twoForOpp  = 0;

    // Rows
    for (int i = 0; i < 3; ++i) {
        char a = b.cells[i][0];
        char c = b.cells[i][1];
        char d = b.cells[i][2];

        if (isLineOpen(a, c, d, aiPlayer, opp))
            openForAI++;
        if (isLineOpen(a, c, d, opp, aiPlayer))
            openForOpp++;

        twoForAI  += countTwoInRowLine(a, c, d, aiPlayer, opp);
        twoForOpp += countTwoInRowLine(a, c, d, opp, aiPlayer);
    }

    // Columns
    for (int j = 0; j < 3; ++j) {
        char a = b.cells[0][j];
        char c = b.cells[1][j];
        char d = b.cells[2][j];

        if (isLineOpen(a, c, d, aiPlayer, opp))
            openForAI++;
        if (isLineOpen(a, c, d, opp, aiPlayer))
            openForOpp++;

        twoForAI  += countTwoInRowLine(a, c, d, aiPlayer, opp);
        twoForOpp += countTwoInRowLine(a, c, d, opp, aiPlayer);
    }

    // Diagonal 1
    {
        char a = b.cells[0][0];
        char c = b.cells[1][1];
        char d = b.cells[2][2];

        if (isLineOpen(a, c, d, aiPlayer, opp))
            openForAI++;
        if (isLineOpen(a, c, d, opp, aiPlayer))
            openForOpp++;

        twoForAI  += countTwoInRowLine(a, c, d, aiPlayer, opp);
        twoForOpp += countTwoInRowLine(a, c, d, opp, aiPlayer);
    }

    // Diagonal 2
    {
        char a = b.cells[0][2];
        char c = b.cells[1][1];
        char d = b.cells[2][0];

        if (isLineOpen(a, c, d, aiPlayer, opp))
            openForAI++;
        if (isLineOpen(a, c, d, opp, aiPlayer))
            openForOpp++;

        twoForAI  += countTwoInRowLine(a, c, d, aiPlayer, opp);
        twoForOpp += countTwoInRowLine(a, c, d, opp, aiPlayer);
    }

    int scoreOpen = openForAI - openForOpp;

    // Defensive emphasis:
    // - reward your 2-in-a-rows
    // - strongly punish opponent 2-in-a-rows
    int scoreTwo = (2 * twoForAI) - (4 * twoForOpp);

    // Combine with stronger weight on the defensive part
    return 2 * scoreOpen + 8 * scoreTwo;
}

// Dispatcher
int evaluateBoard(const Board &b, char aiPlayer, int evalID) {
    switch (evalID) {
        case 1: return evaluateEV1(b, aiPlayer);
        case 2: return evaluateEV2(b, aiPlayer);
        case 3: return evaluateEV3(b, aiPlayer);
        case 4: return evaluateEV4(b, aiPlayer);
        default: return evaluateEV1(b, aiPlayer);
    }
}

// ------------------- Minimax + Alpha-Beta -------------------

struct SearchStats {
    long long nodesGenerated = 0;
    long long nodesExpanded  = 0;
};

int minimaxAB(Board &b,
              int depth,
              bool isMaxNode,
              int alpha,
              int beta,
              char aiPlayer,
              char oppPlayer,
              int evalID,
              SearchStats &stats) {
    stats.nodesGenerated++;

    char state = checkGameState(b);
    if (state == aiPlayer)  return 1000 - depth;
    if (state == oppPlayer) return -1000 + depth;
    if (state == 'D')       return 0;

    if (depth >= MAX_DEPTH) {
        return evaluateBoard(b, aiPlayer, evalID);
    }

    stats.nodesExpanded++;

    if (isMaxNode) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (b.cells[i][j] == EMPTY) {
                    b.cells[i][j] = aiPlayer;
                    int val = minimaxAB(b, depth + 1, false, alpha, beta,
                                        aiPlayer, oppPlayer, evalID, stats);
                    b.cells[i][j] = EMPTY;

                    if (val > best) best = val;
                    if (val > alpha) alpha = val;
                    if (beta <= alpha) return best; // alpha-beta cutoff
                }
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (b.cells[i][j] == EMPTY) {
                    b.cells[i][j] = oppPlayer;
                    int val = minimaxAB(b, depth + 1, true, alpha, beta,
                                        aiPlayer, oppPlayer, evalID, stats);
                    b.cells[i][j] = EMPTY;

                    if (val < best) best = val;
                    if (val < beta) beta = val;
                    if (beta <= alpha) return best; // alpha-beta cutoff
                }
            }
        }
        return best;
    }
}

// ------------------- Move selection -------------------

struct Move {
    int row;
    int col;
    int value;
};

Move findBestMove(Board &b, char currentPlayer, int evalID, SearchStats &stats) {
    char aiPlayer  = currentPlayer;
    char oppPlayer = (currentPlayer == PLAYER_X ? PLAYER_O : PLAYER_X);

    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
    bestMove.value = numeric_limits<int>::min(); // root always maximizes

    int alpha = numeric_limits<int>::min();
    int beta  = numeric_limits<int>::max();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (b.cells[i][j] == EMPTY) {
                b.cells[i][j] = aiPlayer;
                int val = minimaxAB(b, 1, false, alpha, beta,
                                    aiPlayer, oppPlayer, evalID, stats);
                b.cells[i][j] = EMPTY;

                if (val > bestMove.value) {
                    bestMove.value = val;
                    bestMove.row = i;
                    bestMove.col = j;
                }

                if (val > alpha) alpha = val;
            }
        }
    }

    return bestMove;
}

// ------------------- main -------------------

int main() {
    Board board;
    vector<Board> path;
    path.push_back(board);

    int evalX, evalO;
    cout << "Enter eval ID for X (1-4): ";
    cin >> evalX;
    cout << "Enter eval ID for O (1-4): ";
    cin >> evalO;

    char currentPlayer = PLAYER_X;

    long long totalNodesGenerated = 0;
    long long totalNodesExpanded  = 0;

    auto start = chrono::high_resolution_clock::now();

    while (true) {
        char state = checkGameState(board);
        if (state != 'N') {
            cout << "Final board:\n";
            printBoard(board);

            if (state == PLAYER_X)      cout << "Result: X wins\n";
            else if (state == PLAYER_O) cout << "Result: O wins\n";
            else                        cout << "Result: Draw\n";
            break;
        }

        cout << "Current board (player " << currentPlayer << " to move):\n";
        printBoard(board);

        SearchStats moveStats;
        Move best;
        if (currentPlayer == PLAYER_X) {
            best = findBestMove(board, PLAYER_X, evalX, moveStats);
        } else {
            best = findBestMove(board, PLAYER_O, evalO, moveStats);
        }

        if (best.row == -1 || best.col == -1) {
            cout << "No possible moves. Game over.\n";
            break;
        }

        cout << "Player " << currentPlayer << " chooses: ("
             << best.row << ", " << best.col << ") with value "
             << best.value << "\n";

        board.cells[best.row][best.col] = currentPlayer;
        path.push_back(board);

        totalNodesGenerated += moveStats.nodesGenerated;
        totalNodesExpanded  += moveStats.nodesExpanded;

        currentPlayer = (currentPlayer == PLAYER_X ? PLAYER_O : PLAYER_X);
    }

    auto stop = chrono::high_resolution_clock::now();
    auto durationMs = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "\n==============================\n";
    cout << "Game path (states sequence):\n";
    for (size_t k = 0; k < path.size(); ++k) {
        cout << "State #" << k << ":\n";
        printBoard(path[k]);
    }

    cout << "==============================\n";
    cout << "Total nodes generated: " << totalNodesGenerated << "\n";
    cout << "Total nodes expanded : " << totalNodesExpanded  << "\n";
    cout << "Total execution time : " << durationMs.count() << " ms\n";

    return 0;
}
