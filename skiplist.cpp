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
bool programOn = true;


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
    void del(int id);
    Node* find(int id, int opt);
    void range(int id1, int id2, int opt);
    int findComp(int id);
    Node* findAtLevel(int id, int level);

    bool isEmpty();
    void print();
    void getRecord(Node *ptr);
};

SkipList::~SkipList() {
	cout<<"Deconstruct"<<endl;
    //Initialise a holder node
    Node *n;
    Node *prev = NULL;
    // Iterate over the linked list
    // and delete all nodes.
    for (n=header; n!=NULL; n=&(n->forwarding[0])) {
    	if (prev) {
    		delete prev->record;

    	}
    	prev = n;
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
    	cout << "Level: " << current->level << " Val: " << current->data << " Name: "<< current->record-> fName << " | ";
    	for (int i = 0; i < current->level; i++) {

    		cout << (current->forwarding)[i].data << " ";
    	}
    	cout << "| "<<endl;
    	current = &(current->forwarding[0]);
    }
    cout << endl;
}

void SkipList::getRecord(Node *ptr) {
    cout << "Id: " << ptr->data << " First Name: " << ptr->record->fName << " Last Name: " << ptr->record->lName << " Age: "; 
    cout << ptr->record->age << " Year: " << ptr->record->year << " Gpa: " << ptr->record->gpa << " Number of courses: " << ptr->record->numOfCourses<<endl;
}

Node* SkipList::find(int id, int opt = 0) {
	int level = maxLevel;
    Node *current = header;
    Node *next = &(current->forwarding[level]);

    while (next->data != id) {
	    if(next->data > id) {
	    	if (level != 0){
	    		level --;
	    		next = &(current->forwarding[level]);
	    	} else {
	    		// cout << "did not find an element, next one is: "<<next->data<< endl;
	    		if (opt == 1) {
	    			// if searching to insert return the previous one
	    			return current;
	    		} else {

	    			return nullptr;
	    		}
	    	}
	    } else if (next->data < id) {
	    	// cout << next->data << " is smaller than " << id << ". Level is: "<< level<< endl;
    		current = next;
    		// cout << "current "<< current->data<<endl;
    		next = &(current->forwarding[level]);
    		// cout << "next "<< next->data<<endl;
	    }
	}
	// cout << "found an element: "<< next->data << " at level "<< level<< endl;
	if (opt == 1) {
		// if searching to insert return the previous one
		return current;
	} 
	return next;
}

int SkipList::findComp(int id) {
	int level = maxLevel;
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
	    		// cout << "did not find an element, next one is: "<<next->data<< endl;
	    		return comp;
	    	}
	    } else if (next->data < id) {
	    	comp++;
	    	// cout << next->data << " is smaller than " << id << ". Level is: "<< level<< endl;
    		current = next;
    		// cout << "current "<< current->data<<endl;
    		next = &(current->forwarding[level]);
    		// cout << "next "<< next->data<<endl;
	    }
	}
	// cout << "found an element: "<< next->data << " at level "<< level<< endl;
	return comp;
}

