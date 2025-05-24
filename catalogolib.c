//
// Created by Graziano Semerano on 22/05/25.
//

#include "catalogolib.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gioco.h"
#include "utility.h"

unsigned short aggiungiGioco(gioco_t gioco) {

    //controlliamo se il gioco è già presente nel catalogo
    if (isAlredyAdded(&gioco) == 1) return 1;

    //apro il file in modalità scrittura
    FILE *file = fopen(NOME_FILE, "wb+");

    //rimuovere terminata la fase di sviluppo del programma
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDirectory attuale: %s\n", cwd);
    ///////////////////////

    if (file == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        exit(-1);
    }

    //scrivo il gioco nel file
    if (fwrite(&gioco, sizeof(gioco_t), 1, file) != 1) {
        fprintf(stderr, "Errore scrittura file\n");
        fclose(file);
        exit(-1);
    }

    printf("\nGioco aggiunto correttamente\n");
    //chiudo il file e salvo le modifiche
    fclose(file);
    return 0;
}

void modificaGioco() {

}

void cancellaGioco() {

}

unsigned short isAlredyAdded(gioco_t *new_gioco) {
    gioco_t gioco;
    //apro il file in modalità lettura
    FILE *file = fopen(NOME_FILE, "rb");

    //rimuovere terminata la fase di sviluppo del programma
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDirectory attuale: %s\n", cwd);
    ///////////////////////

    if (file == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        exit(-1);
    }

    //valido: 0 non presente nel catalogo (valido), 1 altrimenti
    unsigned short int valido = 0;

    while (fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        valido = gioco.anno_pubblicazione == new_gioco->anno_pubblicazione  &&
                 gioco.copie_vendute == new_gioco->copie_vendute            &&
                 strcmp(gioco.titolo, new_gioco->titolo) == 0               &&
                 strcmp(gioco.descrizione, new_gioco->descrizione) == 0     &&
                 strcmp(gioco.editore, new_gioco->editore) == 0             &&
                 strcmp(gioco.sviluppatore, new_gioco->sviluppatore) == 0 ? 1 : 0;

        for (unsigned short i = 0; i < MAX_GENERI; i++) {
            valido = strcmp(gioco.generi[i], new_gioco->generi[i]) == 0 ? 1 : 0;
        }

        if (valido == 1) break;
    }
    return valido;
}


//ricerca specifica sul file
void ricercaSpecifica(long offset, gioco_t *gioco) {
    //apro il file in modalità lettura
    FILE *file = fopen(NOME_FILE, "rb");

    //rimuovere terminata la fase di sviluppo del programma
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDirectory attuale: %s\n", cwd);
    ///////////////////////

    if (file == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        exit(-1);
    }

    //mi posizione in una posizione specifica
    if (fseek(file, offset, SEEK_SET) != 0) {
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

long *ricercaGlobale(char query[MAX_CHAR], unsigned short *num_elementi) {
    unsigned short num_param = 0, valido = 0;

    //trasforma la stringa inserita in input dall'utente tutta in minuscolo
    for (int i = 0; query[i]; i++) {
        query[i] = tolower((unsigned char)query[i]);
    }

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

    unsigned short capacita = 1;
    long *offset = calloc(capacita, sizeof(long));

    if (offset == NULL) {
        free(offset);
        printf("Errore di allocazione memoria\n");
        exit(-1);
    }

    //apro il file in modalità lettura
    FILE *file = fopen(NOME_FILE, "rb");

    //rimuovere terminata la fase di sviluppo del programma
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDirectory attuale: %s\n", cwd);
    ///////////////////////

    if (file == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        exit(-1);
    }

    long pos = ftello(file);
    while (fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        valido = 0;
        for (unsigned short i = 0; i < num_param; i++) {
            switch (parametri[i][0]) {
                case TOKEN_1: {
                    memmove(&parametri[i][0], &parametri[i][0 + 1], strlen(parametri[i]) - 0);
                    if (gioco.anno_pubblicazione == (typeof(gioco.anno_pubblicazione)) *parametri[i])
                        valido = 1;
                }
                    break;
                case TOKEN_2 : {
                    memmove(&parametri[i][0], &parametri[i][0 + 1], strlen(parametri[i]) - 0);
                    for (unsigned short j = 0; j < MAX_GENERI; j++) {
                        if (strstr(gioco.generi[j], parametri[i]) != NULL) {
                            valido = 1;
                            break;
                        }
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
            if (valido == 1) break;
        }
        if (valido == 1) {
            checkMemory(num_elementi, &capacita, sizeof(long), sizeof(typeof(offset)), (void ***)&offset);
            offset[*num_elementi] = pos;
            (*num_elementi)++;
            pos = ftello(file);
        }
    }
    return offset;
}