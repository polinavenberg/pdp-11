#include <stdio.h>
#include "pdp11.h"
#include <stdlib.h>


extern Command cmd[];
extern int cmd_len;


int get_ss(Command command) {
    return ((command.params & 2) == 2);
}

int get_dd(Command command) {
    return ((command.params & 1) == 1);
}

void get_nn(word w) {
    nn.val = w & 077;
}

void get_b(word w) {
    b_flag.val = (w >> 15);
}

void get_r(word w) {
    r.val = w & 7;
}

void get_xx(word w) {
    xx.val = (w & 0xFF);
}

void get_rn(word w) {
    rn.val = (w >> 6) & 7;
}


Args get_ss_dd(word w) {
    Args res;
    int reg_number = w & 7; //берем последние три бита
    int mode = (w >> 3) & 7; //берем предпоследние три бита
    int x;
    switch (mode) {
        case 0:    //Rn, в регистре хранится значение
            res.adr = reg_number;
            res.val = reg[reg_number];
            trace("R%o ", reg_number);
            break;
        case 1:     //(Rn), в регистре лежит номер ячейки памяти, где лежит значение
            res.adr = reg[reg_number];
            res.val = w_read(res.adr);
            trace("(R%d) ", reg_number);
            break;
        case 2:     //(Rn)+, #nn, в регистре лежит номер ячейки памяти, где лежит значение.
            //Содержание регистра увеличится на 2 после операции
            res.adr = reg[reg_number];
            res.val = w_read(res.adr);
            if (b_flag.val && reg_number != 7)
                reg[reg_number] += 1;
            else
                reg[reg_number] += 2;
            if (reg_number == 7)
                trace("#%o ", res.val);
            else
                trace("(R%d)+ ", reg_number);

            break;
        case 3:     //@(Rn)+, в регистре лежит номер ячейки памяти, в которой лежит адрес, где лежит значение.
            //Содержание регистра увеличится на 2 после операции
            res.adr = reg[reg_number];
            res.adr = w_read(res.adr);
            word bolt = res.adr;
            res.val = w_read(res.adr);
            reg[reg_number] += 2;
            if (reg_number == 7)
                trace("@#%o ", bolt);
            else
                trace("@(R%o)+ ", r);
            break;
        case 4:     //-(Rn), содержимое регистра до операции уменьшается на 2 или 1 и используется как адрес,
            // по которому лежит значение
            if (b_flag.val)
                reg[reg_number] -= 1;
            else
                reg[reg_number] -= 2;
            res.adr = reg[reg_number];
            res.val = w_read(res.adr);
            trace("-(R%o) ", reg_number);
            break;
        case 5:     //@-(Rn), содержимое регистра уменьшается на 2 или 1 и используется как адрес адреса,
            //по которому лежит значение
            reg[reg_number] -= 2;
            res.adr = reg[reg_number];
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
            trace("@-(R%o) ", reg_number);
            break;
        case 6:     //X(Rn), содержимое регистра складывается с числом, записанным после команды, и полученная сумма
            //используется в качестве адреса, в котором лежит значение4

            x = w_read(pc);
            pc += 2;
            res.adr = reg[reg_number] + x;
            res.val = w_read(res.adr);
            if (reg_number == 7)
                trace("%o ", x);
            else
                trace("%o(R%o) ", x, r);
            break;
        case 7:     //@X(Rn), содержимое регистра складывается с числом, записанным после команды, и полученная сумма
            //указывает на ячейку, которая содержит адрес, по которому лежит значение.

            x = w_read(pc);
            pc += 2;
            res.adr = reg[reg_number] + x;
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
            if (reg_number == 7)
                trace("@%o ", x);
            else
                trace("@%o(R%o) ", x, r);
            break;
    }
    trace("= %06o ", res.val);
    return res;
}

void print_flags() {
    // NZ-C
    // trace("NZ-C: %d%d-%d\n", flag_N, flag_Z, flag_C);
    trace(N_flag ? "n" : "-");
    trace(Z_flag ? "z" : "-");
    trace("-");
    trace(C_flag ? "c" : "-");
    trace(" ");

    int i;
    for (i = 0; i < 8; i++)
        trace("r%o:%06o ", i, reg[i]);
    trace("\n");
}


void run() {
    trace("RUN:\n");
    w_write(ostat, 1 << 7);
    pc = 01000;
    while (pc != 0) {
        word w = w_read(pc);
        trace("%06o %06o: ", pc, w);
        pc += 2;
        for (int i = 0; i < cmd_len; i++) {
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                get_b(w);
                trace(cmd[i].name);
                trace(" ");
                if (get_ss(cmd[i]))
                    ss = get_ss_dd(w >> 6);
                if (get_dd(cmd[i]))
                    dd = get_ss_dd(w);
                get_nn(w);
                get_rn(w);
                get_r(w);
                get_xx(w);
                cmd[i].func();
                trace("\n");
                print_flags();
                trace("\n");
                break;
            }
        }
    }

}


