#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"

extern int T_flag;
extern int t_flag;

void trace(const char *format, ...) {
    if (T_flag) {
        va_list ap;
                va_start(ap, format);
        vprintf(format, ap);
                va_end(ap);
    }
}

void print_reg() {
    for (int i = 0; i < REG_SIZE; i++) {
        printf("R[%d] = %06o  ", i, reg[i]);
    }
    printf("\n");
}

void print_mem(word start_adr, word end_adr) {
    trace("MEM:\n");
    while (start_adr <= end_adr) {
        word w = w_read(start_adr);
        trace("%06o: %06o\n", start_adr, w);
        start_adr += 2;
    }
}


void w_write(adr a, word w) {
    //выдает сообщение об ошибке, если передан адрес не первого(четного) байта
    //assert(a % 2 == 0);
    if (a < 8)
        reg[a] = w;
    else {
        mem[a] = (w & 0xFF); //((w << 8) >> 8)
        mem[a + 1] = w >> 8;
    }
}


word w_read(adr a) {
    if (a < 8)
        return (reg[a]);
    else {
        word w = ((word) mem[a + 1]) << 8;
        w = w | mem[a];
        return w;
    }

}

byte b_read(adr a) {
    return mem[a];
}

void b_write(adr a, byte b) {
    if (a < 8) {
        if (b >> 7)
            reg[a] = 0xFF00 | b;
        else
            reg[a] = b;
    } else
        mem[a] = b;
}


void load_file(const char *filename) {
    adr start;
    int i;
    word n;
    FILE *fl = fopen(filename, "r");
    if (fl == NULL) {
        perror("Empty file or wrong filename");
    }
    while (fscanf(fl, "%hx", &start) != EOF) {
        fscanf(fl, "%hx", &n);
        for (i = 0; i < n; i++) {
            byte b;
            fscanf(fl, "%hhx", &b);
            b_write(start + i, b);
        }
    }

}


void mem_dump(adr start, word n) {
    word w;
    for (int i = 0; i < n; i += 2) {
        w = w_read(start + i);
        printf("%06o : %06o\n", start + i, w);
    }
}

