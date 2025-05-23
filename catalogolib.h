//
// Created by Graziano Semerano on 22/05/25.
//
#ifndef CATALOGOLIB_H
#define CATALOGOLIB_H

#include "gioco.h"

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

#ifndef DELIM
#define DELIM ','
#endif

#ifndef TOKEN_1
#define TOKEN_1 '$'
#endif

#ifndef TOKEN_2
#define TOKEN_2 '#'
#endif

void aggiungiGioco();

void modificaGioco();

void cancellaGioco();

void ricercaSpecifica(const unsigned short *offset, gioco_t *gioco);

void ricercaGlobale();

void analisiQuery(char query[MAX_CHAR]);

#endif //CATALOGOLIB_H
