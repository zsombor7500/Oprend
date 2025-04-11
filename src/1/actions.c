#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/registry.h"
#include "util/utils.h"
#include "actions.h"

void display_motd() {
    printf("▄▄▌ ▐ ▄▌▄▄▄ .▄▄▌   ▄▄·       • ▌ ▄ ·. ▄▄▄ .                                                           \n"
           "██· █▌▐█▀▄.▀·██•  ▐█ ▌▪▪     ·██ ▐███▪▀▄.▀·                                                           \n"
           "██▪▐█▐▐▌▐▀▀▪▄██▪  ██ ▄▄ ▄█▀▄ ▐█ ▌▐▌▐█·▐▀▀▪▄                                                           \n"
           "▐█▌██▐█▌▐█▄▄▌▐█▌▐▌▐███▌▐█▌.▐▌██ ██▌▐█▌▐█▄▄▌                                                           \n"
           " ▀▀▀▀ ▀▪ ▀▀▀ .▀▀▀ ·▀▀▀  ▀█▄▀▪▀▀  █▪▀▀▀ ▀▀▀                                                            \n"
           "            ▄▄▄▄▄          ▄▄▄▄▄ ▄ .▄▄▄▄ .                                                            \n"
           "            •██  ▪         •██  ██▪▐█▀▄.▀·                                                            \n"
           "             ▐█.▪ ▄█▀▄      ▐█.▪██▀▐█▐▀▀▪▄                                                            \n"
           "             ▐█▌·▐█▌.▐▌     ▐█▌·██▌▐▀▐█▄▄▌                                                            \n"
           "             ▀▀▀  ▀█▄▀▪     ▀▀▀ ▀▀▀ · ▀▀▀                                                             \n"
           "                                    ▄▄▄  ▄▄▄ . ▄▄ • ▪  .▄▄ · ▄▄▄▄▄▄▄▄   ▄· ▄▌                         \n"
           "                                    ▀▄ █·▀▄.▀·▐█ ▀ ▪██ ▐█ ▀. •██  ▀▄ █·▐█▪██▌                         \n"
           "                                    ▐▀▀▄ ▐▀▀▪▄▄█ ▀█▄▐█·▄▀▀▀█▄ ▐█.▪▐▀▀▄ ▐█▌▐█▪                         \n"
           "                                    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌▐█▄▪▐█ ▐█▌·▐█•█▌ ▐█▀·.                         \n"
           "                                    .▀  ▀ ▀▀▀ ·▀▀▀▀ ▀▀▀ ▀▀▀▀  ▀▀▀ .▀  ▀  ▀ •                          \n"
           "                                                            • ▌ ▄ ·.  ▄▄▄·  ▐ ▄  ▄▄▄·  ▄▄ • ▄▄▄ .▄▄▄  \n"
           "                                                            ·██ ▐███▪▐█ ▀█ •█▌▐█▐█ ▀█ ▐█ ▀ ▪▀▄.▀·▀▄ █·\n"
           "                                                            ▐█ ▌▐▌▐█·▄█▀▀█ ▐█▐▐▌▄█▀▀█ ▄█ ▀█▄▐▀▀▪▄▐▀▀▄ \n"
           "                                                            ██ ██▌▐█▌▐█ ▪▐▌██▐█▌▐█ ▪▐▌▐█▄▪▐█▐█▄▄▌▐█•█▌\n"
           "                                                            ▀▀  █▪▀▀▀ ▀  ▀ ▀▀ █▪ ▀  ▀ ·▀▀▀▀  ▀▀▀ .▀  ▀\n");
}

void display_menu() {
    printf("\n __\n"
           " \\ \\\n"
           "  \\ \\   Options:\n"
           "  / /\n"
           " /_/        ~ 1 - Add entry\n"
           "            ~ 2 - Modify entry\n"
           "            ~ 3 - Delete entry\n"
           "            ~ 4 - List entries\n"
           "            ~ 5 - List unique participant names\n"
           "            ~ 6 - Exit\n\n");
}

int receive_action(FILE* stream) {
    int res_flag = 0;
    char message[] = "Enter an option's index (1-6)";
    do {
        int res = receive_long(stream, message);
        switch (res)
        {
            case 1:
                res_flag = O_ADDENT;
                break;
            case 2:
                res_flag = O_MODENT;
                break;
            case 3:
                res_flag = O_DELENT;
                break;
            case 4:
                res_flag = O_LSTENT;
                break;
            case 5:
                res_flag = O_LSTNAM;
                break;
            case 6:
                res_flag = O_ACTEXT;
                break;
            default:
                printf("Incorrect menu option, try again!\n");
                res_flag = O_ACTERR;
                break;
        }
    } while (O_ISACTERR(res_flag));
    return res_flag;
}

