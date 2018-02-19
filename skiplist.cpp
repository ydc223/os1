#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

int maxLevel = 0;
int maxValue = 0;


struct Node {
	int data;
	struct Record* record;
	Node* forwarding;
	int level;
	~Node(){
	}
};

struct Record {
	string fName;
	string lName;
	int age;
	int year;
	double gpa;
	int numOfCourses;
};

class SkipList {
private:
    Node *header;
    Node *footer;
public:
    SkipList();
    ~SkipList();
    void insert(int id, string fName, string lName, int age, int year, double gpa, int numOfCourses);
    void updateNode(Node* node, string fName, string lName, int age, int year, double gpa, int numOfCourses);
    void del(int id);
    Node* find(int id, int opt);
    float range(int id1, int id2, int opt);
    int findComp(int id);
    Node* findAtLevel(int id, int level);

    bool isEmpty();
    void print();
    void printRecord(Node *ptr);
};

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
SkipList::SkipList() {
    // Setup null pointer
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

// Stack is empty function
bool SkipList::isEmpty(){
	for (int i = 0; i < maxLevel; i++) {
    	if((header->forwarding)[i].forwarding == footer){
    		return false;
    	}
    }
    return (true);
}

enum Options {ins, find_elem, sfind, range, gpa, del, print, load, exit_program, Option_Invalid /*others...*/};

enum Options resolveOption(string command) {
	    if( command == "ins" ) return ins;
	    if( command == "find" ) return find_elem;
	    if( command == "sfind" ) return sfind;
	    if( command == "range" ) return range;
	    if( command == "gpa" ) return gpa;
	    if( command == "del" ) return del;
	    if( command == "print" ) return print;
	    if( command == "load" ) return load;
	    if( command == "exit" ) return exit_program;
	    return Option_Invalid;
}

int choose(string command, SkipList S, string arguments) {
	istringstream iss(arguments);

	switch(resolveOption(command)) {
			// Insert
			case 0: {				
				int id;
				string fName;
				string lName;
				int age;
				int year;
				double gpa;
				int numOfCourses;
				
				iss >> (id);
				iss >> fName;
				iss >> lName;
				iss >> age;
				iss >> year;
				iss >> gpa;
				iss >> numOfCourses;

				// cout <<  fName << " "<< lName << " "<< age <<" "<< year<<" "<< gpa <<" "<< numOfCourses <<" "<< endl;

				S.insert(id, fName, lName, age, year, gpa, numOfCourses);
				// S.print();
				break; 
			}
			//Find
			case 1: {
				int id;
				iss >> id;
				Node* node = S.find(id);
				if (node != nullptr){
					S.printRecord(node);
				} else {
					cout << "The record does not exist"<<endl;
				}
				
				break; 
			}
			//Sfind
			case 2: {
				int id;
				iss >> id;
				int comparisons = S.findComp(id);
				cout << "Number of comparisons: " << comparisons << endl;
				break; 
			}
			//Range
			case 3: {
				int id1;
				int id2;
				iss >> id1;
				iss >> id2;
				S.range(id1, id2);
				break;
			} 
			//GPA and GPA range
			case 4: {
				int temp;

				vector<int> v;

				while (iss >> temp){
				    v.push_back(temp);
				}
				temp = 0;
				for (vector<int>::iterator it = v.begin(); it != v.end(); ++it){
    				temp++;
				}
				if (temp > 2) {
					cout << "Cannot find GPAs in range specified by more than 2"<<endl;
				}
				else if (temp == 1) {
					Node* n = S.find(v[0]);
					if(n != NULL) {
						cout << "GPA of " << v[0] << " is: "<< n->record->gpa;
					} else {
						cout << "Record with such id does not exist"<< endl;
					}
				} else {
					float averageGpa = S.range(v[0], v[1], 1);
					if(averageGpa!=averageGpa) {
						cout << "The average gpa in the range "<< v[0] << " to "<< v[1] << " does not exist."<<endl;
					} else {
						cout << "The average gpa in the range " << v[0] << " to "<< v[1] << " is " << averageGpa << endl;
					}
				}
				break; 
			}
			//Delete
			case 5: {
				int id;
				iss >> id;
				S.del(id);
				break; 
			}
			//Print
			case 6: {
				S.print();
				break; 
			}
			//Load
			case 7: {
				string filename;
				iss >> filename;

				string line;
				ifstream infile;
				infile.open(filename);

				if (infile.fail()) {
			      cerr << "Error opening file " << filename << endl;
			    } else {
			    	while( !infile.fail() && !infile.eof()) // To get you all the lines.
			        {
				        getline(infile,line); // Saves the line in STRING.
				        // cout << line << endl;
				        string fileCommand = line.substr(0, line.find(" "));
				        string substring2 = line.substr(line.find(" ") + 1);
				        // cout<<"Command: "<<fileCommand<<" Line: "<<line << "substr:"<< substring2<<endl; // Prints our STRING.
				        int checkIfOver = choose(fileCommand, S, substring2);
				        if(checkIfOver) {
				        	return 1;
				        }
			        }
			    }
				infile.close();
				break; 
			}
			//Exit
			case 8: {
				// deconstructor gets called autoatically upon the program completion
				return 1;
			}
			//Invalid command
			case 9: {
				cout << "Option is invalid."<<endl;
				break; 
			}
		}
		return 0;
}

int main(int argc, char* argv[]) 
{
	maxLevel =  atoi(argv[1]);
	maxValue =  atoi(argv[2]);
	srand(time(0));
	SkipList S;

	Options resolveOption(string command);
	bool file = false;
	string command;
	int isOver = 0;

	while(!isOver) {
		cout<<"Input a command, please"<<endl;
		cin >> command;
		string line;
		getline(std::cin, line);
		isOver = choose(command, S, line);
	}
    
    return 0;
}
