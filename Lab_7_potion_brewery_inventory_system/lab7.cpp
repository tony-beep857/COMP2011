#include "lab7.h"

// ============================================================
// COMP2011 Lab 7: Potion Brewery Inventory System
// Implementation of all functions (Tasks 1–6)
// ============================================================

// ============================================================
// Task 1 — Initialize an Item
// ============================================================

// Set all fields of the Item pointed to by 'item'.
// - item: pointer to the Item to initialize
// - name: C-string to copy into item->name (use strncpy, null-terminate)
// - quantity: number of units in stock
// - price: price per unit in gold
// - weight: weight per unit in kg
void initItem(Item* item, const char name[], int quantity, double price, double weight) {
    // TODO: Implement this function
    int i;
    for (i = 0; name[i] != '\0'; i++)
        item->name[i] = name[i];
    item->name[i] = '\0';
    item->quantity = quantity;
    item->price = price;
    item->weight = weight;
}

// ============================================================
// Task 2 — Display an Item
// ============================================================

// Print the item's information in this exact format:
//   "<name> (qty: X) - Price: Y.Y gold, Weight: Z.Z kg"
// - item: pointer to a const Item (read-only)
// - Use fixed << setprecision(1) for price and weight
void displayItem(const Item* item) {
    // TODO: Implement this function
    cout << fixed << setprecision(1);
    cout << item->name << " (qty: " << item->quantity << ") - Price: " << item->price << " gold, Weight: " << item->weight << " kg" << endl;
}
/*fixed << setprecision(1)*/

// ============================================================
// Task 3 — Initialize Inventory
// ============================================================

// Set up an empty inventory.
// - inv: pointer to the Inventory to initialize
// - shopName: C-string to copy into inv->shopName (use strncpy, null-terminate)
// - capacity: maximum number of distinct items this shop can hold
// - itemCount should start at 0
void initInventory(Inventory* inv, const char shopName[], int capacity) {
    // TODO: Implement this function
    int i;
    for (i = 0; shopName[i] != '\0'; i++)
        inv->shopName[i] = shopName[i];
    inv->shopName[i] = '\0';
    inv->itemCount = 0;
    inv->capacity = capacity;
}

// ============================================================
// Task 4 — Add an Item (Create)
// ============================================================

// Add a new item to the inventory. Return false if inventory is full.
// - If inv->itemCount >= inv->capacity, print "Inventory full! Cannot add <name>."
// - Otherwise, initialize the next slot in inv->items[] using initItem.
// - Insert the pointer to the new item into inv->itemPtrs[] so that
//   itemPtrs remains sorted by price descending.
// - Increment itemCount and print "<name> added successfully."
// - inv->items[]: stores the actual Item data in insertion order
// - inv->itemPtrs[]: stores pointers into items[], used later for sorting
bool addItem(Inventory* inv, const char name[], int quantity, double price, double weight) {
    // TODO: Implement this function
    if (inv->itemCount >= inv->capacity){
        cout << "Inventory full! Cannot add " << name << "." << endl;
        return false;
    }
    
    Item* newItemPointer = &inv->items[inv->itemCount];
    initItem(newItemPointer, name, quantity, price, weight);
    inv->itemPtrs[inv->itemCount] = newItemPointer;
    (inv->itemCount)++;
    cout << name << " added successfully." << endl;
    //rebuilt pointers array
    for (int i = 0; i < inv->itemCount; i++){
        inv->itemPtrs[i] = &inv->items[i];
    }
    for (int _ = 0; _ < inv->itemCount-1; _++){
        for (int i = 0; i+1 < inv->itemCount; i++){
            if ((inv->itemPtrs[i]->price) < (inv->itemPtrs[i+1]->price)){
                //switching
                Item* temp = inv->itemPtrs[i];
                inv->itemPtrs[i] = inv->itemPtrs[i+1];
                inv->itemPtrs[i+1] = temp;
                
            }
        }
    }
    return true;
}

// ============================================================
// Task 5 — Restock an Item (Update)
// ============================================================

