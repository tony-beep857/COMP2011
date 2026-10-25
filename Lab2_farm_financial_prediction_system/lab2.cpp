#include <iostream>
#include <iomanip>
using namespace std;

int main() {
  
  //below are the variables taking inputs, please keep the names for convenience
  int num_chick;
  double area_growing_corn;
  int henhouse_capacity; // maximum number of chickens in a henhouse
  double chicken_sale_portion; // portion of chickens for sale 
  double balance = 0;

  //below are the constants, please use them as a reference for calculations
  //DO NOT MODIFY THE VALUE
  const double CORN_CONSUMPTION_RATE = 0.1; // rate of corn consumption by chickens
  const int QUARTERS = 10; // total number of quarters for prediction
  const int HENHOUSE_PRICE = 1000; // the price of a henhouse
  const int CHICKEN_PRICE = 100; // the price of a chicken
  const int CORN_PRICE = 100; //the price of an acre of corn

  //Greeting
  cout << "This system helps you predict the financial condition of the farm in the following 10 quarters~" << endl;

  // Input
  /* For simplicity, we skip sanitation check for inputs */
  cout << "Please enter the number of chickens: ";
  cin >> num_chick; // assumed to be a positive integer
  cout << num_chick << endl;
  cout << "Please enter the area growing corn: ";
  cin >> area_growing_corn; // assumed to be positive
  cout << area_growing_corn << endl;
  cout << "Please enter the capacity of a henhouse: ";
  cin >> henhouse_capacity; // assumed to be a positive integer, and bigger than number of chickens
  cout << henhouse_capacity << endl;
  cout << "Please enter the portion of chickens for sale when their number exceeds the henhouse capacity(e.g., for 60%, please enter 0.6): ";
  cin >> chicken_sale_portion; // assumed to be a number among {0.6, 0.7, 0.8, 0.9}
  while ((chicken_sale_portion != 0.6) && (chicken_sale_portion != 0.7) && (chicken_sale_portion != 0.8) && (chicken_sale_portion != 0.9)){
    cout << "Please enter numbers among 0.6, 0.7, 0.8, 0.9: " << endl;
    cin >> chicken_sale_portion;
  }
  cout << chicken_sale_portion << endl;


   /* make and display your prediction
   * Loop for each quarter
   // Task 1: calculation on corns
   *   1) Calculate the amount of corn consumed by chickens, determine whether grandma needs to buy more corn
   *   2) Calculate the remaining corn for sale
   *   3) Calculate the revenue and expenditure, update the balance
   // Task 2: calculation on chickens
   *   4) Calculate the number of chickens after this quarter
   *   5) Determine whether number of chickens exceeds the capacity of henhouses
   *   6) If the number of chickens exceeds the capacity, sell a portion of chickens
   *   7) Calculate the revenue of selling chickens and 
   // Task 3: buy a new henhous
   *   8) If current balance allows, buy one more henhouse with the same capacity 
   *   9) Calculate the expenditure and update the balance
   */
  // you may find the following variable useful, you may define more variables for your convenience
  double quarterly_consumption;
  double corn_for_sale;
  double extra_corn_feed = 0;
  int num_chick_sale;
  double quarterly_revenue;
  double quarterly_expenditure;
  int total_capacity = henhouse_capacity;
  bool buy_henhouse = false;
  cout << left
     << setw(3)  << "Q"
     << setw(7)  << "Cons"
     << setw(10) << "CornBuy"
     << setw(10) << "CornSell"
     << setw(11) << "ChickSell"
     << setw(12) << "ChickLeft"
     << setw(10) << "BuyHouse"
     << setw(6)  << "Cap"
     << setw(10)  << "Rev"
     << setw(10)  << "Exp"
     << setw(10)  << "Bal"
     << endl;

  for (int q = 1; q <= QUARTERS; q++) {
    /* Your code starts here */



    /* Your code ends here */
    // Output
    
    cout << left
     << setw(3)  << q
     << setw(7)  << fixed << setprecision(1) << quarterly_consumption
     << setw(10) << extra_corn_feed
     << setw(10) << corn_for_sale
     << setw(11) << num_chick_sale
     << setw(12) << num_chick
     << setw(10) << buy_henhouse
     << setw(6)  << total_capacity
     << setw(10)  << quarterly_revenue
     << setw(10)  << quarterly_expenditure
     << setw(10)  << balance
     << endl;
  }
  return 0;
}