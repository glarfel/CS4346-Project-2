#ifndef EVALUATION_H
#define EVALUATION_H

#include "board.h"

// Dispatcher that picks EV1/EV2/EV3/EV4 based on evalID
int evaluateBoard(const Board &b, char aiPlayer, int evalID);

// Individual evaluation functions
int evaluateEV1(const Board &b, char aiPlayer); // open lines (prof's)
int evaluateEV2(const Board &b, char aiPlayer); // open lines + 2-in-a-row
int evaluateEV3(const Board &b, char aiPlayer); // placeholder for teammate
int evaluateEV4(const Board &b, char aiPlayer); // placeholder for teammate

#endif // EVALUATION_H
