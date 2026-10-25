# Lab 7: Potion Brewery Inventory System

## Overview

`Lab 7` implements a interactive command-line **Potion Brewery Inventory System** in C++. The program allows a potion shop manager to dynamically initialize an inventory, manage stock items, restock products, track item values, and remove the most expensive item currently stored.

---

## Data Structures & Architecture

The system relies on header specifications defined in `lab7.h`, managing dynamic memory allocation for inventory management:

* **Item**: Struct representing individual inventory products:
  * `name`: C-string buffer storing item name (`MAX_NAME_LENGTH`).
  * `quantity`: Integer representing current stock quantity.
  * `price`: Double value representing unit cost in gold coins.
  * `weight`: Double value representing weight per unit (kg).
* **Inventory**: Struct containing store state and items container:
  * `shopName`: C-string storing the shop name.
  * `items`: Dynamically allocated array of `Item` structures.
  * `itemCount`: Current number of unique item types stored.
  * `maxItems`: Capacity limit for the `items` array.

---

## User Menu & Operations

Upon startup, the system prompts the user to enter the **Shop Name** and **Maximum Inventory Size** to initialize the `Inventory`. The interactive loop provides the following operations:

| Menu Option | Function | Description |
| :--- | :--- | :--- |
| **`1`** | `addItem(...)` | Adds a new `Item` with specified name, quantity, price, and weight to the inventory. Checks against maximum capacity. |
| **`2`** | `removeMostExpensive(...)` | Identifies and removes the item with the highest unit price from the inventory list, shifting remaining items to fill the gap. |
| **`3`** | `restockItem(...)` | Searches for an existing item by name and adds the given quantity to its current stock count. |
| **`4`** | `displayInventory(...)` | Prints formatted details of the shop, including total items, current capacity, item details, total weight, and total shop value. |
| **`0`** | **Quit** | Terminates the loop and exits the program safely. |

---

## Function Prototypes & Interface (`lab7.h`)

Below are the core functions required for implementation in `lab7.cpp`:

```cpp
// Initialize the inventory dynamic array and shop configuration
void initInventory(Inventory* inv, const char* shopName, int maxItems);

// Add a new item to the inventory if capacity allows
void addItem(Inventory* inv, const char* name, int quantity, double price, double weight);

// Remove the item with the highest unit price
void removeMostExpensive(Inventory* inv);

// Increase quantity of an existing item matching the given name
void restockItem(Inventory* inv, const char* name, int additionalQuantity);

// Display inventory details and summary statistics
void displayInventory(const Inventory* inv);
```

---

## Compilation & Execution

### Compilation

Compile `main_2.cpp` along with your `lab7.cpp` implementation using `g++`:

```bash
g++ -std=c++11 main_2.cpp lab7.cpp -o potion_shop
```

### Execution

Run the compiled executable:

```bash
./potion_shop
```

---

## Example Interaction

```text
Welcome to the Potion Brewery Inventory System!
Initializing...
Enter shop name: Alchemist Haven
Enter max inventory size: 5

======================================
0. Quit
1. Add an item
2. Remove most expensive item
3. Restock an item
4. Display inventory
======================================
Option: 1
Enter item name: Health Potion
Enter quantity: 20
Enter price (gold): 15.5
Enter weight (kg): 0.5
```