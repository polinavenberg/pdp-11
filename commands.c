#include "pdp11.h"
#include <stdio.h>
#include <stdlib.h>


Args get_ss_dd(word w){
    Args res;
    int reg_number = w & 7;
    int mode = (w >> 3) & 7;
    switch (mode){
        case 0:    //Rn, в регистре хранится значение
            res.adr = reg_number;
            res.val = reg[reg_number];
            break;
        case 1:     //(Rn), в регистре лежит номер ячейки памяти, где лежит значение
            res.adr = reg[reg_number];
            res.val = w_read(res.adr);
            break;
        case 2:     //(Rn)+, #nn, в регистре лежит номер ячейки памяти, где лежит значение.
        //Содержание регистра увеличится на 2 после операции
            res.adr = reg[reg_number];
            res.val = w_read(res.adr);
            if (b_flag.val)
                reg[reg_number] += 1;
            else
                reg[reg_number] += 2;
            break;
        case 3:     //@(Rn)+, в регистре лежит номер ячейки памяти, в которой лежит адрес, где лежит значение.
        //Содержание регистра увеличится на 2 после операции
            res.adr = reg[reg_number];
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
            if (b_flag.val)
                reg[reg_number] += 1;
            else
                reg[reg_number] += 2;
            break;
        case 4:     //-(Rn), содержимое регистра до операции уменьшается на 2 или 1 и используется как адрес,
        // по которому лежит значение
            if (b_flag.val)
                reg[reg_number] -= 1;
            else
                reg[reg_number] -= 2;
            res.adr = reg[reg_number];
            res.val = w_read(res.adr);
            break;
        case 5:     //@-(Rn), содержимое регистра уменьшается на 2 или 1 и используется как адрес адреса,
        //по которому лежит значение
            if (b_flag.val)
                reg[reg_number] -= 1;
            else
                reg[reg_number] -= 2;
            res.adr = reg[reg_number];
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
            break;
        case 6:     //X(Rn), содержимое регистра складывается с числом, записанным после команды, и полученная сумма
        //используется в качестве адреса, в котором лежит значение4
            word X = w_read(pc);
            pc += 2;
            res.adr = reg[reg_number] + X;
            res.val = w_read(res.adr);
            break;
        case 7:     //@X(Rn), содержимое регистра складывается с числом, записанным после команды, и полученная сумма
        //указывает на ячейку, которая содержит адрес, по которому лежит значение.
            word X = w_read(pc);
            pc += 2;
            res.adr = reg[reg_number] + X;
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
            break;
    }
    if (pc % 2 == 1)
        pc++;
    return res;
}

void do_halt(){
    trace("THE END!!!\n");
    exit(0);
}

void do_mov(){}
void do_add(){}
void do_nothing(){}

void run() {
    trace("RUN:\n");
    pc = 01000;
    while (1) {
        word w = w_read(pc);
        trace("%06o %06o: ", pc, w);
        pc += 2;
        if (w == 0){
            trace("halt ");
            do_halt();
        }
        else if ((w & 0170000) == 0010000){   // 01SSDD
            trace("mov ");
            do_mov();
        }
        else if ((w & 0170000) == 0060000){   // 06SSDD
            trace("add ");
            do_add();

        }
    }
}


Command cmd[] = {
        {0170000, 0010000, "mov", do_mov},
        {0170000, 0060000, "add", do_add}
};