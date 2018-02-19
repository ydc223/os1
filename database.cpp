#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "skiplist_impl.h"

using namespace std;

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
				Node* node = S.find(id, 0);
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
				S.range(id1, id2, 0);
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
					Node* n = S.find(v[0], 0);
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
	int maxLevel =  atoi(argv[1]);
	int maxValue =  atoi(argv[2]);

	srand(time(0));

	SkipList S = SkipList(maxLevel, maxValue);

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
