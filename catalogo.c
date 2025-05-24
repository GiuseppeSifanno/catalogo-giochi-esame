#include <stdio.h>
#include "catalogolib.h"


int main(void) {

    char query[MAX_CHAR] = "FIFA, #fps, #2000, $1989";
    //if (aggiungiGioco(inserisciGioco()) == 0) printf("Va bene\n");
    //else printf("Gia' presente\n");

    unsigned short num_elementi = 0;
    long* offset = ricercaGlobale(query, &num_elementi);
    gioco_t gioco;
    for (int i = 0; i < num_elementi; i++) {
        printf("%lo\n", offset[i]);
    }
    ricercaSpecifica(offset[0], &gioco);
    printf("%s", gioco.titolo);
    printf("%s", gioco.generi[0]);
    return 0;
}
