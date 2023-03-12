/*Context Switch Assignment  100pts
 * Using your knowledge of assembly, stack frames for functions
 * and the calling convention of the compiler, write code below in the
 * "//  ToDo" sections so that the program alternates between running
 * main1's and main2's while loop.  Said another way
 * output should be something like
 *   Main 1 says Hello
 *   Main 2 says Hello
 *   Main 1 says Hello  
 *   ...
   No use of pthread or similar library 
 * Note:  This assignment should be completed by the time we 
 * get to Chapter 27 of the OSTEP book.  It will take some time 
 * to understand the details and get this right.  Don't wait until
 * then to start working on this assignment.  It is not much code
 * but it is code that needs to be well thought out.
 */

#include <iostream>
#include <iomanip>
#include <unistd.h>

using namespace std;

typedef void (*funPtr)(int); // main1 and main2 are examples of this prototype


struct Context {
  long rdi; // Register Destination Index - used for passing arguments to functions
  long rsp; // Stack Pointer - contains the location of the current stack
  long rax; // Register A - used for storing the return value of a function
  long rbx; // Register B - used for storing the return value of a function
  long rcx; // Register C - used for storing the return value of a function
  long rdx; // Register D - used for storing the return value of a function
  long rsi; // Register Source Index - used for passing arguments to functions
  long rip; // Instruction Pointer - used for storing the current instruction location
  long rbp; // Register Base Pointer - used for storing the current stack location
};

Context threadStates[3];


void startThread(funPtr ptr) {
 // To-Do
}

void shareCPU(int thread) {
  // To-Do 
}





/* Cannot change any code below */
void main1(int whoami) {
    while(true) {
        cout << "Main 1 says Hello" << endl;
        usleep(5e5);
        shareCPU(whoami);
    }
}

void main2(int whoami) {
    while(true) {
        cout << "Main 2 says Hello" << endl;
        usleep(5e5);
        shareCPU(whoami);
    }
}

int main() {
	cout << "Main Start Main1" << endl;
    startThread(main1);
    cout << "Main Start Main2" << endl;
    startThread(main2);
    while(true) {
		cout << "Share loop once in main " << endl;
		usleep(5e5);
		shareCPU(0);
	};
    return 0;
}
