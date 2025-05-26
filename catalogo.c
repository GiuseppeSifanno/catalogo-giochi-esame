#include <stdio.h>
#include "catalogolib.h"


int main(void) {

    char query[MAX_CHAR] = "#fps";
/*
    for (int i = 0; i < 3; i++) {
        aggiungiGioco(inserisciGioco());
    }
*/
    unsigned short num_elementi;
    long* offset = ricercaGlobale(query, &num_elementi);

    for (int i = 0; i < num_elementi; i++) {
        printf("%lo\n", offset[i]);
        gioco_t gioco = ricercaSpecifica(offset[i]);
        printf("%s\n", gioco.titolo);
        printf("%s\n", gioco.generi[0]);
    }

    gioco_t *gioco = ordinaStatistiche(1);
    int len = sizeof(*gioco)/sizeof(gioco[0]);
    printf("%d\n", len);
    printf("%u",gioco[0].copie_vendute);
    printf("\n%u",gioco[1].copie_vendute);

    return 0;
}
