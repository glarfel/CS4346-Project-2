# CS 4346 – Project #2
## Tic-Tac-Toe Using Minimax with Alpha–Beta Pruning

---

## How to Compile

From the project root directory:
```bash
g++ src/main.cpp -o ttt -std=c++17
```

Or on Windows PowerShell:
```bash
g++ src\main.cpp -o ttt -std=c++17
```

This will create an executable named `ttt` or `ttt.exe`.

---

## How to Run
```bash
./ttt
```

Or on Windows:
```bash
.\ttt.exe
```

When the program starts, it will ask:
```
Enter eval ID for X (1-4):
Enter eval ID for O (1-4):
```

Enter any combination of the four evaluation functions:

- **1** — EV1 (open-lines heuristic; based on professor's slide)
- **2** — EV2 (open-lines + 2-in-a-row weighting)
- **3** — EV3 (open-lines + positional weighting)
- **4** — EV4 (defensive strategy; penalizes opponent threats)

---

## Program Output

For every run, the program displays:

- Initial Tic-Tac-Toe board
- Intermediate states for each move
- Final board
- Winner (X, O, or Draw)
- Total nodes generated
- Total nodes expanded
- Total execution time (milliseconds)
- Approximate memory used for storing the game path

The program also prints the full sequence of boards (path) generated during the game.

---

## Required Experiments

Run the program with the following evaluation-function combinations:

| Run | X Eval | O Eval |
|-----|--------|--------|
| 1   | 1      | 2      |
| 2   | 1      | 3      |
| 3   | 1      | 4      |
| 4   | 2      | 3      |
| 5   | 2      | 4      |
| 6   | 3      | 4      |

Record the results (winner, nodes, time, memory) and include them in your project report.

---

## File Structure
```
CS4346-Project-2/
│
├── src/
│   └── main.cpp      # Full project source code (Minimax + A/B + EV1–EV4)
│
└── README.md         # This file
```

**Note:** Only `main.cpp` is required for project submission.

---

## Notes

- The project uses a depth cutoff (`MAX_DEPTH = 5`) so evaluation functions meaningfully influence the search.
- All evaluation functions are implemented directly in `main.cpp` as required by the project instructions.
- The code prints everything needed for the report: path, nodes, time, and memory.

---