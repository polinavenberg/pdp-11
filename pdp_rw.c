#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

# define MEMSIZE (64*1024)
typedef unsigned char byte; //8 bit
typedef unsigned short int word; //16 bit
typedef word adr; //16 bit

word mem[MEMSIZE];

word w_read(adr a) {
    return mem[a];
}

void w_write(adr a, word val) {
    mem[a] = val;
}

byte b_read(adr a) {
    byte b;
    if (a % 2 == 0)
        b = mem[a] & 0xFF;
    else
        b = mem[a - 1] >> 8 & 0xFF;
    return b;
}

void b_write(adr a, byte val) {
    if (a % 2 == 0)
        mem[a] = val;
    else
        mem[a - 1] = (word) val << 8 | mem[a - 1];
}

