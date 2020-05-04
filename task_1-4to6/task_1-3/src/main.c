#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main(void) {
uint8_t i= 170;//input
uint8_t l=0;
uint8_t o=0;//output
l=i & 1;
o|=(l<<7);
l=i & (1<<1);
o|=(l<<5);
l=i & (1<<2);
o|=(l<<3);
l=i & (1<<3);
o|=(l<<1);
l=i & (1<<4);
o|=(l>>1);
l=i & (1<<5);
o|=(l>>3);
l=i & (1<<6);
o|=(l>>5);
l=i & (1<<7);
o|=(l>>7);
printf("reverse number= %d\n", o);

}
