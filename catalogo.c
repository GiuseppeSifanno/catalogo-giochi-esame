#include <stdio.h>
#include "catalogolib.h"


int main(void) {
    short ruolo = -1;

    // Validazione input utente per determinare il ruolo
    do {
        printf("Inserisci il tuo ruolo (1 = Amministratore, 0 = Visitatore): ");
        scanf("%hd", &ruolo);

        if (ruolo != 0 && ruolo != 1) {
            printf("Ruolo non valido. Riprova.\n");
        }
    } while (ruolo != 0 && ruolo != 1);

    int scelta;
    char query[MAX_CHAR];
    unsigned short num_elementi;
    gioco_t gioco;
    int codice;

    if (ruolo == 1) {
        // Menu amministratore
        do {
            printf("\n--- Menu Amministratore ---\n");
            printf("0. Esci\n");
            printf("1. Aggiungi Gioco\n");
            printf("2. Modifica Gioco\n");
            printf("3. Cancella Gioco\n");
            printf("Scelta: ");
            scanf("%d", &scelta);

            switch (scelta) {
                case 0:
                    printf("Uscita...\n");
                    break;
                case 1:
                    if (aggiungiGioco(inserisciGioco()) == 1)
                        printf("\n il gioco e' già presente \n");
                    else
                        printf("Gioco aggiunto \n");
                    break;
                case 2:
                    printf("Inserisci la query per la ricerca sapendo che le informazioni vanno separate con una %s \n",
                           DELIM);
                    printf(
                        "Le informazioni riguardanti i generi sono preceduti da %c e le informazioni riguardanti l'anno sono precedute da un %c\n",
                        TOKEN_2, TOKEN_1);

                    printf("es: Call of duty, blizzard, #azione, #tct, $2019\n");
                    printf("es: #fps, #multiplayer -- (tutti i giochi che hanno il genero fps e/o multiplayer)\n");
                    printf("Inserisci query: ");
                    fgets(query, MAX_CHAR, stdin);
                    fflush(stdin);
                    long *offset = ricercaGlobale(query, &num_elementi);
                    for (unsigned short i = 0; i < num_elementi; i++) {
                        gioco = ricercaSpecifica(offset[i]);
                        printf("CODICE GIOCO %hu\n", (i + 1));
                        printf("Titolo: %s\tEditore: %s\tGenere: %s\tAnno di pubblicazione:%hu\n", gioco.titolo,
                               gioco.editore, gioco.generi[0], gioco.anno_pubblicazione);
                    }
                    printf("\nInserisci il codice del gioco che ti interessa: ");
                    scanf("%d", &codice);

                    gioco = inserisciGioco();
                    if (modificaGioco((codice - 1), &gioco) == 1)
                        printf("\n Gioco modificato\n");
                    break;
                case 3:
                    //cancella gioco
                    printf("Inserisci la query per la ricerca sapendo che le informazioni vanno separate con una %s \n",
                           DELIM);
                    printf(
                        "Le informazioni riguardanti i generi sono preceduti da %c e le informazioni riguardanti l'anno sono precedute da un %c\n",
                        TOKEN_2, TOKEN_1);
                    printf("es: Call of duty, blizzard, #azione, #tct, $2019\n");
                    printf("es: #fps, #multiplayer -- (tutti i giochi che hanno il genero fps e/o multiplayer)\n");
                    printf("Inserisci query: ");
                    fgets(query, MAX_CHAR, stdin);
                    fflush(stdin);
                    long *posizione = ricercaGlobale(query, &num_elementi);
                    for (unsigned short i = 0; i < num_elementi; i++) {
                        gioco = ricercaSpecifica(posizione[i]);
                        printf("CODICE GIOCO %hu\n", (i + 1));
                        printf("Titolo: %s\tEditore: %s\tGenere: %s\tAnno di pubblicazione:%hu\n", gioco.titolo,
                               gioco.editore, gioco.generi[0], gioco.anno_pubblicazione);
                    }
                    printf("\nInserisci il codice\n");
                    scanf("%d", &codice);
                    if (cancellaGioco((codice-1)) == 1) printf("\nGioco cancellato \n");
                    break;
                default:
                    printf("Scelta non valida. Riprova.\n");
            }
        } while (scelta != 0);
    } else {
        // Menu visitatore
        do {
            printf("\n--- Menu Visitatore ---\n");
            printf("1. Cerca Gioco\n");
            printf("2. Aggiungi Recensione\n");
            printf("0. Esci\n");
            printf("Scelta: ");
            scanf("%d", &scelta);

            switch (scelta) {
                case 0:
                    printf("Uscita...\n");
                    break;
                case 1:
                    // CERCA UN GIOCO
                    printf("Inserisci la query sapendo che le informazioni del gioco vanni separate con una %s \n",DELIM);
                    printf(
                    "Le informazioni riguardanti i generi sono preceduti da %c e le informazioni riguardanti l'anno sono precedute da un %c\n",TOKEN_2, TOKEN_1);
                    printf("es: Call of duty, blizzard, #azione, #tct, $2019\n");
                    printf("Inserisci query: ");
                    fgets(query, MAX_CHAR, stdin);
                    fflush(stdin);
                    long *posizione = ricercaGlobale(query, &num_elementi);
                    for (unsigned short i = 0; i < num_elementi; i++) {
                        gioco = ricercaSpecifica(posizione[i]);
                        printf("CODICE GIOCO %hu\n", (i + 1));
                        printf("Titolo: %s\tEditore: %s\tGenere: %s\tAnno di pubblicazione:%hu\n", gioco.titolo,
                               gioco.editore, gioco.generi[0], gioco.anno_pubblicazione);
                    }
                    break;
                case 2:
                    break;
                default:
                    printf("Scelta non valida. Riprova.\n");
            }
        } while (scelta != 0);
    }

    return 0;
}
