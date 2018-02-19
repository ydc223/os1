all:
	g++ -g database.cpp skiplist_impl.cpp -o database && ./database 5 99999

