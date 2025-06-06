#include <stdio.h>
#include "catalogolib.h"

long ricercaGioco();

void menuAmministratore(long codice);

void menuVisitatore(long codice);

unsigned short scegliModalita();

void mostraRecensioni(long codice);

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
        printf("\nChe operazione vuoi eseguire?\n");
        if (ruolo == 1) {
            do {
                printf("1. Esci\n");
                printf("2. Ricerca gioco\n");
                printf("3. Ordina statistiche\n");
                printf("4. Inserisci un nuovo gioco\n");
                printf("Scelta:");
                scanf("%d", &scelta);
            } while (scelta < 1 || scelta > 4);
        } else {
            do {
                printf("1. Esci\n");
                printf("2. Ricerca gioco\n");
                printf("3. Ordina statistiche \n");
                printf("Scelta:");
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
                unsigned long num_elementi = 0;
                // Recupera e salva la modalità per utilizzarla in seguito
                unsigned short mode = scegliModalita();
                gioco_t *giochi = ordinaStatistiche(mode, &num_elementi);

                if (num_elementi == 0) {
                    fprintf(stderr, "\nNon ci sono giochi nel catalogo\n");
                    break;
                }

                printf("\n");

                for (unsigned long i = 0; i < num_elementi; i++) {
                    if (mode == MODE_1) {
                        if (i == 0) printf("Titolo\t\t\t\t\tAnno pubblicazione\t\t\tCopie vendute\n");
                        printf("%-40s%-40hu%-8lu\n", giochi[i].titolo,
                           giochi[i].anno_pubblicazione, giochi[i].copie_vendute);
                    }
                    else {
                        if (i == 0) printf("Titolo\t\t\t\t\tAnno pubblicazione\t\t\tMedia valutazione\n");
                        float media = calcolaStatistiche(&giochi[i]);
                        if (media == 0) {
                            printf("%-40s%-40hu\tN/D\n",
                                giochi[i].titolo, giochi[i].anno_pubblicazione);
                        }
                        else {
                            printf("%-40s%-40hu\t%.2f\n",
                                giochi[i].titolo, giochi[i].anno_pubblicazione, media);
                        }
                    }
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
    printf("\nInserisci la query per la ricerca sapendo che le informazioni vanno separate con una %s \n",
           DELIM);
    printf(
        "Le informazioni riguardanti i generi sono preceduti da %c e le informazioni riguardanti l'anno sono precedute da un %c\n",
        TOKEN_2, TOKEN_1);
    printf("Es: Call of duty, blizzard, #azione, #tct, $2019\n");
    printf("Es: #fps, #multiplayer -- (tutti i giochi che hanno il genere fps e/o multiplayer)\n");
    printf("Inserisci query:");
    fflush(stdin);
    fgets(query, MAX_CHAR, stdin);
    // Rimuovi il carattere di nuova riga alla fine
    query[strlen(query) - 1] = '\0';

    long *posizione = ricercaGlobale(query, &num_elementi);
    if (num_elementi == 0) {
        printf("\nNon ci sono giochi che corrispondono ai parametri inseriti\n");
        return -1;
    }
    // Visualizza i risultati della ricerca
    for (unsigned short i = 0; i < num_elementi; i++) {
        // Recupera il gioco
        gioco_t gioco = ricercaSpecifica(posizione[i]);
        printf("\nCODICE GIOCO %hu\n", (i + 1));
        char generi[MAX_CHAR] = "";
        for (int i = 0; gioco.generi[i][0] != '\0'; i++) {
            strcat(generi, gioco.generi[i]);
            if (gioco.generi[i + 1][0] != '\0') strcat(generi, ", ");
        }
        printf("%-40s%-40s%-40s%-10hu\n",
            gioco.titolo, gioco.editore, gioco.sviluppatore, gioco.anno_pubblicazione);
        printf("%s\n", generi);
        printf("Descrizione: %s\n", gioco.descrizione);
    }
    do {
        printf("\nInserisci il codice gioco:");
        scanf("%ld", &codice);
    } while (codice <= 0 || codice > num_elementi);

    // Decrementa il valore del codice perchè deve corrispondere all'indice del vettore
    return posizione[codice - 1];
}

void menuAmministratore(long codice) {
    unsigned short scelta;
    gioco_t gioco;
    do {
        printf("\n---MENU AMMINISTRATORE---\n");
        printf("\nPer il gioco selezionato, che operazione vuoi eseguire?\n");
        printf("1. Modifica gioco\n");
        printf("2. Elimina gioco\n");
        printf("3: Visualizza recensioni\n");
        printf("4: Visualizza statistiche\n");
        printf("5: Torna al menu principale\n");
        printf("Scelta:");

        do {
            scanf("%hu", &scelta);
        } while (scelta < 1 || scelta > 5);

        switch (scelta) {
            case 1: {
                printf("Inserisci i dati del nuovo gioco\n");
                gioco = acquisisciGioco();
                if (modificaGioco(codice, &gioco) == 1)
                    printf("\nGioco modificato\n");
                break;
            }
            case 2: {
                if (cancellaGioco(codice) == 1) {
                    printf("\nGioco cancellato\n");
                    // Ritorna al menu principale poiché non più possibile operare su questo gioco
                    return;
                }
                break;
            }
            case 3: {
                mostraRecensioni(codice);
                break;
            }
            case 4: {
                // Recupera e salva la modalità per utilizzarla in seguito
                unsigned short mode = scegliModalita();
                gioco = ricercaSpecifica(codice);
                if (mode == MODE_1)
                    printf("\nNumero totali copie vendute:%lu\n", gioco.copie_vendute);
                else {
                    float media = calcolaStatistiche(&gioco);
                    if (media == 0) printf("\nNon ci sono valutazioni per questo gioco.\n");
                    else printf("\nLa media di valutazione e':%f\n", media);
                }
                break;
            }
            default:
                break;
        }
    } while (scelta != 5);
}

unsigned short scegliModalita() {
    unsigned short mode;
    printf("\nIn che modalita' vuoi visualizzare le statistiche?\n");
    do {
        printf("%hu. Copie vendute\n", MODE_1);
        printf("%hu. Media valutazione\n", MODE_2);
        printf("Scelta:");
        scanf("%hu", &mode);
    } while (mode < MODE_1 || mode > MODE_2);
    return mode;
}

void menuVisitatore(long codice) {
    int scelta;
    recensioni_t recensione;

    printf("Inserisci il tuo nome utente:");
    fflush(stdin);
    fgets(recensione.nome_utente, MAX_CHAR, stdin);
    // Rimuovi il carattere di nuova riga alla fine
    recensione.nome_utente[strlen(recensione.nome_utente)-1] = '\0';

    do {
        printf("\n---MENU UTENTE---\n");
        printf("\nChe operazione vuoi eseguire?\n");
        printf("1. Inserisci una recensione\n");
        printf("2. Visualizza le recensioni\n");
        printf("3. Acquista il gioco\n");
        printf("4. Torna al menu principale\n");
        do {
            printf("Scelta:");
            scanf("%d", &scelta);
        } while (scelta < 1 || scelta > 4);

        switch (scelta) {
            case 1:
                do {
                    printf("Vuoi inserire una descrizione? 1 - Si\t2 - No\nRisposta:");
                    scanf("%d", &scelta);
                }while (scelta < 1 || scelta > 2);

                if (scelta == 1) {
                    printf("Inserisci la descrizione:");
                    fflush(stdin);
                    fgets(recensione.descrizione, MAX_CHAR, stdin);
                    // Rimuovi il carattere di nuova riga alla fine
                    recensione.descrizione[strlen(recensione.descrizione) - 1] = '\0';
                }

                do {
                    printf("Inserisci la valutazione numerica (da %hu a %hu):", MIN_VALUTAZIONE, MAX_VALUTAZIONE);
                    scanf("%hhu", &recensione.valutazione);
                } while (recensione.valutazione < MIN_VALUTAZIONE || recensione.valutazione > MAX_VALUTAZIONE);

                if (inserisciRecensione(&recensione, &codice) == 1)
                    printf("\nRecensione inserita correttamente\n");
                else
                    fprintf(stderr, "Recensione non inserita. Trovato il limite massimo di recensioni per un gioco.\n");

                break;
            case 2:
                mostraRecensioni(codice);
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

void mostraRecensioni(long codice) {
    unsigned short num;
    recensioni_t *recensioni_ptr = NULL;  // Inizializzazione esplicita del puntatore

    // Chiamata alla funzione e controllo del risultato
    recensioni_ptr = visualizzaRecensioni(codice, &num);

    if (recensioni_ptr == NULL || num == 0) {
        printf("\nNon ci sono recensioni\n");
    } else {
        printf("Le recensioni sono:\n");
        for (int i = 0; i < num; i++) {
            printf("Scritta da %s\n", recensioni_ptr[i].nome_utente);
            printf("Valutazione:%hu\n", recensioni_ptr[i].valutazione);

            // Mostra tutte le descrizioni, se presenti
            if (recensioni_ptr[i].descrizione[0] != '\0') {
                printf("Descrizione:%s\n", recensioni_ptr[i].descrizione);
            }
            printf("\n");
        }

        // Libera la memoria allocata dinamicamente
        free(recensioni_ptr);
    }
}