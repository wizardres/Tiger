#include "slp.h"

A_stm A_CompoundStm(A_stm stm1,A_stm stm2) {
    A_stm s = check_malloc(sizeof(*s));
    s->kind = A_compoundStm;
    s->u.compound.stm1 = stm1;s->u.compound.stm2 = stm2;
    return s;
}

A_stm A_AssignStm(string id,A_exp exp) {
    A_stm s = check_malloc(sizeof(*s));
    s->kind = A_assignStm;
    s->u.assign.id = String(id);
    s->u.assign.exp = exp;
    return s;
}

A_stm A_PrintStm(A_expList exps) {
    A_stm s = check_malloc(sizeof(*s));
    s->kind = A_printStm;
    s->u.print.exps = exps;
    return s;
}

A_exp A_IdExp(string id) {
    A_exp e = check_malloc(sizeof(*e));
    e->kind = A_idExp;
    e->u.id = String(id);
    return e;
}

A_exp A_NumExp(int num) {
    A_exp e = check_malloc(sizeof(*e));
    e->kind = A_numExp;
    e->u.num = num;
    return e;
}

A_exp A_OpExp(A_exp left,A_binop oper,A_exp right) {
    A_exp e = check_malloc(sizeof(*e));
    e->kind = A_opExp;
    e->u.op.left = left;
    e->u.op.oper = oper;
    e->u.op.right = right;
    return e;
}

A_exp A_EseqExp(A_stm stm, A_exp exp) {
    A_exp e = check_malloc(sizeof(*e));
    e->kind = A_eseqExp;
    e->u.eseq.exp = exp;
    e->u.eseq.stm = stm;
    return e;
}

A_expList A_PairExpList(A_exp head,A_expList tail) {
    A_expList e = check_malloc(sizeof(*e));
    e->kind = A_pairExpList;
    e->u.pair.head = head;
    e->u.pair.tail = tail;
    return e;
}

A_expList A_LastExpList(A_exp last) {
    A_expList e = check_malloc(sizeof(*e));
    e->kind = A_lastExpList;
    e->u.last = last;
    return e;
}

Table_ Table(string id,int value,struct table *tail) {
    Table_ t = check_malloc(sizeof(*t));
    t->id = id;
    t->value = value;
    t->tail = tail;
    return t;
}

Table_ update(Table_ t,string id,int value) {
    return Table(id,value,t);
}

int lookup(Table_ t,string key) {
    assert(t);
    if(strcmp(t->id,key) == 0) {
        return t->value;
    }
    else return lookup(t->tail,key);
}

Table_ interpStm(A_stm s,Table_ t) {
    switch(s->kind) {
        case A_compoundStm: {
            t = interpStm(s->u.compound.stm1,t);
            t = interpStm(s->u.compound.stm2,t);
            break;
        }
        case A_assignStm: {
            struct IntAndTable r = interpExp(s->u.assign.exp,t);
            return update(r.t,s->u.assign.id,r.i);
        }
        case A_printStm: {
            A_exp head = s->u.print.exps->u.pair.head;
            struct IntAndTable headT = interpExp(head,t);
            printf("%d ",headT.i);
            A_expList tail = s->u.print.exps->u.pair.tail;
            while(tail) {
                if(tail->kind == A_pairExpList) {
                    head = tail->u.pair.head;
                    headT = interpExp(head,headT.t);
                    printf("%d ",headT.i);
                    tail = tail->u.pair.tail;
                }else {
                    headT = interpExp(tail->u.last,headT.t);
                    printf("%d\n",headT.i);
                    break;
                }
            }
            return headT.t;
        }
    }
}


struct IntAndTable interpExp(A_exp e,Table_ t) {
    switch(e->kind) {
        case A_idExp: {
            int value = lookup(t,e->u.id);
            struct IntAndTable r = { value,update(t,e->u.id,value) };
            return r;
        }
        case A_numExp: {
            struct IntAndTable r = { e->u.num,update(t,"",e->u.num) };
            return r;
        }
        case A_opExp: {
            struct IntAndTable lhs = interpExp(e->u.op.left,t);
            struct IntAndTable rhs = interpExp(e->u.op.right,lhs.t);
            int value;
            switch(e->u.op.oper) {
                case A_plus: {
                    value = lhs.i + rhs.i;
                    break;
                }
                case A_minus: {
                    value = lhs.i - rhs.i;
                    break;
                }
                case A_times: {
                    value = lhs.i * rhs.i;
                    break;
                }
                case A_div: {
                    value = lhs.i / rhs.i;
                    break;
                }
            }
            struct IntAndTable r = { value,update(rhs.t,"",value) };
            return r;
        }
        case A_eseqExp: {
            interpStm(e->u.eseq.stm,t);
            return interpExp(e->u.eseq.exp,t);
        }
    }
}

void interp(A_stm s) {
    interpStm(s,NULL);
}