#include <stdio.h>
#include "catalogolib.h"


int main(void) {

    char query[MAX_CHAR] = "Call of duty,,,, #       Fps   t ,        # Shooter, $2019";
    inserisciGioco();
    ricercaGlobale(query);

    return 0;
}
