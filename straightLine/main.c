#include "prog1.h"

int maxargsStm(A_stm s);

int maxargsExp(A_exp e) {
    switch(e->kind) {
        case A_idExp:
        case A_numExp:
        case A_opExp:
            return 1;
        case A_eseqExp: {
            int n1 =   maxargsStm(e->u.eseq.stm);
            int n2 =   maxargsExp(e->u.eseq.exp);
            return n1 > n2 ? n1 : n2;
        }
        default:return 0;
    }
}

int maxargsExpList(A_expList l) {
    if(l->kind == A_pairExpList) {
        return maxargsExp(l->u.pair.head) + maxargsExpList(l->u.pair.tail);
    }else {
        return 1;
    }
}


int maxargsStm(A_stm s) {
    if(s->kind == A_compoundStm) {
        int n1 = maxargsStm(s->u.compound.stm1);
        int n2 = maxargsStm(s->u.compound.stm2);
        return n1 > n2 ? n1 : n2;
    }
    else if(s->kind == A_assignStm) {
        return maxargsExp(s->u.assign.exp);
    }
    else if(s->kind == A_printStm) {
        return maxargsExpList(s->u.print.exps);
    }else {
        return 0;
    }
}



int main() {
    A_stm s = prog();
    printf("maxargs:%d\n",maxargsStm(s));
    interp(s);
    printf("\n");
}