#include "pdp11.h"
#include <stdio.h>
#include <stdlib.h>


extern byte mem[MEMSIZE];
extern word reg[REG_SIZE];
extern Args ss, dd, b_flag, r, nn, xx, rn;
extern char N_flag, Z_flag, V_flag, C_flag;

void set_NZ(word w) {
    if (w >> 15) {
        Z_flag = 0;
        N_flag = 1;
    } else {
        N_flag = 0;
        if (w == 0)
            Z_flag = 1;
        else
            Z_flag = 0;
    }
}

void set_NZb(byte b) {
    if (b >> 7) {
        Z_flag = 0;
        N_flag = 1;
    } else {
        N_flag = 0;
        if (b == 0)
            Z_flag = 1;
        else
            Z_flag = 0;
    }
}

void set_ALL(int w) {
    set_NZ(w);
    C_flag = (w >> 16) & 1;
}


void do_halt() {
    printf("\n");
    printf("THE END!!!\n");
    print_reg();
    exit(0);
}

void do_mov() {
    if (b_flag.val) {
        b_write(dd.adr, (byte) ss.val);
        set_NZb((byte)ss.val);
    }
    else {
        w_write(dd.adr, ss.val);
        set_NZ(ss.val);
    }
    if ((dd.adr == odata) && ((w_read(ostat) >> 7) & 1)) //putchar
        printf("%c", ss.val);
}


void do_add() {
    w_write(dd.adr, (word) (dd.val + ss.val));
    set_ALL(dd.val + ss.val);
}


void do_clr() {
    w_write(dd.adr, 0);
    set_NZ(0);
    C_flag = 0;


}


void do_sob() {
    if (--reg[rn.val] != 0) {
        pc -= nn.val * 2;
    }
}


void cln() {
    N_flag = 0;
}

void clz() {
    Z_flag = 0;
}

void clv() {
    V_flag = 0;
}

void clc() {
    C_flag = 0;
}

void sen() {
    N_flag = 1;
}

void sez() {
    Z_flag = 1;
}

void sev() {
    V_flag = 1;
}

void sec() {
    C_flag = 1;
}

void ccc() {
    N_flag = 0;
    Z_flag = 0;
    V_flag = 0;
    C_flag = 0;
}

void scc() {
    N_flag = 1;
    Z_flag = 1;
    V_flag = 1;
    C_flag = 1;
}

void br() {
    pc +=  2 * (char) xx.val;
    trace(" %d", 2 * (char) xx.val);
}

void beq() {
    if (Z_flag)
        br();
}

void bne() {
    if (Z_flag == 0)
        br();
}

void bmi() {
    if (N_flag)
        br();
}

void bpl() {
    if (N_flag == 0)
        br();

}


void blt() {
    if ((N_flag ^ V_flag))
        br();
}

void bge() {
    if ((N_flag ^ V_flag) == 0)
        br();
}

void ble() {
    if ((Z_flag | (N_flag ^ V_flag)) == 1)
        br();
}

void do_nothing() {
    trace("UNKOWN COMMAND\n");
    print_reg();
    exit(0);
}

void tst() { //если b_flag 1, то это байт и делаем то, что прописано в тст, иначе это слово и делаем set_NZ
    //и выставляем C и V нулями
    if (b_flag.val) {
        set_NZb(dd.val);
    } else
        set_NZ(dd.val);
    C_flag = 0;
    V_flag = 0;
}

void jsr(){
    word temp = dd.adr;//адрес перехода
    w_write(sp -= 2, w_read(rn.val));//содержимое регистра R сохраняется в стеке
    w_write(rn.val, pc);// в регистр R заносится адрес возврата
    pc = temp;//в PC заносится адрес перехода
}

void rts(){
    pc = w_read(r.val);//адрес возврата переносится из R в pc
    reg[r.val] = w_read(sp);//старое содержимое R восстанавливается из стека
    sp += 2;
}

// params
// 0 - nothing
// 1 - DD
// 2 - SS
// 3 - SS and DD
Command cmd[] = {
        {0177777, 0000000, "halt", do_halt, HAS_NOTHING},
        {0070000, 0010000, "mov", do_mov, HAS_SS_DD},
        {0170000, 0060000, "add", do_add, HAS_SS_DD},
        {0077700, 0005000, "clr", do_clr, HAS_DD},
        {0177000, 0077000, "sob", do_sob, HAS_NOTHING},
        {0177777, 0000250, "cln", cln, HAS_NOTHING},
        {0177777, 0000244, "clz", clz, HAS_NOTHING},
        {0177777, 0000242, "clv", clv, HAS_NOTHING},
        {0177777, 0000241, "clc", clc, HAS_NOTHING},
        {0177777, 0000257, "ccc", ccc, HAS_NOTHING},
        {0177777, 0000270, "sen", sen, HAS_NOTHING},
        {0177777, 0000264, "sez", sez, HAS_NOTHING},
        {0177777, 0000262, "sev", sev, HAS_NOTHING},
        {0177777, 0000261, "sec", sec, HAS_NOTHING},
        {0177777, 0000277, "scc", scc, HAS_NOTHING},
        {0177400, 0000400, "br", br, HAS_NOTHING},
        {0177400, 0001400, "beq", beq, HAS_NOTHING},
        {0177400, 0001000, "bne", bne, HAS_NOTHING},
        {0177400, 0100400, "bmi", bmi, HAS_NOTHING},
        {0177400, 0100000, "bpl", bpl, HAS_NOTHING},
        {0177400, 0002400, "blt", blt, HAS_NOTHING},
        {0177400, 0002000, "bge", bge, HAS_NOTHING},
        {0177400, 0003400, "ble", ble, HAS_NOTHING},
        {0077700, 0005700, "tst", tst, HAS_DD},
        {0177000, 0004000, "jsr", jsr, HAS_DD},
        {0177770, 0000200, "rts", rts, HAS_NOTHING},
        {0000000, 0000000, "nothing", do_nothing, 0}


};
int cmd_len = sizeof(cmd) / sizeof(Command);