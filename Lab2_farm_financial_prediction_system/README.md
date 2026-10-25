# Lab 2: Farm Financial Prediction System

## Overview

`lab2.cpp` is a C++ simulation program that predicts a farm's financial condition over a 10-quarter period ($2.5$ years). Based on initial user inputs for chicken population, corn farming area, henhouse capacity, and chicken sales strategy, the program simulates quarterly corn consumption, chicken population growth, chicken sales, and henhouse expansion while tracking revenues, expenditures, and net balance.

## Constants & Parameters

The simulation uses fixed values for pricing and rates throughout the 10 quarters (`QUARTERS = 10`):

| Constant | Value | Description |
| :--- | :--- | :--- |
| `CORN_CONSUMPTION_RATE` | `0.1` | Acres of corn consumed by one chicken per quarter |
| `HENHOUSE_PRICE` | `$1000` | Cost to purchase one additional henhouse |
| `CHICKEN_PRICE` | `$100` | Revenue earned per chicken sold |
| `CORN_PRICE` | `$100` | Price per acre of corn (bought or sold) |

---

## User Inputs

At runtime, the program prompts for four positive values:

1. **Initial Chicken Count (`num_chick`)**: Number of chickens on the farm.
2. **Corn Growing Area (`area_growing_corn`)**: Acres of corn grown per quarter.
3. **Henhouse Capacity (`henhouse_capacity`)**: Maximum capacity per henhouse (must be larger than initial chicken count).
4. **Chicken Sale Portion (`chicken_sale_portion`)**: The fraction of chickens to sell when capacity is exceeded (valid options: `0.6`, `0.7`, `0.8`, `0.9`).

---

## Quarterly Simulation Tasks

Inside the 10-quarter `for` loop (`for (int q = 1; q <= QUARTERS; q++)`), you need to implement three main tasks:

### Task 1: Corn & Feed Calculations
1. **Quarterly Consumption:** Calculate total corn required:
   $$\text{quarterly\_consumption} = \text{num\_chick} \times \text{CORN\_CONSUMPTION\_RATE}$$
2. **Corn Surplus / Deficit:**
   * **Deficit ($\text{area\_growing\_corn} < \text{quarterly\_consumption}$):** The farm must buy extra feed:
     $$\text{extra\_corn\_feed} = \text{quarterly\_consumption} - \text{area\_growing\_corn}$$
     $$\text{corn\_for\_sale} = 0$$
   * **Surplus ($\text{area\_growing\_corn} \ge \text{quarterly\_consumption}$):** Remaining corn can be sold:
     $$\text{extra\_corn\_feed} = 0$$
     $$\text{corn\_for\_sale} = \text{area\_growing\_corn} - \text{quarterly\_consumption}$$

### Task 2: Chicken Population & Sales
1. **Population Growth:** Update chicken count after growth during the quarter (e.g., doubling or increasing based on lab rules).
2. **Capacity Check:** Check if `num_chick` exceeds `total_capacity`.
3. **Chicken Sales:** If over capacity:
   $$\text{num\_chick\_sale} = \lfloor \text{num\_chick} \times \text{chicken\_sale\_portion} \rfloor$$
   $$\text{num\_chick} = \text{num\_chick} - \text{num\_chick\_sale}$$

### Task 3: Financial Tracking & Henhouse Expansion
1. **Quarterly Revenue:** Calculate total earnings from sold corn and chickens:
   $$\text{quarterly\_revenue} = (\text{corn\_for\_sale} \times \text{CORN\_PRICE}) + (\text{num\_chick\_sale} \times \text{CHICKEN\_PRICE})$$
2. **Quarterly Expenditure:** Calculate total spending on extra corn feed:
   $$\text{quarterly\_expenditure} = \text{extra\_corn\_feed} \times \text{CORN\_PRICE}$$
3. **Balance Update:** Update current balance:
   $$\text{balance} = \text{balance} + \text{quarterly\_revenue} - \text{quarterly\_expenditure}$$
4. **Buy Henhouse:** If `balance >= HENHOUSE_PRICE`, purchase a new henhouse:
   * Deduct `HENHOUSE_PRICE` from `balance`.
   * Add `HENHOUSE_PRICE` to `quarterly\_expenditure`.
   * Increase `total_capacity` by `henhouse_capacity`.
   * Set `buy_henhouse = true` (or `1`).

---

## Program Output Format

The output prints a formatted tabular view with columns formatted using `<iomanip>` (`setw`, `left`, `fixed`, `setprecision`):

| Header | Description |
| :--- | :--- |
| `Q` | Quarter number ($1$ to $10$) |
| `Cons` | Corn consumed (acres) |
| `CornBuy` | Extra corn bought (acres) |
| `CornSell` | Surplus corn sold (acres) |
| `ChickSell` | Number of chickens sold |
| `ChickLeft` | Remaining chicken count |
| `BuyHouse` | Whether a henhouse was bought ($1$/$0$ or `true`/`false`) |
| `Cap` | Total henhouse capacity |
| `Rev` | Total revenue for the quarter ($) |
| `Exp` | Total expenditure for the quarter ($) |
| `Bal` | Cumulative balance at end of quarter ($) |

---

## How to Compile & Run

### Prerequisites
* GCC C++ compiler (`g++`) or any C++11 compliant compiler.

### Steps
1. **Compile the code:**
   ```bash
   g++ -std=c++11 lab2.cpp -o lab2
   ```

2. **Execute the program:**
   ```bash
   ./lab2
   ```

3. **Sample Input Sequence:**
   ```text
   Please enter the number of chickens: 50
   Please enter the area growing corn: 10
   Please enter the capacity of a henhouse: 100
   Please enter the portion of chickens for sale: 0.6
   ```