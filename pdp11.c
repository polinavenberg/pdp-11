#include "pdp11.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int t_flag = 0; //флаг для трессировки
int T_flag = 0; //флаг для большой трессировки

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


int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 't')
                t_flag = 1;
            else if (argv[i][1] == 'T')
                T_flag = 1;
        }
        else
            load_file(argv[i]);
    }
   // print_mem(01000, 01100);
    trace("\n");
    run();
    print_reg();
    return 0;
}