void SkipList::range(int id1, int id2, int opt = 0) {
	int level = maxLevel;
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

	if (opt == 0){
		while (start->data >= id1 && start->data <= id2) {
			getRecord(start);
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
		av = total/count;
		cout <<av<<endl;
	}
	
}

Node* SkipList::findAtLevel(int id, int level) {
    Node *current = header;
    Node *next = &current->forwarding[level];
    // cout << "In findPrevLevel current data: " << next->data << endl;
    while (next->data < id) {
    	// cout << next->data << " is smaller than " << id << ". Level is: "<< level<< endl;
		current = next;
		next = &current->forwarding[level];
	}
	// cout << "found a prev: " << current->data << endl;
	return current;
}

void SkipList::insert(int id, string fName, string lName, int age, int year, double gpa, int numOfCourses) {

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

    Node* prev = find(id, 1);
    cout << "Find in insert, prev node is: "<<prev-> data <<". ";

    // newNode forwarding set to the next
    for (int i = 0; i < level; i++) {
    	if (prev->level > i) {
    	} else {
    		prev = findAtLevel(id, i);
    	}
    	newNode->forwarding[i] = prev->forwarding[i];
		(prev->forwarding)[i] = *newNode;
   	}

    cout << "Level of current insertion: " << level << endl;
}

void SkipList::del(int id) {
    Node* delNode = find(id);
    if(delNode == NULL) {
    	// cout << "This node doesn't exist "<<delNode-> data <<". "<< endl;
    	exit (EXIT_FAILURE);
    }
    // newNode forwarding set to the next
    int level = delNode->level;
    for (int i = 0; i < level; i++) {
    	cout << "DelNode Data"<<delNode->data<<endl;
    	
		Node *prev = findAtLevel(id, i);
		// cout << "Prev at level " << i << " with value " << prev->data << " ";
		// cout << "Deleting forwarding at level " << i << " with value " << delNode->forwarding[i].data<< " ";
		// cout << "Prev forwarding at level " << i << " with value " << prev->forwarding[i].data<< endl;

		(prev->forwarding)[i] = delNode->forwarding[i];
   	}

   	// delete[] delNode->forwarding;
   	// delete delNode->record;
   	// TODO: delete record and 
   	// Perche sef fault ?
   	// delete delNode;
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

void choose(string command, SkipList S) {
	switch(resolveOption(command)) {
			case 0: {

				cout << "------Insert---------"<<endl;
				
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

				cout <<  fName << " "<< lName << " "<< age <<" "<< year<<" "<< gpa <<" "<< numOfCourses <<" "<< endl;

				S.insert(id, fName, lName, age, year, gpa, numOfCourses);
				S.print();
				break; 
			}
			case 1: {
				cout << "-------find------"<<endl;
				int id;
				cin >> id;
				string line;
				getline(std::cin, line);
				Node* node = S.find(id);
				S.getRecord(node);
				break; 
			}
			case 2: {
				cout << "sfind";
				int id;
				cin >> id;
				string line;
				getline(std::cin, line);
				int comparisons = S.findComp(id);
				cout << "Number of comparisons: " << comparisons << endl;
				break; 
			}
			case 3: {
				cout << "range";
				int id1;
				int id2;
				cin >> id1;
				cin >> id2;
				S.range(id1, id2);
				// string line;
				// getline(std::cin, line);
				break;
			} 
			case 4: {
				int temp;
				cout << "gpa "<<endl;

				vector<int> v;
				std::string line;
				getline(std::cin, line);
				istringstream iss(line);
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
					cout << "The average gpa in the range " << v[0] << " to "<< v[1]<< " is ";
					S.range(v[0], v[1], 1);
				}
				break; 
			}
			case 5: {
				int id;
				cin >> id;
				S.del(id);
				break; 
			}
			case 6: {
				S.print();
				break; 
			}
			case 7: {
				string filename;
				cin >> filename;

				string line;
				ifstream infile;
				infile.open(filename);
		        while(!infile.eof()) // To get you all the lines.
		        {
		        	string command;
			        getline(infile,line); // Saves the line in STRING.
			        istringstream iss(line);
			        iss >> command;
			        cout<<line<<endl; // Prints our STRING.
			        choose(command, S, iss);
		        }

				infile.close();

				
				getline(std::cin, line);
				break; 
			}
			case 8: {
				programOn = false;
				// implement deconstructor
				cout << "exit";
				break; 
			}
			case 9: {

				cout << "Option is invalid. Please input a valid option";
				cin >> command;
				break; 
			}
		}
}

int main(int argc, char* argv[]) 
{
	maxLevel =  atoi(argv[1]);
	maxValue =  atoi(argv[2]);
	srand(time(0));

	cout << "MaxLevel: " <<  maxLevel << " MaxValue: " << maxValue << endl;

	SkipList S;
	cout << "---------INSERTING 3-------------" << endl;
	S.insert(3, "yana", "chala", 18, 2016, 3.9, 3);
	// S.print();
	cout << "---------INSERTING 5-------------" << endl;
	S.insert(5, "hy", "chala", 18, 2016, 3.9, 3);
	// S.print();
	cout << "---------INSERTING 4-------------" << endl;
	S.insert(4, "ji", "chala", 18, 2016, 3.9, 3);
	// S.print();
	cout << "---------INSERTING 8-------------" << endl;
	S.insert(8, "po", "chala", 18, 2016, 3.9, 3);
	// S.print();
	cout << "---------INSERTING 6-------------" << endl;
	S.insert(6, "yy", "chala", 18, 2016, 3.9, 3);


	Options resolveOption(string command);
	bool file = false;
	string command;


	while (programOn) {
		cout<<"Input a command please"<<endl;
		cin >> command;
		string line;
		getline(std::cin, line);
		istringstream iss(line);
		choose(command, S, iss);
	}
		
    
    return 0;
	
}




	// maxLevel =  atoi(argv[1]);
	// maxValue =  atoi(argv[2]);
	// cout << "MaxLevel: " <<  maxLevel << " MaxValue: " << maxValue << endl;

	// SkipList S = SkipList();
	// S.print();
	// cout << "---------INSERTING 3-------------" << endl;
	// S.insert(3, "yana", "chala", 18, 2016, 3.9, 3);
	// S.print();
	// cout << "---------INSERTING 5-------------" << endl;
	// S.insert(5, "hy", "chala", 18, 2016, 3.9, 3);
	// S.print();
	// cout << "---------INSERTING 4-------------" << endl;
	// S.insert(4, "ji", "chala", 18, 2016, 3.9, 3);
	// S.print();
	// cout << "---------INSERTING 8-------------" << endl;
	// S.insert(8, "po", "chala", 18, 2016, 3.9, 3);
	// S.print();
	// cout << "---------INSERTING 6-------------" << endl;
	// S.insert(6, "yy", "chala", 18, 2016, 3.9, 3);
	// S.print();
	// cout << "---------DELETING 6-------------" << endl;
	// S.del(6);
	// S.print();
	// cout << "---------DELETING 4-------------" << endl;
	// S.del(4);
	// S.print();
	// Node * val = S.find(3);
	// cout << "Return value from find " << val->data << endl;

