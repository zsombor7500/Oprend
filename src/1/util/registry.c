#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registry.h"
#include "entry.h"
#include "utils.h"
#include "e2.h"

registry* create_registry() {
    registry* r = malloc(sizeof(registry));
    if (r == NULL)
        return NULL;
    return r;
}

registry* init_registry() {
    registry* r = create_registry();
    if (r == NULL)
        return NULL;
    load_registry(r);
    return r;
}

e2* name_exists(e2* unique_names, char* name) {
    if (unique_names == NULL)
        return NULL;

    for (e2* e = unique_names->next; e->key != NULL; e = e->next)
        if (strcmp(e->key->name, name) == 0)
            return e;
    return NULL;
}

void decrement_trailing_ids(e2* p) {
    if (p == NULL)
        return;

    for (e2* e = p->next; e->key != NULL; e = e->next)
        --(e->key->id);
}

int get_highest_id(registry* r) {
    if (r == NULL || r->entries == NULL || r->entries->prev == NULL)
        return -1;
    if (r->entries->prev->key == NULL)
        return 0;
    return r->entries->prev->key->id;
}

e2* get_unique_names(registry* r) {
    e2* res = create_e2();
    if (res == NULL)
        return NULL;
    
    for (e2* e = r->entries->next; e->key != NULL; e = e->next) {
        if (e->key == NULL || e->key->name == NULL)
            return NULL;
        e2* exists = name_exists(res, e->key->name);
        if (exists == NULL) {
            e2* copy = create_e2_with_key(e->key);
            precede(copy, res);
        }
    }
    return res;
}

e2* get_element_by_id(registry* r, int id) {
    if (r == NULL || r->entries == NULL)
        return NULL;

    for (e2* e = r->entries->next; e->key != NULL; e = e->next)
        if (e->key->id == id)
            return e;
    return NULL;
}

entry* get_entry_by_id(registry* r, int id) {
    e2* res = get_element_by_id(r, id);
    if (res == NULL)
        return NULL;
    return res->key;
}

int reassign_entry_by_id(registry* r, int id, entry* new_data) {
    e2* res = get_element_by_id(r, id);
    if (res == NULL)
        return 1;

    entry** e = &(res->key);
    if (*e != NULL)
        free(*e);
    *e = new_data;
    return 0;
}

int delete_entry_by_id(registry* r, int id) {
    e2* res = get_element_by_id(r, id);
    if (res == NULL)
        return 1;
    del(res);
    return 0;
}

void free_registry(registry* r) {
    if (r == NULL)
        return;
    free_h2l_postorder(r->entries->next);
}

int save_registry(registry* r) {
    if (r == NULL || r->entries == NULL)
        return 1;

    FILE* file = fopen(REGISTRY_PATH, "w");
    if (file == NULL) {
        printf("Error while opening registry file to write");
        return 1;
    }

    entry* current;
    for (e2* e = r->entries->next; e->key != NULL; e = e->next)
    {
        current = e->key;
        int len = snprintf(NULL, 0, "%d$%s$%s\n", current->eggs, current->name, current->poem) + 1; /* +1 is the trailing '\0' */
        char* entry_str = malloc(len * sizeof(char));
        snprintf(entry_str, len, "%d$%s$%s\n", current->eggs, current->name, current->poem);

        fwrite(entry_str, sizeof(char), strlen(entry_str), file);
    }
    fclose(file);
    return 0;
}

int load_registry(registry* r) {
    FILE* file = fopen(REGISTRY_PATH, "r");
    if (file == NULL) {
        printf("Error while opening registry file to read");
        return 1;
    }
    
    r->entries = create_e2();
    if (r->entries == NULL)
        return 1;
        
    int id = 1;
    char* line;
    entry* e;
    while ((line = read_line(file)) != NULL) {
        e = NULL;
        if ((e = convert_to_entry(line, id)) == NULL) {
            free(line);
            free_h2l_postorder(r->entries);
            return 1;
        }
        free(line);
        
        e2* new_entry = create_e2();
        new_entry->key = e;
        if (new_entry == NULL) {
            return 1;
        }
        precede(new_entry, r->entries);
        ++id;
    }
    fclose(file);
    return 0;
}
