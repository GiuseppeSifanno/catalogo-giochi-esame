#include <stdio.h>
#include "catalogolib.h"


int main(void) {

    char query[MAX_CHAR] = "all of duty, #Fps, #Shooter";
    //aggiungiGioco(inserisciGioco());
    long *offset = ricercaGlobale(query);
        gioco_t gioco;
    for (int i = 0; i < 10; i++) {
        printf("%lo\n", offset[i]);
    }
    ricercaSpecifica(&offset[0], &gioco);
    printf("%s", gioco.titolo);
    return 0;
}
