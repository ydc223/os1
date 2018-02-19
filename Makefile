all:
	g++ -g database.cpp skiplist_impl.cpp -o database -std=c++11 && ./database 5 99999

