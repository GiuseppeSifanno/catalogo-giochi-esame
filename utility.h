//
// Created by Giuseppe on 23/05/2025.
//

#ifndef CATALOGO_UTILITY_H
#define CATALOGO_UTILITY_H

#include "gioco.h"

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

void checkMemory(const unsigned short *num_param, unsigned short *dim, const unsigned short *len, char ***parametri);

#endif //CATALOGO_UTILITY_H
