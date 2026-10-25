# Lab 6: Priority Queue for Student Records

## Overview

`lab6` is a C++ program implementing a priority queue data structure (`RecordHolder`) that manages student records (`StudentRecord`). Student records are sorted based on their entry date, allowing the system to efficiently track, query, and pop the earliest-admitted student.

---

## Program Structure & Components

### 1. Data Structures

- **`EntryDate`**: Stores the date of entry (`year`, `month`, `day`).
- **`StudentRecord`**: Contains student details:
  - `itso`: ITSO username string.
  - `id`: Unique student identifier automatically incremented by `idCounter`.
  - `gender`: Character representing gender (`'M'` or `'F'`).
  - `entry`: `EntryDate` struct instance.
- **`RecordHolder`**: Stores an array of `StudentRecord` elements and tracks the current size `num_records`.

---

## Priority Ordering Rules

Records are prioritized by the **earliest entry date**:
1. **Year** (lower is earlier)
2. **Month** (lower is earlier)
3. **Day** (lower is earlier)

---

## Key Functions & Operations

The program supports interactive operations via a command loop:

| Command | Operation | Description |
| :--- | :--- | :--- |
| `I` | **Insert** | Prompts for student details, constructs a `StudentRecord`, and inserts it into `RecordHolder` in prioritized order. |
| `G` | **Get Earliest** | Returns a reference to the student record with the earliest entry date without removing it. |
| `P` | **Pop Earliest** | Removes and returns the student record with the earliest entry date. |
| `E` | **Check Empty** | Displays whether `RecordHolder` contains any records. |

After executing the designated number of operations, the system outputs all remaining records inside `RecordHolder`.

---

## Compilation & Execution

### Build
Compile `main.cpp` together with your implementation file (e.g., `lab6.cpp`):
```bash
g++ -std=c++11 main.cpp lab6.cpp -o lab6
```

### Run
```bash
./lab6
```

### Sample Input Example
```text
3
I
john_doe
M
2022 9 1
G
P
```