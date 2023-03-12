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
 * but it is code that needs to be well thought out.*/

#include <iostream>
#include <iomanip>
#include <unistd.h>

using namespace std;

typedef void (*funPtr)(int);
// main1 and main2 are examples of this prototype

struct Context {
  long rdi;
  long rsp;
  long rax;
  long rbx;
  long rcx;
  long rdx;
  long rsi;
  long rbp;
};

Context threadStates[3];
char *stack; // make an array of stacks
int currentThread = 0;
int numberOfThreads = 1;

void saveRegs(Context *p) {
    asm("mov %rdi, (%rdi)");
    asm("mov %rsp, 8(%rdi)"); 
    asm("mov %rax, 16(%rdi)"); 
    asm("mov %rbx, 24(%rdi)"); 
    asm("mov %rcx, 32(%rdi)"); 
    asm("mov %rdx, 40(%rdi)");
    asm("mov %rsi, 48(%rdi)"); 
    asm("mov %rbp, 56(%rdi)");
}

void loadRegs(Context *p) {
    asm("mov (%rdi), %rdi");
    asm("mov 8(%rdi), %rsp");
    asm("mov 16(%rdi), %rax");
    asm("mov 24(%rdi), %rbx");
    asm("mov 32(%rdi), %rcx");
    asm("mov 40(%rdi), %rdx");
    asm("mov 48(%rdi), %rsi");
    asm("mov 56(%rdi), %rbp");
}

void launchThread(funPtr MainPtr, void *stack) {
    asm("mov %rsi,%rsp"); //rsi is SECOND argument, put in the stack pointer
    currentThread = numberOfThreads - 1;
    MainPtr(currentThread);
}

void startThread(funPtr ptr) {
 // To-Do
    stack=(char *)malloc(56000)+56000;
    numberOfThreads++;
    Context *c;
    c = &threadStates[currentThread];
    saveRegs(c);
    launchThread(ptr, stack);
}

void shareCPU(int thread) {
  // To-Do 
  Context *c;
  c = &threadStates[currentThread];
  saveRegs(c);
  if(currentThread ==  1 && numberOfThreads > 2)
    currentThread = 2;
  else if (currentThread == 2)
    currentThread = 1;
  else currentThread = 0;
  //currentThread = rand()%numberOfThreads;
  c = &threadStates[currentThread];
  loadRegs(c);
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
