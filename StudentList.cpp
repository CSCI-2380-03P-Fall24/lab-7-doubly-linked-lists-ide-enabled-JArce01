#include "StudentList.h"
#include <iostream>

// Define a constructor to initialize the list. The list starts with no Students
StudentList::StudentList() {
    head = nullptr;
    tail = nullptr;
    numStudents = 0;
}

// return the number of students currently in the list
int StudentList::listSize() {
    return numStudents;
}

//add a Node with a student to the front (head) of the list.
void StudentList::addFront(Student s) {
    Node* n = new Node{s};
    n->prev = nullptr;
    n->next = head;
    if (head) head->prev = n;
    else      tail = n;          // list was empty
    head = n;
    ++numStudents;
}

//add a Node with a student to the back (tail) of the list.
void StudentList::addBack(Student s) {
    Node* n = new Node{s};
    n->next = nullptr;
    n->prev = tail;
    if (tail) tail->next = n;
    else      head = n;          // list was empty
    tail = n;
    ++numStudents;
}

//Print out the names of each student in the list.
void StudentList::printList() {
    for (Node* cur = head; cur; cur = cur->next) {
        std::cout << cur->data.name;
        if (cur->next) std::cout << ", ";
    }
    std::cout << std::endl;
}

// Remove the Node with the student at the back (tail) of the list
// should not fail if list is empty! Print an error message if this occurs
// Don't forget that your head and tail pointers will be null pointers if the list is empty
void StudentList::popBack() {
    if (!tail) {
        std::cout << "List is empty. Nothing to pop from back." << std::endl;
        return;
    }
    Node* victim = tail;
    Node* left = tail->prev;
    if (left) left->next = nullptr;
    else      head = nullptr;    // list becomes empty
    tail = left;
    delete victim;
    --numStudents;
}

// Remove the Node with the student at the front (head) of the list
// should not fail if list is empty! Print an error message if this occurs
// Don't forget that your head and tail pointers will be null pointers if the list is empty
void StudentList::popFront() {
    if (!head) {
        std::cout << "List is empty. Nothing to pop from front." << std::endl;
        return;
    }
    Node* victim = head;
    Node* right = head->next;
    if (right) right->prev = nullptr;
    else       tail = nullptr;   // list becomes empty
    head = right;
    delete victim;
    --numStudents;
}

//insert a student at the position "index".
// for this list, count head as index 0
// if index is outside of current list range, 
// print a message and insert the student at the back of the list
// the previous Node at your target index should be moved forward. "For exampe, Node with student at index i, becomes index i+1" 
// Remember that you already have methods that can add students to the front or back of list if needed! Don't repeat this code.
void StudentList::insertStudent(Student s, int index) {
    if (index <= 0) { addFront(s); return; }
    if (index >= numStudents) {
        if (index != numStudents)
            std::cout << "Index out of range. Inserting at back." << std::endl;
        addBack(s);
        return;
    }

    // walk to the node currently at position index (0 < index < numStudents)
    Node* cur = head;
    for (int i = 0; i < index; ++i) cur = cur->next;

    Node* left = cur->prev;
    Node* n = new Node{s};

    // link: left <-> n <-> cur
    n->prev = left;
    n->next = cur;
    left->next = n;
    cur->prev = n;

    ++numStudents;
}

//find the student with the given id number and return them
// if no student matches, print a message 
// and create and return a dummy student object
Student StudentList::retrieveStudent(int idnumber) {
    for (Node* cur = head; cur; cur = cur->next) {
        if (cur->data.id == idnumber) return cur->data;
    }
    std::cout << "No student with id " << idnumber << " found." << std::endl;
    Student dummy; // default-constructed dummy
    return dummy;
}

// Remove a Node with a student from the list with a given id number
// If no student matches, print a message and do nothing
void StudentList::removeStudentById(int idnumber) {
    Node* cur = head;
    while (cur && cur->data.id != idnumber) cur = cur->next;
    if (!cur) {
        std::cout << "No student with id " << idnumber << " to remove." << std::endl;
        return;
    }
    Node* L = cur->prev;
    Node* R = cur->next;
    if (L) L->next = R; else head = R;
    if (R) R->prev = L; else tail = L;
    delete cur;
    --numStudents;
}

//Change the gpa of the student with given id number to newGPA
void StudentList::updateGPA(int idnumber, float newGPA) {
    for (Node* cur = head; cur; cur = cur->next) {
        if (cur->data.id == idnumber) {
            cur->data.GPA = newGPA;
            return;
        }
    }
    std::cout << "No student with id " << idnumber << " to update." << std::endl;
}

//Add all students from otherList to this list.
//otherlist should be empty after this operation.
/*
For example, if the list has 3 students:
s1 <-> s2 <-> s3
and otherList has 2 students
s4 <-> s5
then after mergeList the currently list should have all 5 students
s1 <-> s2 <-> s3 <-> s4 <-> s5
and otherList should be empty and have zero students.
*/
void StudentList::mergeList(StudentList &otherlist) {
    if (otherlist.numStudents == 0) return;

    if (numStudents == 0) {
        head = otherlist.head;
        tail = otherlist.tail;
        numStudents = otherlist.numStudents;
    } else {
        tail->next = otherlist.head;
        otherlist.head->prev = tail;
        tail = otherlist.tail;
        numStudents += otherlist.numStudents;
    }

    otherlist.head = otherlist.tail = nullptr;
    otherlist.numStudents = 0;
}

//create a StudentList of students whose gpa is at least minGPA.
//Return this list.  The original (current) list should
//not be modified (do not remove the students from the original list).
StudentList StudentList::honorRoll(float minGPA) {
    StudentList out;
    for (Node* cur = head; cur; cur = cur->next) {
        if (cur->data.GPA >= minGPA) out.addBack(cur->data);
    }
    return out;
}

//remove all students whose GPA is below a given threshold.
// For example, if threshold = 3.0, all students with GPA less than 3.0
// should be removed from the list. 
// Be sure to correctly update both head and tail pointers when removing 
// from the front or back, and adjust numStudents accordingly.
// If the list is empty, print a message and do nothing.
void StudentList::removeBelowGPA(float threshold) {
    if (!head) {
        std::cout << "List is empty. Nothing to remove." << std::endl;
        return;
    }
    Node* cur = head;
    while (cur) {
        Node* next = cur->next;
        if (cur->data.GPA < threshold) {
            Node* L = cur->prev;
            Node* R = cur->next;
            if (L) L->next = R; else head = R;
            if (R) R->prev = L; else tail = L;
            delete cur;
            --numStudents;
        }
        cur = next;
    }
}
