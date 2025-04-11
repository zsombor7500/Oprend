#include <stdio.h>
#include <stdlib.h>
#include "util/registry.h"
#include "actions.h"

int main(int argc, char** argv) {
    FILE* stream = stdin;

    display_motd();

    int action;
    registry* r = init_registry();
    do {
        display_menu();

        action = receive_action(stdin);
        switch (action)
        {
            case O_ADDENT:
                add_entry(stream, r);
                break;
            case O_MODENT:
                modify_entry(stream, r);
                break;
            case O_DELENT:
                delete_entry(stream, r);
                break;
            case O_LSTENT:
                list_all_entries(r);
                break;
            case O_LSTNAM:
                list_participants(r);
                break;
            case O_ACTEXT:
                printf("Exit option choosen!\nExiting...\n");
                cleanup(r);
                exit(0);
            default: // e.g.: O_ACTERR, or any other int than those available in macros
                printf("Unexpected flag received!");
                cleanup(r);
                exit(1);
        }
    } while (!O_ISACTEXT(action));
}