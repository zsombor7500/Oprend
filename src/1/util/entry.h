#ifndef REGISTRAR_ENTRY_H
#define REGISTRAR_ENTRY_H

/* entry parameter flags */
#define P_EGGS    0001    /* eggs parameter */
#define P_NAME    0002    /* name parameter */
#define P_POEM    0004    /* poem parameter */

#define P_ALL     (P_EGGS|P_NAME|P_POEM)    /* bitwise OR on all available parameter flags */

/* entry parameter flag checks (is) */
#define P_ISEGGS(f)    (((f) & P_ALL) == P_EGGS)    /* eggs parameter */
#define P_ISNAME(f)    (((f) & P_ALL) == P_NAME)    /* name parameter */
#define P_ISPOEM(f)    (((f) & P_ALL) == P_POEM)    /* poem parameter */
/* entry parameter flag checks (includes) */
#define P_INCEGGS(f)    ((f) & P_EGGS)    /* eggs parameter */
#define P_INCNAME(f)    ((f) & P_NAME)    /* name parameter */
#define P_INCPOEM(f)    ((f) & P_POEM)    /* poem parameter */

typedef struct {
    int     id;
    int     eggs;
    char*   name;
    char*   poem;
} entry;

/* creation, deletion */
entry* create_entry();
entry* create_entry_with_id(int id);
entry* create_entry_with_params(int id, char* name, char* poem, int eggs);
void   free_entry(entry* e);

#endif
