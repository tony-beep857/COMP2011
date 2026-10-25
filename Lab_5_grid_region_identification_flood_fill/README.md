# Lab 5: Grid Region Identification (Flood Fill)

## Overview

`lab5.cpp` is a C++ program that analyzes a $5 \times 5$ character grid map and identifies contiguous regions of matching symbols. Using a recursive **Flood-Fill / Connected Components Algorithm**, it labels each distinct region with a unique region ID starting from $0$.

---

## Technical Specifications & Constants

- **Dimensions**:
  - `ROWS`: $5$
  - `COLS`: $5$
- **Data Structures**:
  - `grid[ROWS][COLS]` (`char` array): Stores the input map grid entered by the user.
  - `region_id[ROWS][COLS]` (`int` array): Tracks the assigned region ID for each cell. Initialized to `-1` (unvisited).

---

## Core Algorithm: Recursive Flood Fill

The key function is `check_connection`, which uses depth-first recursion to label all connected orthogonal neighbors matching the starting cell's symbol.

### Function Signature
```cpp
void check_connection(int row, int col, int region_id[ROWS][COLS], 
                      int current_id, const char grid[ROWS][COLS], 
                      char current_symbol);
```

### Base Cases (Termination Criteria)
1. **Out of Bounds**: Returns if `row < 0`, `row >= ROWS`, `col < 0`, or `col >= COLS`.
2. **Already Visited**: Returns if `region_id[row][col] != -1`.
3. **Symbol Mismatch**: Returns if `grid[row][col] != current_symbol`.

### Recursive Step
1. Assign `current_id` to `region_id[row][col]`.
2. Recursively check four orthogonal directions:
   - **Up**: `check_connection(row - 1, col, ...)`
   - **Left**: `check_connection(row, col - 1, ...)`
   - **Down**: `check_connection(row + 1, col, ...)`
   - **Right**: `check_connection(row, col + 1, ...)`

---

## Program Execution Workflow

1. **Grid Input**: Prompt the user to enter a $5 \times 5$ grid of characters (e.g., land/water maps like `A`, `B`, `.`, `#`).
2. **Initialization**: Set all elements of `region_id[5][5]` to `-1`.
3. **Region Discovery**:
   - Iterate through the grid cell by cell $(r, c)$ in row-major order.
   - If `region_id[r][c] == -1`, invoke `check_connection(...)` with the current `region_count` ID and increment `region_count`.
4. **Output Display**: Print the generated `region_id` matrix showing the assigned integer IDs for each connected region.

---

## Sample Execution

### Input
```text
A A B B B
A A A B B
C C B B B
C C C C C
C C A A A
```

### Output Grid Output
```text
Please enter the grid of this map: 
A A B B B 
A A A B B 
C C B B B 
C C C C C 
C C A A A 

Here is the region by id: 
0 0 1 1 1 
0 0 0 1 1 
2 2 1 1 1 
2 2 2 2 2 
2 2 3 3 3 
```

---

## Compilation and Running Instructions

### Compilation
Compile using standard `g++` compiler:
```bash
g++ -std=c++11 lab5.cpp -o lab5
```

### Execution
Run the executable:
```bash
./lab5
```