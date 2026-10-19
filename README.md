# Othello AI in C

A terminal Othello (Reversi) game in C with computer opponents built from game-tree search: minimax with alpha-beta pruning, plus positional heuristics.

> Course project for **ESTR1002 Problem Solving by Programming**, The Chinese University of Hong Kong (Nov 2022).

<!-- TODO before publishing: commit the alpha-beta AI source (aiplayer_*.c/.h, which othello.c includes) so the repo builds. Drop or replace the third-party opponent (aiplayer_1155179480) used in mode 3. -->

## Game engine
- **Board:** a 10×10 array with a sentinel border. Playable squares are indices 11–88 (row × 10 + column), so neighbour lookups use 8 fixed offsets (±1, ±9, ±10, ±11) with no bounds checks inside the board.
- **Rules:** move validation in all 8 directions, disc flipping, pass detection when a player has no legal move, game-over detection and final scoring (`board.c`).
- **Modes** (`othello.c`, chosen at start):
  1. Human vs human
  2. Human vs AI (colour assigned at random)
  3. AI vs AI, for benchmarking two engines against each other

## AI strategies
I iterated through increasingly strong players:

| Version | Strategy |
|---|---|
| Greedy | Pick the move that flips the most discs |
| Corner-first heuristic | Static square priorities: take corners first, then edge "A/B" squares, prefer the central 4×4 region, and avoid "X-squares" (diagonally next to corners) that give the opponent a corner |
| Minimax | Depth-limited search (depth 6) over legal moves, with disc count as the leaf evaluation |
| **Minimax + alpha-beta pruning** | Same search with alpha-beta cut-offs, so deeper lookahead fits within the move time limit |
| Endgame solver | With fewer than 6 empty squares left, search to the end of the game for an exact result |

`aiplayer_1155184202_2.c` contains the heuristic + depth-limited lookahead player.

## Build and play

```bash
gcc -O2 -o othello othello.c board.c aiplayer_*.c
./othello      # then enter mode: 1, 2 or 3
```

Enter moves as a two-digit board index: row then column, e.g. `34` = row 3, column 4.

## What I'd do next
- Replace disc-count evaluation with mobility + stability + positional weights
- Add move ordering and iterative deepening to get more out of alpha-beta
- Add bitboard representation for speed