void add_entry(FILE* stream, registry* r) {
    if (r == NULL || r->entries == NULL) {
        printf("Entry adding cancelled!\n");
        return;
    }

    char message[] = "Are you sure you want to add the new entry?";
    entry* new_entry = create_entry_with_id(get_highest_id(r) + 1);
    receive_modify_entry(stream, new_entry, P_NAME|P_POEM|P_EGGS);
    if (receive_confirmation(stream, message) == 0) {
        printf("Entry adding cancelled!\n");
        return;
    }

    e2* new_e2 = create_e2_with_key(new_entry);
    precede(new_e2, r->entries);
    printf("Entry added successfully!\n");

    if (save_registry(r) == 1) {
        printf("Updated registry failed to save!\n");
        return;
    }
    printf("Updated registry was saved successfully!\n");
}

void modify_entry(FILE* stream, registry* r) {
    if (r == NULL)
        return;

    char message[] = "Enter the ID of the entry you'd like to modify";
    int highest_id = get_highest_id(r);
    int len = snprintf(NULL, 0, "%s (1-%d)", message, highest_id) + 1; /* +1 is the trailing '\0' */
    char* full_message = malloc(len * sizeof(char));
    snprintf(full_message, len, "%s (1-%d)", message, highest_id);
    int rem_id;
    do {
        rem_id = receive_long(stream, full_message);
    } while (rem_id < 1);
    free(full_message);

    e2* mod_e = get_element_by_id(r, rem_id);
    if (mod_e == NULL) {
        printf("No element exists with given ID, modification cancelled!");
        return;
    }
    if (mod_e->key == NULL)
        return;

    int req_params = 0;
    if (receive_confirmation(stream, "Would you like to change the name of the entry?") == 1)
        req_params |= P_NAME;
    if (receive_confirmation(stream, "Would you like to change the poem of the entry?") == 1)
        req_params |= P_POEM;
    if (receive_confirmation(stream, "Would you like to change the number of eggs of the entry?") == 1)
        req_params |= P_EGGS;

    if (req_params == 0)
        return;
    receive_modify_entry(stream, mod_e->key, req_params);

    printf("Removal was successful!\n");
    if (save_registry(r) == 1) {
        printf("Updated registry failed to save!\n");
        return;
    }
    printf("Updated registry was saved successfully!\n");
}

void delete_entry(FILE* stream, registry* r) {
    if (r == NULL)
        return;

    char message[] = "Enter the ID of the entry you'd like to remove";
    int highest_id = get_highest_id(r);
    int len = snprintf(NULL, 0, "%s (1-%d)", message, highest_id) + 1; /* +1 is the trailing '\0' */
    char* full_message = malloc(len * sizeof(char));
    snprintf(full_message, len, "%s (1-%d)", message, highest_id);
    int rem_id;
    do {
        rem_id = receive_long(stream, full_message);
    } while (rem_id < 1);
    free(full_message);
    
    e2* rem_e = get_element_by_id(r, rem_id);
    if (rem_e == NULL) {
        printf("No element exists with given ID, removal cancelled!");
        return;
    }
    char message2[] = "Are you sure you want to remove the entry?";
    if (receive_confirmation(stream, message2) == 0) {
        printf("Entry adding cancelled!\n");
        return;
    }
    printf("Proceeding with removal!\n");
    decrement_trailing_ids(rem_e);
    del(rem_e);

    printf("Removal was successful!\n");
    if (save_registry(r) == 1) {
        printf("Updated registry failed to save!\n");
        return;
    }
    printf("Updated registry was saved successfully!\n");
}

void list_all_entries(registry* r) {
    if (r == NULL || r->entries == NULL) {
        printf("Registry entries could not be listed, registry or the entries do not exist!");
        return;
    }

    printf("+---------------------------------\n"
           "| Entries in registry:\n"
           "+---------------------------------\n");
    for (e2* e = r->entries->next; e->key != NULL; e = e->next)
    {
        entry* current = e->key;
        printf("| ID: %d\n", current->id);
        printf("| Name: %s\n", current->name);
        printf("| Number of eggs: %d\n", current->eggs);
        printf("| Poem: \"%s\"\n|\n", current->poem);
        printf("+---------------------------------\n");
    }
}

void list_participants(registry* r) {
    e2* unique_names = get_unique_names(r);
    if (unique_names == NULL) {
        printf("Couldn't retrieve participant names.");
        return;
    }

    printf("+---------------------------------\n"
           "| List of participants:\n"
           "+---------------------------------\n");
    int i = 1;
    for (e2* e = unique_names->next; e->key != NULL; e = e->next)
    {
        entry* current = e->key;
        printf("| %d. %s\n", i, current->name);
        ++i;
    }
    printf("+---------------------------------\n");
}

void cleanup(registry* r) {
    free_registry(r);
}
