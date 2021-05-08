#include "pdp11.h"
#include <stdio.h>
#include <stdlib.h>


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
    if (b_flag.val)
        b_write(dd.adr, (byte) ss.val);
    else
        w_write(dd.adr, ss.val);
    set_NZ(w_read(dd.adr));
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
        if ((dd.val >> 7) & 1) {
            Z_flag = 0;
            N_flag = 1;
        } else {
            N_flag = 0;
            if (dd.val == 0)
                Z_flag = 1;
            else
                Z_flag = 0;
        }
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
        {0177777, 0000000, "halt", do_halt, 0},
        {0070000, 0010000, "mov", do_mov, 3},
        {0170000, 0060000, "add", do_add, 3},
        {0077700, 0005000, "clr", do_clr, 1},
        {0177000, 0077000, "sob", do_sob, 0},
        {0177777, 0000250, "cln", cln, 0},
        {0177777, 0000244, "clz", clz, 0},
        {0177777, 0000242, "clv", clv, 0},
        {0177777, 0000241, "clc", clc, 0},
        {0177777, 0000257, "ccc", ccc, 0},
        {0177777, 0000270, "sen", sen, 0},
        {0177777, 0000264, "sez", sez, 0},
        {0177777, 0000262, "sev", sev, 0},
        {0177777, 0000261, "sec", sec, 0},
        {0177777, 0000277, "scc", scc, 0},
        {0177400, 0000400, "br", br, 0},
        {0177400, 0001400, "beq", beq, 0},
        {0177400, 0001000, "bne", bne, 0},
        {0177400, 0100400, "bmi", bmi, 0},
        {0177400, 0100000, "bpl", bpl, 0},
        {0177400, 0002400, "blt", blt, 0},
        {0177400, 0002000, "bge", bge, 0},
        {0177400, 0003400, "ble", ble, 0},
        {0077700, 0005700, "tst", tst, 1},
        {0177000, 0004000, "jsr", jsr, 1},
        {0177770, 0000200, "rts", rts, 0},
        {0000000, 0000000, "nothing", do_nothing, 0}


};
int cmd_len = sizeof(cmd) / sizeof(Command);