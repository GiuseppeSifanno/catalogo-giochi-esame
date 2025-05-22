//
// Created by Graziano Semerano on 22/05/25.
//

#ifndef GIOCO_H
#define GIOCO_H

#include <stdint.h>

#ifndef MAX_CHAR
#define MAX_CHAR 255
#endif

#ifndef MAX_GENERI
#define MAX_GENERI 10
#endif

typedef struct recensioni {
    char nome_utente[MAX_CHAR];
    uint8_t valutazione;
    char descrizione [MAX_CHAR];
}recensioni_t;

typedef struct gioco {
    char titolo [MAX_CHAR];
    char editore [MAX_CHAR];
    char sviluppatore [MAX_CHAR];
    char descrizione [MAX_CHAR];
    unsigned short anno_pubblicazione;
    unsigned long copie_vendute;
    char generi[MAX_GENERI][MAX_CHAR];
}gioco_t;

#endif GIOCO_H
