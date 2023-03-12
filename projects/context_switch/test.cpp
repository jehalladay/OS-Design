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
    long x0;
    long x1;
    long x2;
    long x3;
    long x4;
    long x5;
    long x6;
    long x7;
    long x8;
    long x9;
    long x10;
    long x11;
    long x12;
    long x13;
    long x14;
    long x15;
    long sp;
} threadStates[3];





/**
 * @brief This function will put each register into the correct field in the context pointers location
 * 
 * each register takes 8 bits, so we store each register 8 bits after the previous one
 * 
 * @param p a context pointer
 */
void saveRegisters(Context *p) {
    asm("mov %0, x0" : "=r" (p->x0));
    asm("mov %0, x1" : "=r" (p->x1));
    asm("mov %0, x2" : "=r" (p->x2));
    asm("mov %0, x3" : "=r" (p->x3));
    asm("mov %0, x4" : "=r" (p->x4));
    asm("mov %0, x5" : "=r" (p->x5));
    asm("mov %0, x6" : "=r" (p->x6));
    asm("mov %0, x7" : "=r" (p->x7));
    asm("mov %0, x8" : "=r" (p->x8));
    asm("mov %0, x9" : "=r" (p->x9));
    asm("mov %0, x10" : "=r" (p->x10));
    asm("mov %0, x11" : "=r" (p->x11));
    asm("mov %0, x12" : "=r" (p->x12));
    asm("mov %0, x13" : "=r" (p->x13));
    asm("mov %0, x14" : "=r" (p->x14));
    asm("mov %0, x15" : "=r" (p->x15));
    asm("mov %0, sp" : "=r" (p->sp));
}

/**
 * @brief This function will take the context pointer and move all of the contents into the appropriate 
 *          register
 * 
 * @param p a context pointer
 */
void loadRegisters(Context *p) {
    asm("mov x0, %0" : "=r" (p->x0));
    asm("mov x1, %0" : "=r" (p->x1));
    asm("mov x2, %0" : "=r" (p->x2));
    asm("mov x3, %0" : "=r" (p->x3));
    asm("mov x4, %0" : "=r" (p->x4));
    asm("mov x5, %0" : "=r" (p->x5));
    asm("mov x6, %0" : "=r" (p->x6));
    asm("mov x7, %0" : "=r" (p->x7));
    asm("mov x8, %0" : "=r" (p->x8));
    asm("mov x9, %0" : "=r" (p->x9));
    asm("mov x10, %0" : "=r" (p->x10));
    asm("mov x11, %0" : "=r" (p->x11));
    asm("mov x12, %0" : "=r" (p->x12));
    asm("mov x13, %0" : "=r" (p->x13));
    asm("mov x14, %0" : "=r" (p->x14));
    asm("mov x15, %0" : "=r" (p->x15));
    asm("mov sp, %0" : "=r" (p->sp));
}


void launchThread(funPtr fun, void *stack) {
    asm("mov sp, %0" : : "m" (stack));
    // asm("mov %0, sp" : "=r" (stack));

    currentThread = numThreads - 1; // give each thread a new unique identifier
    fun(currentThread);
}


void startThread(funPtr fun) {
    // To-Do
    stack = (char *)malloc(64000 * sizeof(long) * 17 ) + (64000 * sizeof(long) * 17); // Create a new stack 
    numThreads++;
    cout << "inside startThread" << endl;
    saveRegisters(&threadStates[currentThread]);
    cout << "inside startThread: after saveRegisters" << endl;
    launchThread(fun, stack);
}


/**
 * @brief This function will save the state of the current thread and then load one of our saved states
 * 
 * @param thread 
 */
void shareCPU(int thread) {
    // To-Do 
    // saveRegisters(&threadStates[thread]);
    saveRegisters(&threadStates[currentThread]);

    cout << "currentThread: " << currentThread << " numThreads: " << numThreads << endl;


    // // First check to see if there are other threads running
    if (currentThread ==  1 && numThreads > 2) { // if 1 is running, run 2
        currentThread = 2;
    } else if (currentThread == 2) { // if 2 is running, run 1
        currentThread = 1;
    } else { // if none are running, run 0
        currentThread = 0;
    } 

    // loadRegisters(&threadStates[currentThread]);
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
	cout << "Main Start Main 1" << endl;
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
