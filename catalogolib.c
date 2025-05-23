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
    unsigned short pos_start = 0, pos_end = 0;
    char **parametri = malloc(sizeof(char *) * dim);

    if (parametri == NULL) {
        printf("Errore di allocazione memoria\n");
        return;
    }

    for (unsigned short i = 0; query[i] != '\0'; i++) {
        if (query[i] == DELIM) {
            //pos_end non può essere un numero negativo
            pos_end = ((i - 1) < 0) ? 0 : i - 1;
            
            // Calcola la lunghezza del parametro
            size_t len = pos_end - pos_start + 1;

            //se la lunghezza del parametro trovato è maggiore a 1, allora ci sono dati da recuperare (parametro + '\0')
            if (len > 1) {
                // Verifica se abbiamo bisogno di più spazio per i parametri
                if (num_param >= dim) {
                    dim++; // Incremento la capacità

                    // Usa un puntatore temporaneo per realloc
                    char **temp = realloc(parametri, dim * sizeof(char *));
                    if (temp == NULL) {
                        printf("Errore di allocazione memoria\n");
                        // Libera la memoria già allocata
                        for (unsigned short j = 0; j < num_param; j++) {
                            free(parametri[j]);
                        }

                        free(parametri);  // Non perdiamo il puntatore originale
                        return;
                    }
                    parametri = temp;  // Solo se realloc ha avuto successo
                }

                // Alloca memoria per questo parametro
                parametri[num_param] = malloc((len + 1) * sizeof(char));

                //se malloc non ha avuto successo
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
            else pos_start = i + 1;
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
            char **temp = realloc(parametri, dim * sizeof(char *));
            if (temp == NULL) {
                printf("Errore di allocazione memoria\n");
                // Libera la memoria già allocata prima di uscire dalla funzione
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