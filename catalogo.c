#include <stdio.h>
#include "catalogolib.h"


int main(void) {

    char query[MAX_CHAR] = "#fps";

    for (int i = 0; i < 3; i++) {
        aggiungiGioco(inserisciGioco());
    }

    unsigned short num_elementi = 0;
    long* offset = ricercaGlobale(query, &num_elementi);
    gioco_t gioco;
    for (int i = 0; i < num_elementi; i++) {
        printf("%lo\n", offset[i]);
        ricercaSpecifica(offset[i], &gioco);
        printf("%s\n", gioco.titolo);
        printf("%s\n", gioco.generi[0]);
    }
    return 0;
}
