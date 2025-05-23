//
// Created by Graziano Semerano on 22/05/25.
//

#include "catalogolib.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gioco.h"

void aggiungiGioco() {
}

void modificaGioco() {
}

void cancellaGioco() {
}

//ricerca specifica sul file
void ricercaSpecifica(const unsigned short *offset, gioco_t *gioco) {
    //apro il file in modalità lettura
    FILE *file = fopen(NOME_FILE, "rb");

    if (file == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        exit(-1);
    }

    //mi posizione in una posizione specifica
    if (fseek(file, *offset, SEEK_SET) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        exit(-1);
    }

    //leggo il dato dal catalogo e lo salvo
    if (fread(gioco, sizeof(gioco_t), 1, file) != 1) {
        fprintf(stderr, "Errore lettura file\n");
        fclose(file);
        exit(-1);
    }

    //chiudo il file
    fclose(file);
}

void ricercaGlobale() {

}

void analisiQuery(char query[MAX_CHAR]) {
    unsigned short num_param = 0, dim = 1;
    char **parametri = malloc(sizeof(char *) * dim);

    if (parametri == NULL) {
        printf("Errore di allocazione memoria\n");
        return;
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
        
        size_t len = strlen(token_copy);
        
        // Se il token non è vuoto dopo il trimming
        if (len > 0) {
            checkMemory(&num_param, &dim, &len, &parametri);
            
            // Copia il parametro
            strncpy(parametri[num_param], token_copy, len);
            parametri[num_param][len] = '\0'; // Termina la stringa

            num_param++;
        }

        token = strtok(NULL, ",");
    }

    ///////////////////////
    // Rimuovere terminata la fase di sviluppo del programma
    printf("Numero totale di parametri: %d\n", num_param);

    for (unsigned short i = 0; i < num_param; i++) {
        printf("Parametro %d: %s\n", i + 1, parametri[i]);
    }
    ///////////////////////

    // Libera tutta la memoria allocata
    for (unsigned short i = 0; i < num_param; i++) {
        free(parametri[i]);
    }
    free(parametri);
}

void trim(char *query) {
    char *start = query;
    // Rimuove spazi iniziali
    while (isspace((unsigned char)*start)) start++;
    
    // Se la stringa è vuota dopo il trimming iniziale
    if (*start == '\0') {
        *query = '\0';
        return;
    }
    
    // Rimuove spazi finali
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    
    // Sposta la stringa trimmata all'inizio del buffer
    if (start != query) {
        memmove(query, start, strlen(start) + 1);
    }
    
    // Gestione speciale dei token
    if ((query[0] == TOKEN_1 || query[0] == TOKEN_2) && strlen(query) > 1 && query[1] == ' ') {
        memmove(&query[1], &query[2], strlen(&query[2]) + 1);
    }
}

void checkMemory(const unsigned short *num_param, unsigned short *dim, const size_t *len, char ***parametri) {
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