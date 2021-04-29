#include <stdarg.h>
#include <stdio.h>
#include "pdp11.h"

void trace(const char * fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

void print_reg(){
    for (int i = 0; i < REG_SIZE; i++) {
        trace("R%d = %06o ", i, reg[i]);
        if (i == 3)
            trace("\n");
    }
    trace("\n");
}

void print_mem(word start_adr, word end_adr)
{
    trace("MEM:\n");
    while (start_adr <= end_adr) {
        word w = w_read(start_adr);
        trace("%06o: %06o\n", start_adr, w);
        start_adr += 2;
    }
}

