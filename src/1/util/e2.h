#ifndef REGISTRY_NODE_H
#define REGISTRY_NODE_H

#include "entry.h"

typedef struct e2 {
    entry* key;
    struct e2* prev;
    struct e2* next;
} e2;

/* creation */
e2*  create_e2();
e2*  create_e2_with_key(entry* key);

/* insertion, removal */
void precede(e2* q, e2* r);
void follow(e2* p, e2* q);
void unlink(e2* q);

/* deletion */
void del(e2* p);
void free_h2l_postorder(e2* p);

#endif