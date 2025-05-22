//
// Created by Graziano Semerano on 22/05/25.
//
#ifndef CATALOGOLIB_H
#define CATALOGOLIB_H

#include <stdint.h>

#ifndef PATH
/**
 * Percorso in cui verrà posizionato il file
 */
#define PATH "./"
#endif


#ifndef NOMEFILE
/**
 * Nome del file del catalogo
 */
#define NOME_FILE "catalogo"
#endif

#ifndef MAX_CHAR
#define MAX_CHAR 255
#endif



void aggiungiGioco();

void modificaGioco();

void cancellaGioco();

void ricerca();

void analisiQuery();

#endif //CATALOGOLIB_H
