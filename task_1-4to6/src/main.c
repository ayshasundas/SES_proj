#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// You have to declare functions before you can use them.
// The definition of them may take place at another place.

/**add two variables, uses call-by-value (copy of variables is passed)
 */
uint8_t add(uint8_t a, uint8_t b) {
    printf("a and b in add function = %x and %x\n", a, b);
    a += b;
    return a;
}

/**swap two variables, uses call-by-pointer (pointer to variables is passed)
 */
void swap(uint8_t *a, uint8_t *b) {
    printf("*a in swap function = %x\n",*a);
    uint8_t c = *a;
    printf("c in swap function = %x\n",c);
    *a = *b;
    printf("*a = *b in swap function = %x\n",*a);
    *b = c;
    printf("*b=c in swap function = %x\n",*b);
}

typedef uint8_t (*aFunctionPointer_t)(uint8_t a, uint8_t b);


uint8_t add(uint8_t a, uint8_t b);
void swap(uint8_t *a, uint8_t *b);

int main(void) {
    printf("!!!Hello World!!!\n");

    // define some variables
    uint8_t a = 3;
    uint8_t b = 5;
    uint8_t *c = &a;

    // call some functions
    printf("a=%x\nb=%d\nc=%x\n", &a, b, c);
    printf("result of add: a+b=%d\n", add(a, b));

    swap(&a, &b);
    printf("result of swap: a=%d, b=%d\n", a, b);

    *c = add(a, b);
    printf("result of add: c=%d\n", *c);
    printf("result of add: a+b=%d?\n", add(a, b));

    printf("result of add: 200+100=%d?\n", add(200, 100));

    // sometimes you have to store the address of a function in a pointer
    aFunctionPointer_t p = &add;
    printf("call add via function pointer, result: %d\n", p(3,6));


    // bitwise operations

    // uses decimal numbers/ operation OR
    printf("3|5=%d\n", 3 | 5);

    // use hex and decimal numbers/ operation AND
    printf("0xf2&7=0x%02x\n", 0xf2 & 7);   //Why 02

    // use binary numbers/ operation XOR
    printf("0xb1011^0xb1100=%d \n", 0b1011 ^ 0b1100);

    // use decimal numbers/ operation NOT
    printf("~13=%d \n", (uint8_t) ~13);

    // left and right shift
    printf("1<<3=%d\n", 1 << 3);
    printf("0xf0>>4=0x%02x\n", 0xF0 >> 4);

    return EXIT_SUCCESS;
}
