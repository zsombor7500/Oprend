#ifndef REGISTRAR_REGISTRY_H
#define REGISTRAR_REGISTRY_H

#include <stdio.h>
#include "entry.h"
#include "e2.h"

#define REGISTRY_PATH       "registry.dat"
#define REGISTRY_DELIMITER  "$"

typedef struct {
    e2* entries;
} registry;

/* creation */
registry* create_registry();

/* initialization */
registry* init_registry();

/* e2 h2l list operations */
e2*    name_exists(e2* unique_names, char* name);
void   decrement_trailing_ids(e2* p);

/* registry operations */
int    get_highest_id(registry* r);
e2*    get_unique_names(registry* r);
e2*    get_element_by_id(registry* r, int id);
entry* get_entry_by_id(registry* r, int id);
int    reassign_entry_by_id(registry* r, int id, entry* new_data);
int    delete_entry_by_id(registry* r, int id);
void   free_registry(registry* r);

/* registry I/O operations */
int    save_registry(registry* r);
int    load_registry(registry* r);

#endif
