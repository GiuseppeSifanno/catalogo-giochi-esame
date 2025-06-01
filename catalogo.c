#include <stdio.h>
#include "catalogolib.h"

long ricercaGioco();

void menuAmministratore(long codice);

void menuVisitatore(long codice);

unsigned short scegliModalita();

int main(void) {
    short ruolo = -1;
    // Validazione input utente per determinare il ruolo
    do {
        printf("Inserisci il tuo ruolo (1 = Amministratore, 0 = Visitatore):");
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
                printf("1. Esci\n");
                printf("2. Ricerca gioco\n");
                printf("3. Ordina statistiche\n");
                printf("4. Inserisci un nuovo gioco\n");
                printf("Scelta: ");
                scanf("%d", &scelta);
            } while (scelta < 1 || scelta > 4);
        } else {
            do {
                printf("1. Esci\n");
                printf("2. Ricerca gioco\n");
                printf("3. Ordina statistiche \n");
                printf("Scelta: ");
                scanf("%d", &scelta);
            } while (scelta < 1 || scelta > 3);
        }

        switch (scelta) {
            case 1: {
                printf("Uscita...\n");
                break;
            }
            case 2: {
                long codice = ricercaGioco();
                if (codice == -1) break;

                if (ruolo == 1) menuAmministratore(codice);
                else menuVisitatore(codice);

                break;
            }
            case 3: {
                unsigned int num_elementi;
                gioco_t *giochi = ordinaStatistiche(scegliModalita(), &num_elementi);
                for (int i = 0; i < num_elementi; i++) {
                    printf("Titolo: %s \n Anno di pubblicazione: %hu \n ", giochi[i].titolo,
                           giochi[i].anno_pubblicazione);
                }
                break;
            }
            case 4: {
                if (ruolo == 1) {
                    if (aggiungiGioco(acquisisciGioco()) == 1) printf("Gioco gia presente.\n");
                    else printf("Gioco aggiunto.\n");
                }
                break;
            }
            default:
                break;
        }
    } while (scelta != 1);

    system("pause");
}

long ricercaGioco() {
    char query[MAX_CHAR];
    unsigned short num_elementi;
    long codice;
    printf("Inserisci la query per la ricerca sapendo che le informazioni vanno separate con una %s \n",
           DELIM);
    printf(
        "Le informazioni riguardanti i generi sono preceduti da %c e le informazioni riguardanti l'anno sono precedute da un %c\n",
        TOKEN_2, TOKEN_1);
    printf("es: Call of duty, blizzard, #azione, #tct, $2019\n");
    printf("es: #fps, #multiplayer -- (tutti i giochi che hanno il genere fps e/o multiplayer)\n");
    printf("Inserisci query: ");
    fgets(query, MAX_CHAR, stdin);
    fflush(stdin);

    long *posizione = ricercaGlobale(query, &num_elementi);
    if (num_elementi == 0) {
        printf("Non ci sono giochi che corrispondono ai parametri inseriti\n");
        return -1;
    }
    for (unsigned short i = 0; i < num_elementi; i++) {
        gioco_t gioco = ricercaSpecifica(posizione[i]);
        printf("CODICE GIOCO %hu\n", (i + 1));
        printf("Titolo: %s\tEditore: %s\tGenere: %s\tAnno di pubblicazione:%hu\n", gioco.titolo,
               gioco.editore, gioco.generi[0], gioco.anno_pubblicazione);
    }
    do {
        printf("\nInserisci il codice gioco:");
        scanf("%d", &codice);
    } while (codice <= 0 || codice > num_elementi);

    //decrementiamo il valore del codice perchè deve corrispondere all'indice del vettore
    return posizione[codice - 1];
}

