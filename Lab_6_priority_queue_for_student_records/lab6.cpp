#include "lab6.h"
#include <cstring>

// Given functions; Please do not edit them
void swapRecord(StudentRecord &record1, StudentRecord &record2) {
    StudentRecord temp = record1;
    record1 = record2;
    record2 = temp;
}

bool isEmpty(const RecordHolder &recordHolder) {
    return !recordHolder.num_records;
}

int compareTo(const Date &date1, const Date &date2) {
    if (date1.year != date2.year) return static_cast<int>(date1.year) - static_cast<int>(date2.year);
    if (date1.month != date2.month) return static_cast<int>(date1.month) - static_cast<int>(date2.month);
    return static_cast<int>(date1.day) - static_cast<int>(date2.day);
}

int compareTo(const StudentRecord &student1, const StudentRecord &student2) {
    int val = compareTo(student1.entry, student2.entry);
    return val ? val : strcmp(student1.itso, student2.itso);
}

const StudentRecord& getEarliestRecord(const RecordHolder &recordHolder) {
    return recordHolder.records[0];
}

bool insertRecord(RecordHolder &recordHolder, StudentRecord &record) {
    if (recordHolder.num_records == MAX_RECORD_LENGTH) return false;

    recordHolder.records[recordHolder.num_records] = record;
    pushUp(recordHolder, static_cast<int>(recordHolder.num_records));
    return ++recordHolder.num_records;
}

void pushUp(RecordHolder &recordHolder, int index) {
    // TODO: Task 1
    //base case
    if (index <= 0) return;

    //calculate parent index
    int parentIndex = (index -1) / 2;

    //if child is left
    if (compareTo(recordHolder.records[index], recordHolder.records[parentIndex]) < 0) //child is earlier than parent
    {
            swapRecord(recordHolder.records[index], recordHolder.records[parentIndex]);
            pushUp(recordHolder, parentIndex);
    }

}

void pushDown(RecordHolder &recordHolder, int index) {
    // TODO: Task 2
    
    int leftChildIndex = 2 * index + 1, rightChildIndex = 2 * index + 2;
    if (rightChildIndex >= recordHolder.num_records &&  leftChildIndex < recordHolder.num_records) // right child out of bound while left child not out of bound 
    {
        if (compareTo(recordHolder.records[index], recordHolder.records[leftChildIndex]) > 0)  // copare to left child, parent is later than child
            swapRecord(recordHolder.records[index], recordHolder.records[leftChildIndex]);
        return;
    }
    else if (leftChildIndex >= recordHolder.num_records &&  rightChildIndex >= recordHolder.num_records) // both child out of bound
        return;
    else
    {   
        int leftDiff = compareTo(recordHolder.records[index], recordHolder.records[leftChildIndex]);
        int rightDiff = compareTo(recordHolder.records[index], recordHolder.records[rightChildIndex]);
        if (leftDiff <= 0 && rightDiff <= 0) return;
        else if (leftDiff > 0 && rightDiff <= 0) //parent later than left child but earlier than right child
        {
            swapRecord(recordHolder.records[index], recordHolder.records[leftChildIndex]);
            pushDown(recordHolder, leftChildIndex);
        }
        
        else if (rightDiff > 0 && leftDiff <= 0)// parent earlier than left child but later than right child
        {
            swapRecord(recordHolder.records[index], recordHolder.records[rightChildIndex]);
            pushDown(recordHolder, rightChildIndex);
        }

        else if (compareTo(recordHolder.records[leftChildIndex], recordHolder.records[rightChildIndex]) < 0) //  parent later than both child, choose the earlier one
        {
            swapRecord(recordHolder.records[index], recordHolder.records[leftChildIndex]);
            pushDown(recordHolder, leftChildIndex);
        }

        else// right is earlier
        {
            swapRecord(recordHolder.records[index], recordHolder.records[rightChildIndex]);
            pushDown(recordHolder, rightChildIndex);
        }

    }


}

StudentRecord& popEarliestRecord(RecordHolder &recordHolder) {
    // TODO: Task 3
    swapRecord(recordHolder.records[0], recordHolder.records[recordHolder.num_records-1]);
    recordHolder.num_records -= 1;
    pushDown(recordHolder, 0);
    return recordHolder.records[recordHolder.num_records];
}
