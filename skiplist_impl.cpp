#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "skiplist_impl.h"

using namespace std;

SkipList::~SkipList() {
    Node *n;
    int numNodes=0;

    //Count the number of nodes
    for (n=header; n!=NULL; n=&(n->forwarding[0])) {
    	numNodes++;
    }

	// Iterate over the linked list backwards and delete all nodes.
    for(int j = numNodes; j < 0; j--){
    	n=header;
    	for (int i = 0; i < numNodes; i++) {
	    	n=&(n->forwarding[0]);
    	}
    	delete[] n->forwarding;
    	delete n->record;
    	delete n;
    }
}

// Constructor
SkipList::SkipList(int maxLevel, int maxValue) {
    // Setup null pointer
    this->maxLevel = maxLevel;
    this->maxValue = maxValue;
    Node *topElement = new Node;
    topElement->data = 0;
    topElement->forwarding = new Node[maxLevel];
    topElement->level = maxLevel;
    
    Node *lastElement = new Node;
    lastElement->data = maxValue;
    lastElement->forwarding = NULL;
    lastElement->level = 1;

    for (int i = 0; i < maxLevel; i++) {
    	topElement->forwarding[i] = *lastElement;
    }

    header = topElement;
    footer = lastElement;
}

void SkipList::print() {
    cout << "________Printing the skiplist: ________"<< endl ;

    Node *current = &header->forwarding[0];
    while (current->data != maxValue) {
    	cout << "Level: " << current->level << " Val: " << current->data << " Name: "<< current->record-> fName << " Forwarding: | ";
    	for (int i = 0; i < current->level; i++) {

    		cout << (current->forwarding)[i].data << " ";
    	}
    	cout << "| "<<endl;
    	current = &(current->forwarding[0]);
    }
    cout << endl;
}

void SkipList::printRecord(Node *ptr) {
    cout << "Id: " << ptr->data << " First Name: " << ptr->record->fName << " Last Name: " << ptr->record->lName << " Age: "; 
    cout << ptr->record->age << " Year: " << ptr->record->year << " Gpa: " << ptr->record->gpa << " Number of courses: " << ptr->record->numOfCourses<<endl;
}

//I am not following the exact 
Node* SkipList::find(int id, int returnPrev = 0) {
	int level = maxLevel-1;
    Node *current = header;
    Node *next = &(current->forwarding[level]);

    while (next->data != id) {
	    if(next->data > id) {
	    	if (level != 0){
	    		level --;
	    		next = &(current->forwarding[level]);
	    	} else {
	    		if (returnPrev == 1) {
	    			// if searching from the insert function return the previous one
	    			return current;
	    		} else {
	    			return nullptr;
	    		}
	    	}
	    } else if (next->data < id) {
    		current = next;
    		next = &(current->forwarding[level]);
	    }
	}
	return next;
}

// Same as the find function, but instead returns the number of comparisons
// I am not 100% sure what you call comparisons exactly, so I inceremnt comp after all comparison if statements
int SkipList::findComp(int id) {
	int level = maxLevel-1;
	int comp = 0;
    Node *current = header;
    Node *next = &(current->forwarding[level]);
    while (next->data != id) {
    	comp++;

	    if(next->data > id) {
	    	comp++;

	    	if (level != 0){
	    		comp++;
	    		level --;
	    		next = &(current->forwarding[level]);
	    	} else {
	    		return comp;
	    	}
	    } else if (next->data < id) {
	    	comp++;
    		current = next;
    		next = &(current->forwarding[level]);
	    }
	}
	return comp;
}

// This function addresses both range and gpa functionalities
// Depending on the value of gpa the outputs vary
// it return the average gpa in range if gpa = 1
float SkipList::range(int id1, int id2, int gpa = 0) {
	int level = maxLevel - 1;
    Node *current = header;
    Node *next = &(current->forwarding[level]);
    Node *start;
    while (next->data != id1) {
	    if(next->data > id1) {
	    	if (level != 0){
	    		level --;
	    		next = &(current->forwarding[level]);
	    	} else {
	    		if(next->data < id2)
	    		start = next;
	    		break;
	    	}
	    } else if (next->data < id1) {

    		current = next;
    		next = &(current->forwarding[level]);
	    }
	}
	
	start = next;
	if (gpa == 0){
		while (start->data >= id1 && start->data <= id2) {
			printRecord(start);
			start = &start->forwarding[0];
		}
	} else {
		float total = 0;
		float count = 0;
		float av;
		while (start->data >= id1 && start->data <= id2) {
			total += start->record->gpa;
			count++;
			start = &start->forwarding[0];
		}
		return av = total/count;
	}
	return 0;
}

//Finds a previous node at the specified level
Node* SkipList::findAtLevel(int id, int level) {
    Node *current = header;
    Node *next = &current->forwarding[level];
    while (next->data < id) {
		current = next;
		next = &current->forwarding[level];
	}
	return current;
}

//Update the info in the record
void SkipList::updateNode(Node* node, string fName, string lName, int age, int year, double gpa, int numOfCourses) {
	node->record->fName = fName;
    node->record->lName = lName;
    node->record->age = age;
    node->record->year = year;
    node->record->gpa = gpa;
    node->record->numOfCourses = numOfCourses;
}

//Inserts a value
void SkipList::insert(int id, string fName, string lName, int age, int year, double gpa, int numOfCourses) {

    Node* prev = find(id, 1);

    //If the node exists, then update it
    if(prev->data == id) {
    	updateNode(prev, fName, lName, age, year, gpa, numOfCourses);
    } else {
    	// Else, insert a new node
	    int level = (rand() % maxLevel);
	    Node * newNode = new Node;
	    newNode->data = id;
	    newNode->forwarding = new Node[level];
	    newNode->level = level;
	    newNode->record = new Record;

	    newNode->record->fName = fName;
	    newNode->record->lName = lName;
	    newNode->record->age = age;
	    newNode->record->year = year;
	    newNode->record->gpa = gpa;
	    newNode->record->numOfCourses = numOfCourses;

	    // newNode forwarding set to the next
	    for (int i = 0; i < level; i++) {
	    	if (prev->level > i) {
	    	} else {
	    		prev = findAtLevel(id, i);
	    	}
	    	newNode->forwarding[i] = prev->forwarding[i];
			(prev->forwarding)[i] = *newNode;
	   	}
    }
}

void SkipList::del(int id) {
    Node* delNode = find(id);
    if(delNode == NULL) {
    	cout << "The student with id "<<id <<" doesn't exist."<< endl;
    } else {
    	// newNode forwarding set to the next
	    int level = delNode->level;
	    for (int i = 0; i < level; i++) {
			Node *prev = findAtLevel(id, i);
			(prev->forwarding)[i] = delNode->forwarding[i];
	   	}
    }
}
