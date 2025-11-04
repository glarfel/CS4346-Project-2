#ifndef BOARD_H
#define BOARD_H

#include "utils.h"

struct Board {
    char cells[3][3];
    Board();
};

void printBoard(const Board &b);
bool isMovesLeft(const Board &b);

// Returns:
//   'X' if X wins
//   'O' if O wins
//   'D' for draw
//   'N' for no result yet
char checkGameState(const Board &b);

#endif // BOARD_H
