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

char *stack; // represents the stack for the program
int currentThread = 0, numThreads = 1;
typedef void (*funPtr)(int); // main1 and main2 are examples of this prototype

struct Context {
    long rdi; // Register Destination Index - used for passing arguments to functions
    long rsp; // Stack Pointer - contains the location of the current stack
    long rax; // Register A - used for storing the return value of a function
    long rbx; // Register B - used for storing the return value of a function
    long rcx; // Register C - used for storing the return value of a function
    long rdx; // Register D - used for storing the return value of a function
    long rsi; // Register Source Index - used for passing arguments to functions
    long rbp; // Register Base Pointer - used for storing the current stack location
} threadStates[3];


struct Context {
    long edi; // <--- r5
    long esp; // <--- r13
    long eax; // <--- r0
    long ebx; // <--- r1
    long ecx; // <--- r2 
    long edx; // <--- r3
    long esi; // <--- 
    long ebp; // <---
} threadStates[3];





/**
 * @brief This function will put each register into the correct field in the context pointers location
 * 
 * each register takes 8 bits, so we store each register 8 bits after the previous one
 * 
 * @param p a context pointer
 */
void saveRegisters(Context *p) {
    asm("mov %rdi, (%rdi)");  
    asm("mov %rsp, 8(%rdi)"); 
    asm("mov %rax, 16(%rdi)"); 
    asm("mov %rbx, 24(%rdi)"); 
    asm("mov %rcx, 32(%rdi)"); 
    asm("mov %rdx, 40(%rdi)");
    asm("mov %rsi, 48(%rdi)"); 
    asm("mov %rbp, 56(%rdi)");
}

/**
 * @brief This function will take the context pointer and move all of the contents into the appropriate 
 *          register
 * 
 * @param p a context pointer
 */
void loadRegisters(Context *p) {
    asm("mov (%rdi), %rdi");
    asm("mov 8(%rdi), %rsp");
    asm("mov 16(%rdi), %rax");
    asm("mov 24(%rdi), %rbx");
    asm("mov 32(%rdi), %rcx");
    asm("mov 40(%rdi), %rdx");
    asm("mov 48(%rdi), %rsi");
    asm("mov 56(%rdi), %rbp");
}


void launchThread(funPtr fun, void *stack) {
    asm("mov %rsi, %rsp"); // save the stack(rsi) and put it in the stack pointer

    numThreads++;
    currentThread = numThreads - 1; // give each thread a new unique identifier
    fun(currentThread);
}


void startThread(funPtr fun) {
    // To-Do
    stack = (char *)malloc(64000) + 64000; // Create a new stack 
    saveRegisters(&threadStates[currentThread]);
    launchThread(fun, stack);
}


/**
 * @brief This function will save the state of the current thread and then load one of our saved states
 * 
 * @param thread 
 */
void shareCPU(int thread) {
    // To-Do 
    saveRegisters(&threadStates[thread]);
    // saveRegisters(&threadStates[currentThread]);


    // // First check to see if there are other threads running
    // if (currentThread ==  1 && numThreads > 2) { // if 1 is running, run 2
    //     currentThread = 2;
    // } else if (currentThread == 2) { // if 2 is running, run 1
    //     currentThread = 1;
    // } else { // if none are running, run 0
    //     currentThread = 0;
    // } 

    loadRegisters(&threadStates[currentThread]);
    // loadRegisters(&threadStates[thread]);
}

/* Cannot change any code below */
void main1(int thread) {
    while(true) {
        cout << "Main 1 says Hello" << endl;
        usleep(5e5);
        shareCPU(thread);
    }
}

void main2(int thread) {
    while(true) {
        cout << "Main 2 says Hello" << endl;
        usleep(5e5);
        shareCPU(thread);
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
