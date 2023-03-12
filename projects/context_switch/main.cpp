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


/**
 * @brief This function will put each register into the correct field in the context pointers location
 * 
 * current implementation:
 *  we dynamically store the registers in the context pointer using the extended asm syntax
 *       - the first argument is the assembly instruction
 *           - use the %% to indicate that we are using a register
 *           - use the % to indicate that we are using an outside variable
 *           - use the =m to indicate that we are writing to memory or =r to indicate that we are writing to a register
 *       - the second argument is the output operands
 *       - the third argument is the input operands
 *       - the fourth argument is the clobbered registers
 * 
 * previous implementation:
 *  each register takes 8 bits, so we store each register 8 bits after the previous one
 * 
 * @param p a context pointer
 */
void saveRegisters(Context *p) {
    // move all of the registers into the context pointer

    asm("mov %%rdi, %0" : "=m" (p->rdi)); // from rdi to &p->rdi
    asm("mov %%rsp, %0" : "=m" (p->rsp)); // from rsp to &p->rsp
    asm("mov %%rax, %0" : "=m" (p->rax)); // from rax to &p->rax
    asm("mov %%rbx, %0" : "=m" (p->rbx)); // from rbx to &p->rbx
    asm("mov %%rcx, %0" : "=m" (p->rcx)); // from rcx to &p->rcx
    asm("mov %%rdx, %0" : "=m" (p->rdx)); // from rdx to &p->rdx
    asm("mov %%rsi, %0" : "=m" (p->rsi)); // from rsi to &p->rsi
    asm("mov %%rbp, %0" : "=m" (p->rbp)); // from rbp to &p->rbp

    // previous implementation
    // asm("mov %rdi, (%rdi)");
    // asm("mov %rsp, 8(%rdi)"); 
    // asm("mov %rax, 16(%rdi)"); 
    // asm("mov %rbx, 24(%rdi)"); 
    // asm("mov %rcx, 32(%rdi)"); 
    // asm("mov %rdx, 40(%rdi)");
    // asm("mov %rsi, 48(%rdi)"); 
    // asm("mov %rbp, 56(%rdi)");
}

/**
 * @brief This function will take the context pointer and move all of the contents into the appropriate 
 *          register
 * 
 * current implementation:
 *  we dynamically load the registers from the context pointer using the extended asm syntax
 *      - the first argument is the assembly instruction
 *          - use the %% to indicate that we are using a register
 *          - use the % to indicate that we are using an outside variable
 *          - use the =m to indicate that we are reading from memory or =r to indicate that we are reading from a register
 *      - the second argument is the output operands
 *      - the third argument is the input operands
 *      
 * 
 * previous implementation:
 *  each register takes 8 bits, so we store each value in the context pointer 8 bits after the previous one
 * 
 * @param p a context pointer
 */
void loadRegisters(Context *p) {
    asm("mov %0, %%rdi" : "=m" (p->rdi));
    asm("mov %0, %%rsp" : "=m" (p->rsp));
    asm("mov %0, %%rax" : "=m" (p->rax));
    asm("mov %0, %%rbx" : "=m" (p->rbx));
    asm("mov %0, %%rcx" : "=m" (p->rcx));
    asm("mov %0, %%rdx" : "=m" (p->rdx));
    asm("mov %0, %%rsi" : "=m" (p->rsi));
    asm("mov %0, %%rbp" : "=m" (p->rbp));

    // previous implementation
    // asm("mov (%rdi), %rdi");
    // asm("mov 8(%rdi), %rsp");
    // asm("mov 16(%rdi), %rax");
    // asm("mov 24(%rdi), %rbx");
    // asm("mov 32(%rdi), %rcx");
    // asm("mov 40(%rdi), %rdx");
    // asm("mov 48(%rdi), %rsi");
    // asm("mov 56(%rdi), %rbp");
}


void launchThread(funPtr fun, void *stack) {
    // asm("mov %esi, %esp"); // save the stack(esi) and put it in the stack pointer

    // numThreads++;
    // currentThread = numThreads - 1; // give each thread a new unique identifier
    // fun(currentThread);

    asm("mov %rsi,%rsp"); //rsi is SECOND argument, put in the stack pointer
    asm("mov %0, %%rsp" : : "m" (stack));
    currentThread = numThreads - 1;
    fun(currentThread);
}


void startThread(funPtr fun) {
    // To-Do
    stack = (char *)malloc(sizeof(long)*9*1000) + (sizeof(long)*9*1000);
    numThreads++;
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
    // saveRegisters(&threadStates[thread]);
    saveRegisters(&threadStates[currentThread]);


    // First check to see if there are other threads running
    if (currentThread ==  1 && numThreads > 2) { // if 1 is running, run 2
        currentThread = 2;
    } else if (currentThread == 2) { // if 2 is running, run 1
        currentThread = 1;
    } else { // if none are running, run 0
        currentThread = 0;
    } 

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
