#include <iostream>
#include "lab6.h"

using namespace std;

int main() 
{
    /*RecordHolder h;
    int n;
    cout << "Please enter the number of operations: ";
    cin >> n;
    cout << n << endl;

    for (int i = 0; i < n; i++) 
    {
        char op;
        cout << "Please enter operation (I/P/G/E): ";
        cin >> op;
        cout << op << endl;
        
        if (op == 'I')
        {
            StudentRecord r = createStudentRecord();
            bool success = insertRecord(h, r);
            if (success) {
                cout << "Insert successful!" << endl;
            } else {
                cout << "Insert failed (full)." << endl;
            }
            cout << sep;
        
        } 
    }*/
   Date date1;
   date1.day = 5;
   date1.month =3;
   date1.year = 2024;

   Date date2;
   date2.day = 2;
   date2.month = 1;
   date2.year = 2025;
   // date1 is eariler than date2
    Date date3;
    date2.day = 2;
    date2.month = 1;
    date2.year = 2023;


   int x = compareTo(date3, date2);
   cout << x << endl;
}