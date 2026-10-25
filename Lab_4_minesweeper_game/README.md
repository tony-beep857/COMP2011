# Lab 4: 5x5 Minesweeper Game

## Overview

`lab4.cpp` is a C++ implementation of a classic text-based **Minesweeper** game played on a $5 \times 5$ grid. The program allows users to select a predefined board configuration, explore cells using row and column coordinates, automatically reveal adjacent non-mine counts, and track game victory or loss conditions.

---

## Core Features & Logic

### 1. Board Representation & Data Structure
- **Grid Layout**: $5 \times 5$ grid mapped to a 1D array of size 25 (`SIZE = 25`).
- **Index Conversion**: Converts 2D coordinates `(row, col)` to 1D array index:
  $$\text{index} = \text{row} \times \text{COLS} + \text{col}$$
- **Boards**:
  - `game_board`: Internal grid containing mines (`'*'`) and safe spaces (`'-'`).
  - `display_board`: Visual grid shown to the player with unrevealed cells (`'-'`), mine counts (`'0'`–`'8'`), clicked mine (`'!'`), or revealed mines (`'*'`).

---

## Key Functions & Implementation

### 1. Board Rendering & Mine Detection
- **`displaying_board(const char display_board[])`**: Prints the $5 \times 5$ display grid to the console.
- **`check_mine(int row, int col, const char game_board[])`**: Returns `true` if `game_board[index]` contains `'*'`.
- **`count_mine(int row, int col, const char game_board[])`**: Checks all 8 neighboring cells around `(row, col)` and returns the total number of adjacent mines.

### 2. Gameplay Tasks

#### Task 1: Update Revealed Cell (`show_number`)
Converts the integer mine count into its character equivalent and updates `display_board`:
```cpp
display_board[index] = '0' + num_mine;
```

#### Task 2: Zero Neighbor Reveal (`reveal_zero_neighbors`)
When a selected cell has 0 adjacent mines, this function inspects its 8 neighboring cells:
- Checks boundary limits (`0 <= r < ROWS`, `0 <= c < COLS`).
- Skips the center cell itself and any neighbor containing a mine.
- Calculates the adjacent mine count for valid neighbor cells and updates their display values using `show_number`.

#### Task 3: Win Condition Check (`game_win`)
Evaluates whether the player has won:
- Returns `false` if any cell contains an exploded mine (`'!'`).
- Returns `false` if any mine cell (`'*'`) has been revealed (`!= '-'`).
- Returns `false` if any non-mine safe cell remains unrevealed (`== '-'`).
- Returns `true` when all safe cells are revealed and all mines remain unclicked.

### 3. Loss & Victory Handling
- **`reveal_mines_on_loss(...)`**: Marks the hit mine with `'!'` and reveals all remaining mines as `'*'`.
- **`show_mine_win(...)`**: Reveals all mine positions with `'*'` upon winning.

---

## Program Flow & Main Loop

1. **Board Selection**: Prompt user to select board index `0`–`5` (defaults to `0` if invalid).
2. **Game Loop**:
   - Prompt user for `row` and `col` inputs ($0 \le \text{row}, \text{col} \le 4$).
   - Validate coordinate bounds.
   - If cell contains a mine: Trigger loss, reveal mines, display final board, end game.
   - If cell is safe:
     - Count adjacent mines (`count_mine`).
     - Display number on board (`show_number`).
     - If count is 0, explore adjacent neighbors (`reveal_zero_neighbors`).
3. **Victory Check**: Check if `game_win` returns `true`. If so, print congratulations, reveal remaining mines, and terminate.

---

## Predefined Game Boards

The program provides 6 hardcoded 1D board layouts (`board0` through `board5`) with varying mine configurations and difficulty levels.

---

## Compilation and Execution

### Using GCC / G++

```bash
# Compile lab4.cpp
g++ -std=c++11 lab4.cpp -o lab4

# Run executable
./lab4
```

### Example Usage

```text
Please enter the game board you want to select: 0
0
- - - - - 
- - - - - 
- - - - - 
- - - - - 
- - - - - 

Please the row and col you want to explore (e.g., 2 3 means row 2 column 3): 0 0
0 0
Smart move~~
0 1 - - - 
1 1 - - - 
- - - - - 
- - - - - 
- - - - - 
```