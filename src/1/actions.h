#ifndef REGISTRAR_ACTIONS_H
#define REGISTRAR_ACTIONS_H

#include <stdio.h>
#include "util/registry.h"

/* action flags */
#define O_ACTERR    0001    /* action parsing error (no available action choosen) flag */
#define O_ADDENT    0002    /* entry adding action flag */
#define O_MODENT    0004    /* entry modification action flag */
#define O_DELENT    0010    /* entry deletion action flag */
#define O_LSTENT    0020    /* entry listing action flag */
#define O_LSTNAM    0040    /* unique name listing flag */
#define O_ACTEXT    0100    /* exit action flag */

#define O_ACTALL    (O_ACTERR|O_ADDENT|O_MODENT|O_DELENT|O_LSTENT|O_LSTNAM|O_ACTEXT)    /* bitwise OR on all available action flags */

/* action flag checks */
#define O_ISACTERR(f)    (((f) & O_ACTALL) == O_ACTERR)    /* action parsing error */
#define O_ISADDENT(f)    (((f) & O_ACTALL) == O_ADDENT)    /* entry adding action */
#define O_ISMODENT(f)    (((f) & O_ACTALL) == O_MODENT)    /* entry modification action */
#define O_ISDELENT(f)    (((f) & O_ACTALL) == O_DELENT)    /* entry deletion action */
#define O_ISLSTENT(f)    (((f) & O_ACTALL) == O_LSTENT)    /* entry listing action */
#define O_ISLSTNAM(f)    (((f) & O_ACTALL) == O_LSTNAM)    /* unique name listing action */
#define O_ISACTEXT(f)    (((f) & O_ACTALL) == O_ACTEXT)    /* exit action */

/* data display */
void display_motd();
void display_menu();

/* input handling */
int receive_action(FILE* stream);

/* actions */
void add_entry(FILE* stream, registry* r);
void modify_entry(FILE* stream, registry* r/* , int index, entry* e */);
void delete_entry(FILE* stream, registry* r/* , int index */);
void list_all_entries(registry* r);
void list_participants(registry* r);
void cleanup(registry* r);

#endif