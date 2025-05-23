//
// Created by Giuseppe on 23/05/2025.
//

#include "utility.h"
#include "gioco.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **analisiQuery(char query[MAX_CHAR], unsigned short *param) {
    unsigned short dim = 1, new_param = *param;
    char **parametri = malloc(sizeof(char *) * dim);

    if (parametri == NULL) {
        printf("Errore di allocazione memoria\n");
        exit(-1);
    }

    // Crea una copia della stringa query per strtok
    char query_copy[MAX_CHAR];
    strncpy(query_copy, query, MAX_CHAR);
    query_copy[MAX_CHAR - 1] = '\0'; // Assicura terminazione

    char *token = strtok(query_copy, ",");

    while (token != NULL) {
        char token_copy[MAX_CHAR];
        strncpy(token_copy, token, MAX_CHAR);
        token_copy[MAX_CHAR - 1] = '\0';

        // Rimuovi spazi all'inizio e alla fine
        trim(token_copy);

        unsigned short len = strlen(token_copy);

        // Se il token non è vuoto dopo il trimming
        if (len > 0) {
            checkMemory(&new_param, &dim, &len, &parametri);

            // Copia il parametro
            strncpy(parametri[new_param], token_copy, len);
            parametri[new_param][len] = '\0'; // Termina la stringa

            new_param++;
        }

        token = strtok(NULL, DELIM);
    }
    *param = new_param;
    return parametri;
}

void trim(char *token) {
    char *start = token;
    // Rimuove spazi iniziali
    while (isspace((unsigned char)*start)) start++;

    // Se la stringa è vuota dopo il trimming iniziale
    if (*start == '\0') {
        *token = '\0';
        return;
    }

    // Rimuove spazi finali
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    // Sposta la stringa trimmata all'inizio del buffer
    if (start != token) {
        memmove(token, start, strlen(start) + 1);
    }

    if (token[0] == TOKEN_1 || token[0] == TOKEN_2) {
        unsigned short j = 1;
        while (isspace((unsigned char)token[j])) {
            j++;
        }
        memmove(&token[1], &token[j], strlen(&token[j]) + 1);
    }
}

void checkMemory(const unsigned short *num_param, unsigned short *dim, const unsigned short *len, char ***parametri) {
    // Verifica se abbiamo bisogno di più spazio per i parametri
    if (*num_param >= *dim) {
        (*dim)++; // Incremento la capacità

        // Usa un puntatore temporaneo per realloc
        char **temp = realloc(*parametri, (*dim) * sizeof(char *));
        if (temp == NULL) {
            printf("Errore di allocazione memoria\n");
            // Libera la memoria già allocata
            for (unsigned short j = 0; j < *num_param; j++) {
                free((*parametri)[j]);
            }

            free(*parametri);  // Non perdiamo il puntatore originale
            return;
        }
        *parametri = temp;  // Solo se realloc ha avuto successo
    }

    // Alloca memoria per questo parametro
    (*parametri)[*num_param] = malloc(((*len) + 1) * sizeof(char));

    //se malloc non ha avuto successo
    if ((*parametri)[*num_param] == NULL) {
        printf("Errore di allocazione memoria\n");
        // Libera la memoria già allocata
        for (unsigned short j = 0; j < *num_param; j++) {
                free((*parametri)[j]);
        }
        free(*parametri);
        return;
    }
}