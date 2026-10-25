# Lab 3: Degree Classification Statistics System

## Overview

`lab3.cpp` is a C++ program designed to process student academic performance data from an input text file and generate summary graduation statistics. It reads paired values representing Cumulative Grade Average (**CGA**) and Major Cumulative Grade Average (**MCGA**), evaluates degree honors classification, updates running statistics for graduates, and prints a final summary report.

---

## Core Features & Logic

### 1. File Input/Output Processing (`main`)
* Prompts the user to enter a file path (up to 100 characters).
* Opens the file stream using `std::fstream`.
* Iteratively reads `cga` and `mcga` values from the file until EOF (End of File) is reached.
* Closes the file stream after processing.

### 2. Degree Classification (`degree_classification`)
Evaluates student eligibility for graduation based on CGA and MCGA metrics. Increments corresponding counters and returns `true` if the student qualifies for a degree, or `false` if they fail to qualify (`None`).

| Classification Tier | Min MCGA Threshold | Min CGA Threshold |
| :--- | :--- | :--- |
| **First Class Honors** | $\ge 3.60$ | $\ge 3.40$ |
| **Second Class Honors, Division I** | $\ge 2.85$ | $\ge 2.70$ |
| **Second Class Honors, Division II** | $\ge 2.15$ | $\ge 2.00$ |
| **Third Class Honors** | $\ge 1.70$ | $\ge 1.70$ |
| **Pass** | $\ge 0.85$ | $\ge 0.85$ |
| **None (Fail/No Degree)** | $< 0.85$ | $< 0.85$ |

> **Note:** Comparison helper function `greater_or_equal(a, b)` handles floating-point precision comparisons ($|a - b| < 10^{-12}$).

### 3. Statistics & Running Averages (`update_averages`)
When a student successfully graduates (`graduated == true`), the overall graduate statistics are updated:
1. **Incremental Average Formula:**
   $$\text{avg\_cga}_{\text{new}} = \frac{\text{avg\_cga}_{\text{old}} \cdot N + \text{cga}}{N + 1}$$
   $$\text{avg\_mcga}_{\text{new}} = \frac{\text{avg\_mcga}_{\text{old}} \cdot N + \text{mcga}}{N + 1}$$
2. Increment total graduate count $N = N + 1$.

---

## Input File Format

The input text file should contain pairs of floating-point numbers separated by whitespace or newlines representing CGA and MCGA respectively.

**Example Input File (`students.txt`):**
```text
3.85 3.90
2.80 2.90
1.80 1.75
0.50 0.60
3.50 3.65
```

---

## Output Structure

The program outputs individual classification messages per line followed by summary statistics:

```text
Student with CGA = 3.85 and MCGA = 3.9 graduated with: First Class Honors
Student with CGA = 2.8 and MCGA = 2.9 graduated with: Second Class Honors, Division I
Student with CGA = 1.8 and MCGA = 1.75 graduated with: Third Class Honors
Student with CGA = 0.5 and MCGA = 0.6 graduated with: None
Student with CGA = 3.5 and MCGA = 3.65 graduated with: First Class Honors
Statistics:
Number of graduates: 4
Number of graduates with First Class Honors: 2
Number of graduates with Second Class Honors, Division I: 1
Number of graduates with Second Class Honors, Division II: 0
Number of graduates with Third Class Honors: 1
Number of graduates with Pass: 0
--------------------------------------------------------------------
Average graduate CGA: 2.9875
Average graduate MCGA: 3.05
```

---

## Compilation & Execution

### Compile
Using `g++` (GCC) compiler:
```bash
g++ -std=c++11 lab3.cpp -o lab3
```

### Run
```bash
./lab3
```
When prompted, provide the path to your data file (e.g., `students.txt`).