#include <stdio.h>

struct context {
    int one;
    int two;
};

long fun(long x, context *p) {
    asm("in_fun:");
    context *c = p;
    c->one = x;
    x = 1;
    c->two = x;
    // b = b+1;
    return x+14;

}

long z=11;
int main() {
    asm("start_main:");
    long x = 12;
    asm("before_context:");
    context p[3];
    asm("before_call:");
    long y = fun(13, &p[0]);
    asm("after_call:");
    printf("%d\n", (int)y);
    x++;

    return 0;
}