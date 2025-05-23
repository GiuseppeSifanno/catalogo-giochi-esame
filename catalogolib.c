//
// Created by Graziano Semerano on 22/05/25.
//

#include "catalogolib.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gioco.h"
#include "utility.h"

void aggiungiGioco(const gioco_t *gioco) {
    //apro il file in modalità scrittura
    FILE *file = fopen(NOME_FILE, "wb");

    if (file == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        exit(-1);
    }

    //scrivo il gioco nel file
    if (fwrite(gioco, sizeof(gioco_t), 1, file) != 1) {
        fprintf(stderr, "Errore scrittura file\n");
        fclose(file);
        exit(-1);
    }

    printf("\nGioco aggiunto correttamente\n");
    //chiudo il file e salvo le modifiche
    fclose(file);
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

void ricercaGlobale(char query[MAX_CHAR]) {
    unsigned short num_param = 0, valido = 0;
    gioco_t gioco;
    //parametri recuperati durante l'analisi della query in input dell'utente
    char **parametri = analisiQuery(query, &num_param);

    if (parametri == NULL) {
        printf("Errore di analisi query\n");
        exit(-1);
    }

    ///////////////////////
    // Rimuovere terminata la fase di sviluppo del programma
    printf("Numero totale di parametri: %d\n", num_param);

    for (unsigned short i = 0; i < num_param; i++) {
        printf("Parametro %d: %s\n", i + 1, parametri[i]);
    }
    ///////////////////////

    //apro il file in modalità lettura
    FILE *file = fopen(NOME_FILE, "rb");

    if (file == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        exit(-1);
    }

    while (fread(&gioco, sizeof(gioco_t), 1, file) != EOF) {
        valido = 0;
        for (unsigned short i = 0; i < num_param; i++) {
            switch (parametri[i][0]) {
                case TOKEN_1: {
                    if (gioco.anno_pubblicazione != (typeof(gioco.anno_pubblicazione)) *parametri[i]) break;
                    valido = 1;
                }
                    break;
                case TOKEN_2 : {
                    for (unsigned short j = 0; j < MAX_GENERI; j++) {
                        if (strstr(gioco.generi[j], parametri[i]) != NULL) {
                            valido = 1;
                            break;
                        }
                        if (valido == 1) break;
                    }
                }
                    break;
                default: {
                    if (strstr(gioco.titolo , parametri[i]) != NULL) valido = 1;
                    if (strstr(gioco.editore , parametri[i]) != NULL) valido = 1;
                    if (strstr(gioco.sviluppatore , parametri[i]) != NULL) valido = 1;
                }
                    break;
            }
            if (valido == 0) break;
        }
    }
}