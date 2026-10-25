# Lab 1: Pass or Fail Grade Calculator

## Overview
`lab1.cpp` is a C++ starter program designed to determine whether a student passes or fails a course based on their scores across various coursework components: **Lab Exercises**, **Programming Assignments (PAs)**, **Midterm Exam**, and **Final Exam**.

The program prompts the user to enter raw scores for each component, calculates weighted overall and exam-only scores, evaluates passing conditions against established thresholds, and displays the final pass/fail verdict.

---

## Grade Weight distribution & Thresholds

### Scoring Weights
| Component | Max Score | Weight |
| :--- | :--- | :--- |
| **Lab Exercises** | 24 | 10% |
| **Programming Assignments (PAs)** | 300 | 24% |
| **Midterm Exam** | 100 | 26% |
| **Final Exam** | 100 | 40% |
| **Total Weight** | | **100%** |

### Passing Thresholds
* **Exam-Only Threshold (`PASSING_EXAM_ONLY`):** 14 points (calculated from weighted Midterm + Final score out of a max possible 66 points).
* **Overall Threshold (`PASSING_OVERALL`):** 36 points (calculated from overall weighted total out of 100 points).

> **Pass Condition:** A student must pass **both** the exam-only requirement and the overall requirement (`passExamOnly && passOverall`).

---

## Formula Requirements for Implementation

To complete the program logic in the designated section (`=== Start:` to `=== End:`), calculate the following:

1. **Weighted Exam-Only Score (`weightedExamOnly`):**
   $$\text{weightedExamOnly} = \left(\frac{\text{midtermScore}}{\text{MAX\_MIDTERM}} \times \text{WEIGHT\_MIDTERM}\right) + \left(\frac{\text{finalScore}}{\text{MAX\_FINAL}} \times \text{WEIGHT\_FINAL}\right)$$

2. **Weighted Overall Score (`weightedOverall`):**
   $$\text{weightedOverall} = \left(\frac{\text{labScore}}{\text{MAX\_LABS}} \times \text{WEIGHT\_LABS}\right) + \left(\frac{\text{paScore}}{\text{MAX\_PAS}} \times \text{WEIGHT\_PAS}\right) + \text{weightedExamOnly}$$

3. **Pass Criteria Flags:**
   * `passExamOnly = (weightedExamOnly >= PASSING_EXAM_ONLY)`
   * `passOverall = (weightedOverall >= PASSING_OVERALL)`

---

## Prerequisites
* A C++ compiler (e.g., `g++`, `clang++`, or MSVC).
* Standard C++ runtime environment (C++11 or higher recommended).

---

## Compilation & Execution

### 1. Compile the Code
Using `g++`:
```bash
g++ -o lab1 lab1.cpp
```

### 2. Run the Executable
On Linux/macOS:
```bash
./lab1
```

On Windows:
```cmd
lab1.exe
```

---

## Example Input / Output

### Input
```text
Enter your total lab score (Max: 24): 20
Enter your total PA score (Max: 300): 250
Enter your midterm score (Max: 100): 70
Enter your final score (Max: 100): 75
```

### Output (When correctly implemented)
```text
Your exam only weighted score is: 48.2
Your overall score is: 76.533
You passed! Take a rest!
```