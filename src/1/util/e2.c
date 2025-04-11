#include <stdio.h>
#include <stdlib.h>
#include "entry.h"
#include "e2.h"

e2* create_e2() {
    e2* e = malloc(sizeof(e2));
    if (e == NULL) {
        printf("Fatal error! Memory allocation failed!");
        return NULL;
    }
    e->key = NULL;
    e->prev = e->next = e;
    return e;
}

e2* create_e2_with_key(entry* key) {
    e2* e = create_e2();
    if (e == NULL)
        return NULL;
    e->key = key;
    return e;
}

void precede(e2* q, e2* r) {
    if (q == NULL || r == NULL || r->prev == NULL)
        return;
    e2* p = r->prev;
    q->prev = p;
    q->next = r;
    p->next = r->prev = q;
}

void follow(e2* p, e2* q) {
    if (p == NULL || q == NULL || p->next == NULL)
        return;
    e2* r = p->next;
    q->prev = p;
    q->next = r;
    p->next = r->prev = q;
}

void unlink(e2* q) {
    if (q == NULL || q->prev == NULL || q->next == NULL)
        return;
    e2* p = q->prev;
    e2* r = q->next;
    p->next = r;
    r->prev = p;
    q->prev = q->next = q;
}

void del(e2* q) {
    if (q == NULL)
        return;
    unlink(q);
    if (q->key != NULL)
        free_entry(q->key);
    free(q);
}

void free_h2l_postorder(e2* p) {
    if (p == NULL || p->key == NULL)
        return;
    if (p->next != NULL)
        free_h2l_postorder(p->next);
    del(p);
}