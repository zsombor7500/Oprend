#include <stdio.h>
#include <stdlib.h>
#include "entry.h"

entry* create_entry() {
    entry* e = malloc(sizeof(entry));
    if (e == NULL) {
        printf("Fatal error! Memory reallocation failed!");
        return NULL;
    }
    return e;
}

entry* create_entry_with_id(int id) {
    if (id < 0) {
        printf("Invalid argument(s) when creating new entry!\n");
        return NULL;
    }
    entry* e = create_entry();
    if (e == NULL)
        return NULL;
    e->id = id;
    return e;
}

entry* create_entry_with_params(int id, char* name, char* poem, int eggs) {
    if (id < 0 || eggs < 0 || name == NULL || poem == NULL) {
        printf("Invalid argument(s) when creating new entry!\n");
        return NULL;
    }
    entry* e = create_entry();
    if (e == NULL)
        return NULL;
    e->id = id;
    e->name = name;
    e->poem = poem;
    e->eggs = eggs;
    return e;
}

void free_entry(entry* e) {
    free(e->name);
    free(e->poem);
    free(e);
}
