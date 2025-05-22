//
// Created by Graziano Semerano on 22/05/25.
//

#include "catalogolib.h"

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

void ricerca(const unsigned short *offset, gioco_t *gioco) {
    //ricerca specifica sul file

    //apro il file in modalità lettura
    FILE *file = fopen(NOME_FILE, "rb");
    //mi posizione in una posizione specifica
    fseek(file, *offset, SEEK_SET);
    //leggo il dato dal catalogo e lo salvo
    fread(gioco, sizeof(gioco_t), 1, file);
    //chiudo il file
    fclose(file);

    if (gioco == NULL) exit(-1);
}

void ricerca() {

}

void analisiQuery(char query[MAX_CHAR]) {
    unsigned short num_param = 0, x = 0, dim = 1;
    unsigned short token_pr, pos_start = 0, pos_end = 0;
    char **parametri = malloc(sizeof(char) * dim);

    for (unsigned short i = 0; query[i] != '\0'; i++) {
        if (x == 0) {
            if (strcmp(query[pos_start], TOKEN_1) == 0 || strcmp(query[pos_start], TOKEN_2) == 0)
                x = 1;
        }
        if (strcmp(query[i], DELIM) == 0) {
            pos_end = i - 1;
            num_param++;
            // Calcola la lunghezza del parametro
            size_t len = pos_end - pos_start + 1;

            // Verifica se abbiamo bisogno di più spazio per i parametri
            if (num_param >= dim) {
                dim++; // Incremento la capacità
                char **temp = realloc(parametri, dim * sizeof(char*));
                if (temp == NULL) {
                    printf("Errore di allocazione memoria\n");
                    // Libera la memoria già allocata
                    for (unsigned short j = 0; j < num_param; j++) {
                        free(parametri[j]);
                    }
                    free(parametri);
                    return;
                }
                parametri = temp;
            }

            // Alloca memoria per questo parametro
            parametri[num_param] = malloc((len + 1) * sizeof(char));
            if (parametri[num_param] == NULL) {
                printf("Errore di allocazione memoria\n");
                // Libera la memoria già allocata
                for (unsigned short j = 0; j < num_param; j++) {
                    free(parametri[j]);
                }
                free(parametri);
                return;
            }

            // Copia il parametro
            strncpy(parametri[num_param], &query[pos_start], len);
            parametri[num_param][len] = '\0'; // Termina la stringa

            num_param++;

            // Aggiorna la posizione di inizio per il prossimo parametro
            pos_start = i + 1;
        }
    }

    // Gestisci l'ultimo parametro
    if (query[0] != '\0' && pos_start < strlen(query)) {
        pos_end = strlen(query) - 1;

        // Calcola la lunghezza dell'ultimo parametro
        size_t len = pos_end - pos_start + 1;

        // Verifica se abbiamo bisogno di più spazio
        if (num_param >= dim) {
            dim++;
            char **temp = realloc(parametri, dim * sizeof(char*));
            if (temp == NULL) {
                printf("Errore di allocazione memoria\n");
                for (unsigned short j = 0; j < num_param; j++) {
                    free(parametri[j]);
                }
                free(parametri);
                return;
            }
            parametri = temp;
        }

        // Alloca memoria per l'ultimo parametro
        parametri[num_param] = malloc((len + 1) * sizeof(char));
        if (parametri[num_param] == NULL) {
            printf("Errore di allocazione memoria\n");
            for (unsigned short j = 0; j < num_param; j++) {
                free(parametri[j]);
            }
            free(parametri);
            return;
        }

        // Copia l'ultimo parametro
        strncpy(parametri[num_param], &query[pos_start], len);
        parametri[num_param][len] = '\0';

        num_param++;
    }

    printf("Numero totale di parametri: %d\n", num_param);

    for (unsigned short i = 0; i < num_param; i++) {
        printf("Parametro %d: %s\n", i + 1, parametri[i]);
    }
}
