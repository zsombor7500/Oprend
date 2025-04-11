#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registry.h"
#include "entry.h"
#include "e2.h"

entry* convert_to_entry(char* str, int id) {
    char* endptr;
    char* token;
    char* context = NULL;
    
    // Eggs
    token = strtok_r(str, REGISTRY_DELIMITER, &context); // strtok_r is not necessary in this case, since there is no need for thread safety (no multithreading), but it is a good practice to use, I read
    if (token == NULL) {
        printf("Error parsing registry data! Number of eggs must be defined!\n");
        return NULL;
    }
    int eggs = strtol(token, &endptr, 10);
    if (*endptr != '\0') {
        printf("Error parsing registry data! Number of eggs must be of type integer!\n");
        return NULL;
    }
    if (eggs < 0) {
        printf("Error parsing registry data! Number of eggs must be a positive integer!\n");
        return NULL;
    }

    // Name
    token = strtok_r(NULL, REGISTRY_DELIMITER, &context);
    if (token == NULL) {
        printf("Error parsing registry data! Name must be defined!\n");
        return NULL;
    }
    char* name = malloc(strlen(token) * sizeof(char));
    strncpy(name, token, strlen(token));

    // Poem
    token = strtok_r(NULL, REGISTRY_DELIMITER, &context);
    if (token == NULL) {
        printf("Error parsing registry data! Poem must be defined!\n");
        return NULL;
    }
    if (strtok_r(NULL, REGISTRY_DELIMITER, &context) != NULL) {
        printf("Error parsing registry data! Extra columns were found after a poem!\n");
        return NULL;
    }
    char* poem = malloc(strlen(token) * sizeof(char));
    strncpy(poem, token, strlen(token));

    // Entry assembly
    entry* res = create_entry_with_params(id, name, poem, eggs);
    if (res == NULL) {
        return NULL;
    }
    return res;
}

char* read_line(FILE* stream) {
    size_t b_block_size = 64;
    size_t b_size = b_block_size;
    size_t len = 0;
    char* buffer = malloc(b_size * sizeof(char));
    
    if (buffer == NULL) {
        printf("Fatal error! Memory allocation failed!");
        return NULL;
    }

    while (fgets(buffer + len, b_size - len, stream)) {
        len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            return buffer;
        }

        b_size += b_block_size;
        char* temp = realloc(buffer, b_size * sizeof(char));
        if (temp == NULL) {
            free(buffer);
            printf("Fatal error! Memory reallocation failed!");
            return NULL;
        }
        buffer = temp;
    }
        
    if (len == 0) {
        free(buffer);
        return NULL;
    }
    return buffer;
}

char* receive_str(FILE* stream, const char* message) {
    char* response = NULL;
    do {
        printf("%s: ", message);
        if (response != NULL)
            free(response);
        response = read_line(stream);
    } while (response == NULL || !strcmp(response, ""));
    return response;
}

int receive_confirmation(FILE* stream, const char* message) {
    char postfix[] = "(y/n)";
    int len = snprintf(NULL, 0, "%s %s", message, postfix) + 1; /* +1 is the trailing '\0' */
    char* full_message = malloc(len * sizeof(char));
    snprintf(full_message, len, "%s %s", message, postfix);

    char* response = NULL;
    do {
        if (response != NULL)
            free(response);
        response = receive_str(stream, full_message);
    } while (strcmp(response, "y") && strcmp(response, "n"));
    int res = (strcmp(response, "y") == 0); // Otherwise it can only be equal to "n"
    free(full_message);
    free(response);
    return res;
}

long receive_long(FILE* stream, const char* message) {
    char* response = NULL;
    char* endptr;
    long res;
    do {
        if (response != NULL)
            free(response);
        response = receive_str(stream, message);
        res = strtol(response, &endptr, 10);
    } while (*endptr != '\0');
    free(response);
    return res;
}

void receive_modify_entry(FILE* stream, entry* e, int required_params) {
    if (e == NULL)
        return;

    if (P_INCNAME(required_params)) {
        char* name = receive_str(stream, "Enter the name");
        if (e->name == NULL)
            free(e->name);
        e->name = name;
    }

    if (P_INCPOEM(required_params)) {
        char* poem = receive_str(stream, "Enter the poem in a single line");
        if (e->poem == NULL)
            free(e->poem);
        e->poem = poem;
    }

    if (P_INCEGGS(required_params)) {
        int eggs = 0;
        do {
            eggs = receive_long(stream, "Enter the number of eggs (>=0)");
        } while (eggs < 0);
        e->eggs = eggs;
    }
}