// Search inv->items[] for an item matching 'name' (use strcmp).
// If found, add additionalQty to its quantity, print
//   "<name> restocked. New quantity: X", and return true.
// If not found, print "Item not found: <name>" and return false.
// - capacity limits the number of distinct items, NOT total quantity,
//   so restocking is always allowed.
bool restockItem(Inventory* inv, const char name[], int additionalQty) {
    // TODO: Implement this function
    for (int i = 0; i < inv->itemCount; i++){
        if (!strcmp(name, inv->items[i].name)){
            inv->items[i].quantity += additionalQty;
            cout << name << " restocked. New quantity: " << inv->items[i].quantity << endl;
            return true;
        }
    }

    cout << "Item not found: " << name;
    return false;
}

// ============================================================
// Task 6 — Remove the Most Expensive Item (by total value)
// ============================================================

// Remove the item with the highest total value (quantity * price).
// If inventory is empty, print "Inventory is empty. Nothing to remove."
//   and return false.
// Otherwise:
// - Find the index in items[] with the highest quantity * price
// - Print "Removed: <name> (Total value: X.X gold)"
// - Shift items[] left to fill the gap, decrement itemCount
// - Rebuild itemPtrs[] and keep it sorted by unit price descending
// - Return true
bool removeMostExpensive(Inventory* inv) {
    // TODO: Implement this function
    if (inv->itemCount == 0){
        cout << "Inventory is empty. Nothing to remove.";
        return false;
    }
    double highest = 0;
    int highestIndex = 0;

    for (int i = 0; i < inv->itemCount; i++){
        int quantityPrice = inv->items[i].quantity * inv->items[i].price;
        if (quantityPrice >= highest){
            highest = quantityPrice;
            highestIndex = i;
        }
    }
    cout << fixed << setprecision(1);
    cout << "Removed: " << inv->items[highestIndex].name << " (Total value: " << (inv->items[highestIndex].quantity* inv->items[highestIndex].price) << " gold)" << endl;
    
    for (int i = highestIndex; i + 1 < inv->itemCount; i++){//shift the item
        inv->items[i] = inv->items[i+1];
    }
    inv->itemCount-= 1;
    //rebuilt pointers array
    for (int i = 0; i < inv->itemCount; i++){
        inv->itemPtrs[i] = &inv->items[i];
    }
    for (int _ = 0; _ < inv->itemCount-1; _++){
        for (int i = 0; i+1 < inv->itemCount; i++){
            if ((inv->itemPtrs[i]->price) < (inv->itemPtrs[i+1]->price)){
                //switching
                Item* temp = inv->itemPtrs[i];
                inv->itemPtrs[i] = inv->itemPtrs[i+1];
                inv->itemPtrs[i+1] = temp;
                
            }
        }
    }
    return true;
}

// ============================================================
// Provided Function — Display Inventory (Sorted by Unit Price, Descending)
// ============================================================

// Display the full inventory using itemPtrs[] order (highest first).
// - Print "=== Inventory of <shopName> ==="
// - If empty, print "(empty)"; otherwise call displayItem for each
//   entry in itemPtrs[]
// - Compute total inventory value: sum of (quantity * price) for each item
// - Print "Total inventory value: X.X gold"
void displayInventory(Inventory* inv) {
    cout << "=== Inventory of " << inv->shopName << " ===" << endl;
    if (inv->itemCount == 0) {
        cout << "(empty)" << endl;
    } else {
        for (int i = 0; i < inv->itemCount; i++) {
            displayItem(inv->itemPtrs[i]);
            
            //cout << "quantity " << inv->itemPtrs[i]->quantity << "price " << inv->itemPtrs[i]->price << endl;
        }
       //displayItem(inv->itemPtrs[0]);
       //displayItem(inv->itemPtrs[1]);
    }
    double total = 0.0;
    for (int i = 0; i < inv->itemCount; i++) {
        total += inv->itemPtrs[i]->quantity * inv->itemPtrs[i]->price;
    }
    cout << "Total inventory value: " << fixed << setprecision(1) << total << " gold" << endl;
}