void menuAmministratore(long codice) {
    unsigned short scelta, num;
    gioco_t gioco;
    do {
        printf("Per il gioco selezionato, che operazione vuoi eseguire?\n");
        printf("1. Modifica gioco\n");
        printf("2. Elimina gioco\n");
        printf("3: Visualizza recensioni\n");
        printf("4: Visualizza statistiche\n");
        printf("5: Torna al menu principale\n");
        printf("Scelta: ");

        do {
            scanf("%hu", &scelta);
        } while (scelta < 1 || scelta > 5);

        switch (scelta) {
            case 1: {
                printf("Inserisci i dati del nuovo gioco");
                gioco = acquisisciGioco();
                if (modificaGioco(codice, &gioco) == 1)
                    printf("\n Gioco modificato\n");
                break;
            }
            case 2: {
                if (cancellaGioco(codice) == 1) {
                    printf("\nGioco cancellato \n");
                    return;
                }
                break;
            }
            case 3: {
                recensioni_t *recensioni_ptr = visualizzaRecensioni(codice, &num);
                if (num == 0) printf("Non ci sono recensioni\n");
                printf("Le recensioni sono : \n");
                for (int i = 0; i < num; i++) {
                    printf("Scritta da %s: \n", recensioni_ptr[i].nome_utente);
                    if (recensioni_ptr[i].descrizione[0] != '\0')
                        printf("%s\n", recensioni_ptr[i].descrizione);
                    printf("Valutazione e' : %d", recensioni_ptr[i].valutazione);
                }

                break;
            }
            case 4: {
                unsigned short mode = scegliModalita();
                gioco = ricercaSpecifica(codice);
                if (mode == MODE_1)
                    printf("Numero totali copie vendute: %lu \n", (unsigned long) calcolaStatistiche(MODE_1, &gioco));
                else
                    printf("La media di valutazione e' : %f \n", calcolaStatistiche(MODE_2, &gioco));

                break;
            }
            default:
                break;
        }
    } while (scelta != 5);
}

unsigned short scegliModalita() {
    unsigned short mode;
    printf("In che modalità vuoi visualizzare le statistiche? \n");
    do {
        printf("1. Copie vendute\n");
        printf("2. Media valutazione\n");
        scanf("%hu", &mode);
    } while (mode < MODE_1 || mode > MODE_2);
    return mode;
}

void menuVisitatore(long codice) {
    int scelta;
    unsigned short num;
    recensioni_t recensione;
    printf("Inserisci il tuo nome utente:");
    fflush(stdin);
    fgets(recensione.nome_utente, MAX_CHAR, stdin);
    fflush(stdin);
    do {
        printf("\n---MENU UTENTE---\n");
        printf("Che operazione vuoi eseguire?\n");
        printf("1. Inserisci una recensione\n");
        printf("2. Visualizza le recensioni\n");
        printf("3. Acquista il gioco\n");
        printf("4. Torna al menu principale\n");
        do {
            printf("Scelta: ");
            scanf("%d", &scelta);
        } while (scelta < 1 || scelta > 4);
        switch (scelta) {
            case 1:
                do {
                    printf("Vuoi inserire una descrizione? 1 - Si\t2 - No\nRisposta: ");
                    scanf("%d", &scelta);
                }while (scelta < 1 || scelta > 2);

                if (scelta == 1) {
                    printf("Inserisci la descrizione \n");
                    scanf("%[^\n]s", recensione.descrizione);
                    fflush(stdin);
                }

                do {
                    printf("Inserisci la valutazione numerica (da 1 a 5) \n");
                    scanf("%hu", &recensione.valutazione);
                } while (recensione.valutazione < 1 || recensione.valutazione > 5);

                if (inserisciRecensione(&recensione, &codice) == 1)
                    printf("\nRecensione inserita correttamente\n");
                else
                    fprintf(stderr, "Recensione non inserita. Trovato il limite massimo di recensioni per un gioco.\n");

                break;
            case 2:
                recensioni_t *recensioni_ptr = visualizzaRecensioni(codice, &num);
                if (num == 0) printf("Non ci sono recensioni\n");
                else {
                    printf("Le recensioni sono: \n");
                    for (int i = 0; i < num; i++) {
                        printf("Scritta da %s\n", recensioni_ptr[i].nome_utente);
                        printf("Valutazione: %hu\n", recensioni_ptr[i].valutazione);

                        if (recensioni_ptr[i].descrizione[0] != '\0')
                            printf("Descrizione: %s\n", recensioni_ptr[i].descrizione);
                    }
                }
                break;
            case 3:
                if (acquistaGioco(codice) == 1)
                    printf("Gioco acquistato correttamente\n");
                break;
            default:
                break;
        }
    } while (scelta != 4);
}
