#ifndef SKIPLIST_H   /* Include guard */
#define SKIPLIST_H

using namespace std;

struct Node {
	int data;
	struct Record* record;
	Node* forwarding;
	int level;
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
    int maxLevel;
    int maxValue;
public:
    SkipList(int maxLevel, int maxValue);
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


#endif // FOO_H_
