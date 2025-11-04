## CS 4346 — Project #2 (Fall 2025)

**Due Date:** November 12, 2025

### Overview

This project requires you to implement the MINIMAX-A-B algorithm to build a Tic‑Tac‑Toe game-playing program (Max vs Min). Each team (up to 3 students) will implement the core Minimax with alpha–beta pruning algorithm and develop evaluation functions to compare performance.

Teams must email their member names to the instructor promptly. (Note in original instructions: -5 points if not received by October 22.)

### Team & Language

- Team size: up to 3 students.
- Preferred languages: C or C++ (discuss with instructor if you want to use another language).

### Project Requirements (high-level)

1. Implement the MINIMAX-A-B algorithm as described in the project specification.
2. Implement one baseline evaluation function (class example) plus additional evaluation functions — one different function per team member (so a team of 3 provides 4 evaluation functions total: baseline + 3 member functions).
3. Run head-to-head experiments: run Minimax-A-B with the evaluation functions in the combinations listed below, collect metrics, and analyze results.

### Experimental runs

Run the following 6 pairings (Max vs Min) to evaluate the relative performance of evaluation functions:

- EV#1 vs EV#2
- EV#1 vs EV#3
- EV#1 vs EV#4
- EV#2 vs EV#3
- EV#2 vs EV#4
- EV#3 vs EV#4

For each run collect and tabulate:

- total nodes generated
- total nodes expanded
- execution time
- memory usage (approximate)
- winning/losing/draw statistics

Analyze the aggregated results and explain which evaluation functions perform better and why.

### What to measure and report (per student)

Each team member must individually perform steps 5-1 and 5-2 from the spec. That includes:

- Running and recording the program outputs for the required pairings.
- Tabulating metrics listed above for each run.
- Writing an individual section in the report with analysis, conclusions, and observations.

### Algorithm (summary)

Implement MINIMAX-A-B with the usual recursive structure and alpha–beta thresholds (Use-Thresh and Pass-Thresh). If the recursion has reached a depth limit or a terminal state, return the static evaluation for the current player. Otherwise generate successors and apply the pruning logic described in the project description.

(The original project description contains the full pseudocode to follow — include it in the report as needed.)

### Report format (required)

Your project report must follow the format below (include everything in the report — no separate attachments):

- Title Page (title, full names, team member names, submission date)
- Problem Description
- Domain explanation
- Methodologies (detailed description of MINIMAX-A-B and any design choices)
- Source code implementation details and explanations
- Full copy of source code included inside the report (not as an attachment)
- Program runs and tabulated results for the six experiments
- Analysis of the program (features added/modified)
- Analysis of the results (interpretation of metrics)
- Conclusion (what you learned)
- Team members’ contributions (detailed per-person)
- References

Rubric details (length/points) were provided in the original assignment — make sure to follow them.

### Submission instructions

- Upload the project report to Canvas (Project #2 assignment). The report must be self-contained (do not send email). Failure to follow the Canvas upload procedure may result in a large point deduction.
- Upload source code and any additional files required to run your program on Canvas as separate files. File naming convention for source files: `Project2-<studentID>.cpp` (or similar — keep naming consistent per instructions).

Canvas submission steps (summary):

1. Login to Canvas → Course: CS 4346 Fall 2025
2. Assignments → Project #2 → Submit Assignment
3. Choose and upload files (one by one)
4. Agree to any EULA and click Submit

### Academic integrity

Each student must perform steps 5-1 and 5-2 individually. Do not copy programs or reports from others. Plagiarism or copying will be penalized.

### Quick checklist for deliverables

- Project report (complete, self-contained) — uploaded to Canvas
- Source code files and related resources — uploaded to Canvas
- Readme file with run instructions included in the upload

### Instructor / Contact

Dr. Ali (see course materials for office/demo details).

---

If you want, I can:

- shorten the README further into a one-page summary for your repo root,
- or keep and include the full pseudocode and rubric as appendices inside the README.

Tell me which option you prefer or if you want extra sections (examples, run commands, sample outputs) added.
