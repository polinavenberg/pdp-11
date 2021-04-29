#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pdp11.h"

void load_file();

void mem_dump(adr start, word n);

void test_mem() {
    // пишем байт, читаем байт
    byte b0 = 0x0a;
    b_write(2, b0);
    byte bres = b_read(2);
    assert(b0 == bres);

    // пишем два байта, читаем слово
    adr a = 4;
    b0 = 0x0a;
    byte b1 = 0xcb;
    word w = 0xcb0a;
    b_write(a, b0);
    b_write(a + 1, b1);
    word wres = w_read(a);
    assert(w == wres);

    // пишем слово, читаем слово
    a = 4;
    w = 0xcb0a;
    w_write(a, w);
    wres = w_read(a);
    assert(w == wres);

    //пишем слово, читаем два байта
    a = 4;
    b0 = 0x0a;
    b1 = 0xcb;
    w = 0xcb0a;
    w_write(a, w);
    byte b0res = b_read(a);
    byte b1res = b_read(a + 1);
    assert(b0 == b0res);
    assert(b1 == b1res);
}


void load_file(const char * filename) {
    adr start;
    int i;
    word n;
    FILE *fl = fopen(filename, "r");
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


int main() {
    test_mem();
    return 0;
}
