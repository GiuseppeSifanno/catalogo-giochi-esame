//
// Created by Graziano Semerano on 22/05/25.
//
#ifndef CATALOGOLIB_H
#define CATALOGOLIB_H

#include "gioco.h"
#include "utility.h"

#ifndef NOME_FILE
/**
 * Nome del file del catalogo
 */
#define NOME_FILE "catalogo.dat"
#endif

unsigned short aggiungiGioco(gioco_t gioco);

void modificaGioco();

void cancellaGioco();

/**
 * Ritorna 1 se il gioco è già presente nel catalogo, 0 altrimenti
 * @param new_gioco
 * @return Valido
 */
unsigned short isAlredyAdded(gioco_t *new_gioco);

void ricercaSpecifica(long offset, gioco_t *gioco);

long *ricercaGlobale(char query[MAX_CHAR], unsigned short *num_elementi);

#endif //CATALOGOLIB_H
