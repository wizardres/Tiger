#include "util.h"

void *checked_malloc(int size) {
    void *p = malloc(size);
    assert(p);
    return p;
}

string String(char *p) {
    if(!p) return NULL;
    int len = strlen(p) + 1;
    string str = checked_malloc(len);
    strncpy(str,p,len);
    str[len-1] = '\0';
    return str;
}

U_boolList U_BoolList(bool head,U_boolList tail) {
    U_boolList list = checked_malloc(sizeof(*list));
    list->head = head;
    list->tail = tail;
    return list;
}