#include <iostream>
#include <fstream>
using namespace std;

const int MAX_PATH_LENGTH = 100; // maximum length of a file path

// Helper function for comparison
bool greater_or_equal(double a, double b) {
    return ((a > b) || (((a>b)? (a-b):(b-a)) < 1e-12));
}

bool degree_classification(double cga, double mcga, int &first_honor, int &second_honor_div1, int &second_honor_div2, int &third_honor, int &pass) {
    cout << "Student with CGA = " << cga << " and MCGA = " << mcga << " graduated with: ";
    // TODO: Task 1 - Implement Degree Classification Logic
    if (mcga >= 3.6 && cga >= 3.4){
        first_honor++;
        cout << "First Class Honors" << endl;
    } else if (mcga >= 2.85 && cga >= 2.7){
        second_honor_div1++;
        cout << "Second Class Honors, Division I" <<endl;
    } else if (mcga >= 2.15 && cga >= 2){
        second_honor_div2++;
        cout << "Second Class Honors, Division II" <<endl;
    } else if (mcga >= 1.7 && cga >= 1.7){
        third_honor++;
        cout << "Third Class Honors" << endl;
    } else if (mcga >= 0.85 && cga >= 0.85){
        pass++;
        cout << "Pass" << endl;
    } else{
        cout << "None" << endl;
        return false;
    }
    return true;

    // TODO: End of Task 1
}

void update_averages(bool graduated, double cga, double mcga, int &num_of_students, double &avg_cga, double &avg_mcga) {
    // TODO: Task 2 - Update number of graduates and average values
    if (graduated == true){
        avg_cga = (avg_cga * num_of_students + cga) / (num_of_students+1);
        avg_mcga = (avg_mcga * num_of_students + mcga) / (num_of_students+1);
        num_of_students++;

    }



    // TODO: End of Task 2
}

int main() {
    char file_path[MAX_PATH_LENGTH];
    cout << "Please enter the input file path: ";
    cin >> file_path;

    fstream fin(file_path);

    // Check if the file can be opened
    if (!fin) {
        cout << "Error: Can not open " << file_path << endl;
        return -1;
    }


    double cga, mcga;
    int num_of_students = 0;
    // Average values
    double avg_cga = 0, avg_mcga = 0;
    // Number of students in classification
    int first_honor = 0, second_honor_div1 = 0, second_honor_div2 = 0, third_honor = 0, pass = 0;

    // TODO: Task 3a: Read a line from the input file to the variables
    while (fin >> cga >> mcga) {

    // TODO: End of Task 3a
        bool graduated = degree_classification(cga, mcga, first_honor, second_honor_div1, second_honor_div2, third_honor, pass);
        update_averages(graduated, cga, mcga, num_of_students, avg_cga, avg_mcga);
    }

    // TODO: Task 3b: Close the file stream
    fin.close();

    // TODO: End of Task 3b

    cout << "Statistics:" << endl
    << "Number of graduates: " << num_of_students << endl
    << "Number of graduates with First Class Honors: " << first_honor << endl
    << "Number of graduates with Second Class Honors, Division I: " << second_honor_div1 << endl
    << "Number of graduates with Second Class Honors, Division II: " << second_honor_div2 << endl
    << "Number of graduates with Third Class Honors: " << third_honor  << endl
    << "Number of graduates with Pass: " << pass << endl
    << "--------------------------------------------------------------------" << endl
    << "Average graduate CGA: " << avg_cga << endl
    << "Average graduate MCGA: " << avg_mcga << endl;

    return 0;
}
