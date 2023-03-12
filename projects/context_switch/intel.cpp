/** Context Switch Assignment  100pts
 *      James Halladay
 *      Professor Karl Castleton
 *      OS Design
 *      3/12/2023
 *    
 *      This program is a context switcher that switches between two threads
 *    
 *      Developed for intel's x86_64 architecture
 * 
 *      TODO:
 *          - implement different scheduling algorithms
 *          - allow command line arguments to select scheduler
 *          - translate into arm for local use on my Macbook
 *    
 *      Resources:
 *          https://godbolt.org/ <--- compiler explorer: online ide and assembly interpreter 
 *          https://gcc.gnu.org/onlinedocs/gcc/Simple-Constraints.html#Simple-Constraints
 *          https://www.cs.uaf.edu/2012/fall/cs301/lecture/10_01_link_with_cpp.html 
 *          https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C 
 * '        https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/language/asm.html
 *          https://en.wikipedia.org/wiki/X86_assembly_language 
 * 
 *      Assignment instructions:
 *
 *          Using your knowledge of assembly, stack frames for functions
 *          and the calling convention of the compiler, write code below in the
 *          "//  ToDo" sections so that the program alternates between running
 *          main1's and main2's while loop.  Said another way
 *          output should be something like
 *              Main 1 says Hello
 *              Main 2 says Hello
 *              Main 1 says Hello  
 *              ...
 *          No use of pthread or similar library 
 *          Note:  This assignment should be completed by the time we 
 *          get to Chapter 27 of the OSTEP book.  It will take some time 
 *          to understand the details and get this right.  Don't wait until
 *          then to start working on this assignment.  It is not much code
 *          but it is code that needs to be well thought out.
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

typedef void (*funPtr)(int);

const int TOTAL_THREADS = 3, STACK_SIZE = sizeof(long)*9*1000;
int sentinal = 0, currentThread = 0, numThreads = 1;

struct Context {
  long rax;
  long rbx;
  long rcx;
  long rdx;
  long rsi;
  long rdi;
  long rbp;
  long rsp;
};

struct Thread {
    char *tstack;
    int id;
    Thread *child;
    Context context;
} threads[TOTAL_THREADS];


/**
 * @brief This function will put each register into the correct field in the context field of the thread pointer
 * 
 * we dynamically store the registers in the context pointer using the extended asm syntax
 *      - the first argument is the assembly instruction
 *          - use the %% to indicate that we are using a register
 *          - use the % to indicate that we are using an outside variable
 *          - use the =m to indicate that we are writing to memory or =r to indicate that we are writing to a register
 *              - this is called the constraint
 *      - the second argument is the output operands
 *      - the third argument is the input operands
 *      - the fourth argument is the clobbered registers
 * 
 * @param t a thread pointer
 */
void saveRegister(Thread *t) {

    // extended asm syntax that uses a macro to put the memory location in a register and then replace %0 with that register
    asm("mov %%rdi, %0" : "=m" (t->context.rdi));
    asm("mov %%rsp, %0" : "=m" (t->context.rsp));
    asm("mov %%rax, %0" : "=m" (t->context.rax));
    asm("mov %%rbx, %0" : "=m" (t->context.rbx));
    asm("mov %%rcx, %0" : "=m" (t->context.rcx));
    asm("mov %%rdx, %0" : "=m" (t->context.rdx));
    asm("mov %%rsi, %0" : "=m" (t->context.rsi));
    asm("mov %%rbp, %0" : "=m" (t->context.rbp));
}


/**
 * @brief This function will take the thread pointer and move all of the contents of its context field into the registers
 * 
 * we dynamically load the registers from the context pointer using the extended asm syntax
 *     - the first argument is the assembly instruction
 *         - use the %% to indicate that we are using a register
 *         - use the % to indicate that we are using an outside variable
 *         - use the =m to indicate that we are reading from memory or =r to indicate that we are reading from a register
 *     - the second argument is the output operands
 *     - the third argument is the input operands
 *      
 * 
 * @param t a thread pointer
 */
void loadRegister(Thread *t) {
    asm("mov %0, %%rdi" : "=m" (t->context.rdi));
    asm("mov %0, %%rsp" : "=m" (t->context.rsp));
    asm("mov %0, %%rax" : "=m" (t->context.rax));
    asm("mov %0, %%rbx" : "=m" (t->context.rbx));
    asm("mov %0, %%rcx" : "=m" (t->context.rcx));
    asm("mov %0, %%rdx" : "=m" (t->context.rdx));
    asm("mov %0, %%rsi" : "=m" (t->context.rsi));
    asm("mov %0, %%rbp" : "=m" (t->context.rbp));
}


/**
 * @brief this function assigns a thread to the function and overwrites the stack with the thread stack
 * 
 * @param fun a function pointer 
 */
void launchThread(funPtr fun) {
    currentThread = ++numThreads - 1;

    // overwrite stack with thread stack 
    asm("mov %0, %%rsp" : : "m" (threads[currentThread].tstack)); 
    
    fun(currentThread);
}


/**
 * @brief This function will save the current thread and then launch a new thread
 * 
 * @param fun a function pointer
 */
void startThread(funPtr fun) {
    saveRegister(&threads[currentThread]);
    
    // save stack for current thread
    asm("mov %%rsp, %0" : "=m" (threads[currentThread].tstack)); 

    launchThread(fun);
}


/**
 * @brief This function will determine which thread to load next
 * 
 * TODO:
 *    - allow scheduling to change through either a passed in or global variable
 * 
 */
void schedule() {
    if (currentThread == 1 && sentinal == 0) {
        sentinal++;
        currentThread = 0;
    } else if (currentThread == 2 && sentinal == 1) {
        sentinal++;
        currentThread = 0;
    } else if (currentThread >= 0 && currentThread < 2) {
        currentThread++;
    } else {
        currentThread--;
    }
}


/**
 * @brief This function will save the current thread and then load the next thread
 * 
 * @param threadId the id of the thread to be stopped
 */
void shareCPU(int threadId) {
    saveRegister(&threads[threadId]);

    // call schedule to determine which thread to load
    schedule();

    loadRegister(&threads[currentThread]);
}


/**
 * @brief This function gives the threads their initial state
 * 
 * TODO:
 *      - change scheduling through a command line argument
 * 
 * @param argc 
 * @param argv 
 */
void init(int argc, char *argv[]) {
    if(argc > 1) {
        cout << "No functionality for command line arguments added yet" << endl;
        exit(1);
    }

    for(int i = 0; i < TOTAL_THREADS; i++) {
        threads[i].tstack = (char *)malloc(STACK_SIZE) + STACK_SIZE;
        threads[i].id = i;
    }
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


int main(int argc, char *argv[]) {

    init(argc, argv);

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
