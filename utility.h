//
// Created by Giuseppe on 23/05/2025.
//

#ifndef CATALOGO_UTILITY_H
#define CATALOGO_UTILITY_H

#include "catalogolib.h"
#include <stdio.h>

#ifndef DELIM
#define DELIM ","
#endif

#ifndef TOKEN_1
#define TOKEN_1 '$'
#endif

#ifndef TOKEN_2
#define TOKEN_2 '#'
#endif

char** analisiQuery(char query[MAX_CHAR], unsigned short *param);

void trim(char *token);

int checkMemory(const unsigned short *num_elementi, unsigned short *capacita,
    unsigned long dimensione_elemento, unsigned long dimensione_puntatore, void ***array);

gioco_t inserisciGioco();

FILE *apriCatalogo(char mode[3]);

#endif //CATALOGO_UTILITY_H
