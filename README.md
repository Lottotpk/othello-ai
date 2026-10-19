# Othello AI in C

A terminal Othello (Reversi) game in C, with a computer opponent that uses minimax search with alpha-beta pruning and positional rules.

> Originally a course project for **ESTR1002 Problem Solving by Programming**, The Chinese University of Hong Kong (2022). Refactored for readability.

## Build and play

```bash
gcc -std=c99 -Wall -Wextra -O2 -o othello main.c board.c ai_player.c
./othello
```

Enter a game mode:
- `1`: human vs human
- `2`: human vs computer (colours assigned at random; black moves first)

Enter moves as a two-digit board index, row then column. For example, `34` means row 3, column 4.

## Code structure

| File | Contents |
|---|---|
| `board.h` / `board.c` | Board representation, move validation, disc flipping, display, human input |
| `ai_player.h` / `ai_player.c` | Computer opponent: move generation, positional rules, alpha-beta search |
| `main.c` | Game loop and game modes |

**Board representation:** a 10×10 array with a one-cell border around the 8×8 playing area. Square (row, col) is stored at index `10*row + col`, so playable squares are indices 11–88. Neighbouring squares are fixed offsets (±1, ±9, ±10, ±11), so the code can walk in any direction without bounds checks.

## How the AI chooses a move

1. **Corners first.** It takes a corner whenever one is legal, because a corner disc can never be flipped.
2. **Search.** Otherwise it runs an 8-ply minimax search with alpha-beta pruning. Each move is made and then undone in place, using a list of the flipped discs, so the board is never copied inside the search.
3. **Two-phase evaluation.**
   - **Opening and middlegame** (fewer than 30 discs on the board): the AI *minimises* its own disc count. This is the "evaporation" strategy: holding fewer discs early keeps the AI's mobility high and the opponent's low. On its own turn, the AI searches edge A- and B-squares first.
   - **Late game:** the AI *maximises* its own disc count.
4. **Positional rules inside the search.**
   - The AI avoids **X-squares** (diagonally next to a corner) unless it has no other move, because they give the opponent the corner.
   - It plays a **C-square** (on the edge, next to a corner) only if the edge then holds no opponent disc and at least five of its own.
   - A line where either side takes a corner is scored immediately as best or worst, without searching further.

## Known limitations
- The search has no move ordering, and it lists a move once per capturing direction. In some busy late-middlegame positions, an 8-ply search can take up to about a minute.

## Possible improvements
- Replace the disc-count evaluation with a weighted mix of mobility, stability and positional value.
- Add move ordering and iterative deepening, so alpha-beta prunes more and the search can go deeper in the same time.
- Handle passes inside the search, and generate each legal move once instead of once per capturing direction.
- Use a bitboard representation for speed.
