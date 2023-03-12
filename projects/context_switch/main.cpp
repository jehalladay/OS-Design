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
// #include <unistd.h>

using namespace std;

char *stack; // represents the stack for the program
int currentThread = 0, numThreads = 1;
typedef void (*funPtr)(int); // main1 and main2 are examples of this prototype

struct Context {
    long edi; // Register Destination Index - used for passing arguments to functions
    long esp; // Stack Pointer - contains the location of the current stack
    long eax; // Register A - used for storing the return value of a function
    long ebx; // Register B - used for storing the return value of a function
    long ecx; // Register C - used for storing the return value of a function
    long edx; // Register D - used for storing the return value of a function
    long esi; // Register Source Index - used for passing arguments to functions
    long ebp; // Register Base Pointer - used for storing the current stack location
    
} threadStates[3];


/**
 * @brief This function will put each register into the correct field in the context pointers location
 * 
 * each register takes 8 bits, so we store each register 8 bits after the previous one
 * 
 * @param p a context pointer
 */
void saveRegisters(Context *p) {
    // move all of the registers into the context pointer
    // asm("mov %edi, %0" : "=r" "%edi" (p->edi));
    // asm("mov %esp, %0" : "=r" "%esp" (p->esp));
    // asm("mov %eax, %0" : "=r" "%eax" (p->eax));
    // asm("mov %ebx, %0" : "=r" "%ebx" (p->ebx));
    // asm("mov %ecx, %0" : "=r" "%ecx" (p->ecx));
    // asm("mov %edx, %0" : "=r" "%edx" (p->edx));
    // asm("mov %esi, %0" : "=r" "%esi" (p->esi));
    // asm("mov %ebp, %0" : "=r" "%ebp" (p->ebp));


    asm("mov %edi, %0" : "=r" "%%edi" (p->edi));
    asm("mov %esp, %0" : "=r" "%%esp" (p->esp));
    asm("mov %eax, %0" : "=r" "%%eax" (p->eax));
    asm("mov %ebx, %0" : "=r" "%%ebx" (p->ebx));
    asm("mov %ecx, %0" : "=r" "%%ecx" (p->ecx));
    asm("mov %edx, %0" : "=r" "%%edx" (p->edx));
    asm("mov %esi, %0" : "=r" "%%esi" (p->esi));
    asm("mov %ebp, %0" : "=r" "%%ebp" (p->ebp));

    // asm("mov %edi, (%edi)");  
    // asm("mov %esp, 8(%edi)"); 
    // asm("mov %eax, 16(%edi)"); 
    // asm("mov %ebx, 24(%edi)"); 
    // asm("mov %ecx, 32(%edi)"); 
    // asm("mov %edx, 40(%edi)");
    // asm("mov %esi, 48(%edi)"); 
    // asm("mov %ebp, 56(%edi)");
}

/**
 * @brief This function will take the context pointer and move all of the contents into the appropriate 
 *          register
 * 
 * @param p a context pointer
 */
void loadRegisters(Context *p) {
    asm("mov (%edi), %edi");
    asm("mov 8(%edi), %esp");
    asm("mov 16(%edi), %eax");
    asm("mov 24(%edi), %ebx");
    asm("mov 32(%edi), %ecx");
    asm("mov 40(%edi), %edx");
    asm("mov 48(%edi), %esi");
    asm("mov 56(%edi), %ebp");
}


void launchThread(funPtr fun, void *stack) {
    asm("mov %esi, %esp"); // save the stack(esi) and put it in the stack pointer

    numThreads++;
    currentThread = numThreads - 1; // give each thread a new unique identifier
    fun(currentThread);
}


void startThread(funPtr fun) {
    // To-Do
    stack = (char *)malloc(17*8*1000) + (17*8*1000); // Create a new stack 
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
        // usleep(5e5);
        shareCPU(thread);
    }
}

void main2(int thread) {
    while(true) {
        cout << "Main 2 says Hello" << endl;
        // usleep(5e5);
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
        // usleep(5e5);
        shareCPU(0);
    };

    return 0;
}
