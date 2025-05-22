//
// Created by Graziano Semerano on 22/05/25.
//

#ifndef GIOCO_H
#define GIOCO_H

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
    char generi[MAX_CHAR];
}gioco_t;

#endif //GIOCO_H
