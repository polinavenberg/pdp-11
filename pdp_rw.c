#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

# define MEMSIZE (64*1024)
typedef unsigned char byte; //8 bit
typedef unsigned short int word; //16 bit
typedef word adr; //16 bit

byte mem[MEMSIZE];

void b_write(adr a, byte b);

byte b_read(adr a);

void w_write(adr a, word w);

word w_read(adr a);

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


int main() {
    test_mem();
    return 0;
}

void w_write(adr a, word w) {
    //выдает сообщение об ошибке, если передан адрес не первого(четного) байта
    assert(a % 2 == 0);
    mem[a] = (byte) ((w << 8) >> 8);
    mem[a + 1] = (byte) (w >> 8);
}


word w_read(adr a) {
    word w = ((word) mem[a + 1]) << 8;
    w = w | mem[a];
    return w;

}

byte b_read(adr a) {
    return mem[a];
}

void b_write(adr a, byte b) {
    mem[a] = b;
}
