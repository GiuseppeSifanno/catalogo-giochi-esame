#include <stdio.h>
#include "catalogolib.h"

long ricercaGioco();

void Amministratore(long codice);

unsigned short scegliModalita();

void Visitatore(void);

int main(void) {
    short ruolo = -1;
    gioco_t *gioco;
    // Validazione input utente per determinare il ruolo
    do {
        printf("Inserisci il tuo ruolo (1 = Amministratore, 0 = Visitatore): ");
        scanf("%hd", &ruolo);

        if (ruolo != 0 && ruolo != 1) {
            printf("Ruolo non valido. Riprova.\n");
        }
    } while (ruolo != 0 && ruolo != 1);
    int scelta;
    do {
        printf("Che operazione vuoi eseguire?\n");
        if (ruolo == 1) {
            do {
                printf("0. Esci\n");
                printf("1. Ricerca gioco\n");
                printf("2. Ordina statistiche \n");
                printf("3. inserisci gioco \n");
                scanf("%d", &scelta);
            } while (scelta < 0 || scelta > 3);
        } else {
            do {
                printf("0. Esci\n");
                printf("1. Ricerca gioco\n");
                printf("2. Ordina statistiche \n");
                scanf("%d", &scelta);
            } while (scelta < 0 || scelta > 2);
        }
        switch (scelta) {
            case 0:
                printf("Uscita...\n");
                break;
            case 1:
                long codice = ricercaGioco();
                if (ruolo == 1) Amministratore((codice - 1));
                else Visitatore();
                break;
            case 2:
                unsigned int num_elementi;
                gioco_t *giochi;
                giochi = ordinaStatistiche(scegliModalita(), &num_elementi);
                for (int i = 0; i < num_elementi; i++) {
                    printf("Titolo: %s \n Anno di pubblicazione: %hu \n ",giochi[i].titolo,giochi[i].anno_pubblicazione);
                }
                break;
            case 3:
                if (ruolo == 1) {
                    if (aggiungiGioco(inserisciGioco()) == 1) printf("Gioco gia presente.\n");
                    else printf("Gioco aggiunto.\n");
                }
                break;
            default:
                break;

        }
    } while (scelta != 0);


    return 0;
}

long ricercaGioco() {
    char query[MAX_CHAR];
    unsigned short num_elementi;
    gioco_t gioco;
    long codice;
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
    if (num_elementi == 0) printf("non ci sono giochi");
    else {
        for (unsigned short i = 0; i < num_elementi; i++) {
            gioco = ricercaSpecifica(posizione[i]);
            printf("CODICE GIOCO %hu\n", (i + 1));
            printf("Titolo: %s\tEditore: %s\tGenere: %s\tAnno di pubblicazione:%hu\n", gioco.titolo,
                   gioco.editore, gioco.generi[0], gioco.anno_pubblicazione);
        }
        do {
            printf("\nInserisci il codice\n");
            scanf("%d", &codice);
        } while (codice <= 0);
    }
    return codice;
}

void Amministratore(long codice) {
    int scelta;
    unsigned short num, mode;
    gioco_t gioco;
    recensioni_t *recensioni_ptr;
    do {
        printf("dato il gioco da voi selezionato che operazione vuoi eseguire ? \n ");
        printf("1. modifica\n");
        printf("2. elimina gioco \n");
        printf("3: visualizza recensioni\n");
        printf("4: visualizza statistiche\n");
        printf("5: Torna al menu principale\n");
        printf("Scelta: ");
        do {
            scanf("%d", &scelta);
        }while(scelta < 1 || scelta > 5);
        switch (scelta) {
            case 1:
                printf("inserici i dati del nuovo gioco");
            gioco = inserisciGioco();
            if (modificaGioco(codice, &gioco) == 1)
                printf("\n Gioco modificato\n");
            break;
            case 2:
                if (cancellaGioco(codice) == 1) printf("\nGioco cancellato \n");
            case 3:
                recensioni_ptr = visualizzaRecensioni(codice, &num);
            if (num == 0) printf("Non ci sono recensioni\n");
            printf("Le recensioni sono : \n");
            for (int i = 0; i < num; i++) {
                printf("Scritta da %s: \n", recensioni_ptr[i].nome_utente);
                if (recensioni_ptr[i].descrizione[0] != '\0')
                    printf("%s\n", recensioni_ptr[i].descrizione);
                printf("Valutazione e' : %d", recensioni_ptr[i].valutazione);
            }
            break;
            case 4:
                mode = scegliModalita();
            gioco = ricercaSpecifica(codice);
            if (mode == MODE_1)
                printf("Numero totali copie vendute: %lu \n", (unsigned long) calcolaStatistiche(MODE_1, &gioco));
            else
                printf("La media di valutazione e' : %f \n", calcolaStatistiche(MODE_2, &gioco));
            break;
            default:
                break;
        }
    }while (scelta != 5);
}

unsigned short scegliModalita() {
    unsigned short mode;
    printf("In che modalita vuoi visualizzare le statistiche? \n");
    do {
        printf("1. Copie vendute\n");
        printf("2. Media valutazione\n");
        scanf("%hu", &mode);
    } while (mode < MODE_1 || mode > MODE_2);
    return mode;

}

void Visitatore(void){}