//
// Created by Graziano Semerano on 22/05/25.
//

#include "catalogolib.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "gioco.h"
#include "utility.h"

void aggiungiGioco(gioco_t *gioco) {
    //apro il file in modalità lettura
    FILE *file = fopen(NOME_FILE, "wb");

    if (file == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        exit(-1);
    }

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
    analisiQuery(query);
}