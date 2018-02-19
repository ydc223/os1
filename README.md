# Operating System Programming Assignment 1

My source files include database.cpp and skiplist_impl.cpp. Database.cpp has the main logic of the program, while skiplist_impl contains the implementation of the skiplist functionality. The program requires the user to input two command line arguments. My main file contains the following command: 


g++ -g database.cpp skiplist_impl.cpp -o database -std=c++11 && ./database 5 99999, 

so that it automatically runs the program with the parameters 5 as maxLevel and 99999 as maxValue. 


The header file contains Node structure, Record structure and the Skiplist class. Each node has data, this data is the id that was inserted. The rest of the information is recorded in the Record. 


Skiplist_impl.cpp implements functionality such as insert, find, delete, etc. Some of the functions have optional arguments, such as range, which outputs vary depending on where the function is called. This is done to avoid code duplication. This might not be among the best practices, but it performs well for the specified functionality. 


My print function, prints some information about that record, such as id and name, it also prints the level of insertion and all the values of where the forwarding pointer point to.


My database.cpp gets the command from the user and executes it by the first word specified. All possible command are specified in the enum Options and they are resolved with the resolveOption function that returns the appropriate enum value based on the value of the input string. 
The menu is implemented using switch/case statements. Enum Options also contain an option for an Invalid option, in this case the program just goes back to the input and asks the user to input the command. The program finishes when exit is type, then choose[command] function returns 1 which is a signaling value that the program should end. 
