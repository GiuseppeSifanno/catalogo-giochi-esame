//
// Created by Graziano Semerano on 22/05/25.
//
#ifndef CATALOGOLIB_H
#define CATALOGOLIB_H

#include "gioco.h"
#include "utility.h"

#ifndef PATH
/**
 * Percorso in cui verrà posizionato il file
 */
#define PATH "./"
#endif

#ifndef NOME_FILE
/**
 * Nome del file del catalogo
 */
#define NOME_FILE "catalogo"
#endif

void aggiungiGioco(gioco_t *gioco);

void modificaGioco();

void cancellaGioco();

void ricercaSpecifica(const unsigned short *offset, gioco_t *gioco);

void ricercaGlobale(char query[MAX_CHAR]);

#endif //CATALOGOLIB_H
