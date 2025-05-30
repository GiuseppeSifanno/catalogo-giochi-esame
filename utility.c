//
// Created by Giuseppe on 23/05/2025.
//

#include "utility.h"

void tolower_str(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char) tolower(str[i]);
    }
}

gioco_t inserisciGioco() {
    gioco_t gioco;
    int result;
    char risposta[MAX_CHAR];

    // Inizializza tutti i campi a zero/vuoto
    memset(&gioco, 0, sizeof(gioco_t));

    printf("Inserisci titolo: ");
    scanf(" %[^\n]", gioco.titolo);
    tolower_str(gioco.titolo);
    fflush(stdin);
    printf("Inserisci editore: ");
    scanf(" %[^\n]", gioco.editore);
    tolower_str(gioco.editore);
    fflush(stdin);
    printf("Inserisci sviluppatore: ");
    scanf(" %[^\n]", gioco.sviluppatore);
    tolower_str(gioco.sviluppatore);
    fflush(stdin);
    printf("Inserisci descrizione: ");
    scanf(" %[^\n]", gioco.descrizione);
    tolower_str(gioco.descrizione);
    fflush(stdin);
    do {
        printf("Inserisci anno di pubblicazione (>= %d): ", ANNO_MIN);
        result = scanf("%hu", &gioco.anno_pubblicazione);

        if (result != 1) {
            printf("Input non valido. Inserisci un numero.\n");
            // Pulisce il buffer da input errato
            while (getchar() != '\n');
        } else if (gioco.anno_pubblicazione < ANNO_MIN) {
            printf("Anno non valido. Deve essere >= %d.\n", ANNO_MIN);
            // Anche se input valido, forziamo il ciclo a ripetere
            result = 0;
        } else {
            // Input corretto, puliamo eventuali residui
            while (getchar() != '\n');
        }
    } while (result != 1);
    fflush(stdin);
    result = 0;
    do {
        printf("Inserisci numero copie vendute: ");
        result = scanf("%lu", &gioco.copie_vendute);

        if (result != 1) {
            printf("Input non valido. Inserisci un numero intero positivo.\n");
            while (getchar() != '\n'); // pulizia buffer
        } else {
            while (getchar() != '\n'); // pulizia buffer residuo
        }
    } while (result != 1);;
    fflush(stdin);

    for (int i = 0; i < MAX_GENERI; i++) {
        printf("Inserisci genere %d: ", i + 1);
        scanf(" %[^\n]", gioco.generi[i]);
        tolower_str(gioco.generi[i]);
        if (i < MAX_GENERI - 1) {
            while (1) {
                printf("Vuoi inserire un altro genere? (Si/No): ");
                scanf(" %[^\n]", risposta);
                tolower_str(risposta);
                if (strcmp(risposta, "si") == 0) {
                    break; // continua il ciclo
                } else if (strcmp(risposta, "no") == 0) {
                    i = MAX_GENERI; // forza uscita dal ciclo
                    break;
                } else {
                    printf("Risposta non valida. Scrivi 'Si' o 'No'.\n");
                }
            }
        }
    }
    fflush(stdin);
    return gioco;
}

char **analisiQuery(char query[MAX_CHAR], unsigned short *param) {
    unsigned short capacita = 1, num_elementi = 0;
    char **parametri = calloc(capacita, sizeof(char *));

    if (parametri == NULL) {
        printf("Errore di allocazione memoria\n");
        exit(-1);
    }

    // Crea una copia della stringa query per strtok
    char query_copy[MAX_CHAR];
    strncpy(query_copy, query, MAX_CHAR);
    query_copy[MAX_CHAR - 1] = '\0'; // Assicura terminazione

    char *token = strtok(query_copy, DELIM);

    while (token != NULL) {
        char token_copy[MAX_CHAR];
        strncpy(token_copy, token, MAX_CHAR);
        token_copy[MAX_CHAR - 1] = '\0';

        // Rimuovi spazi all'inizio e alla fine
        trim(token_copy);

        unsigned short len = strlen(token_copy);

        // Se il token non è vuoto dopo il trimming
        if (len > 0) {
            if (checkMemory(&num_elementi, &capacita, sizeof(char *), sizeof(char *), (void ***) &parametri) == 0) {
                return NULL;
            }

            // Alloca memoria per la stringa
            parametri[num_elementi] = malloc((len + 1) * sizeof(char));
            if (parametri[num_elementi] == NULL) {
                printf("Errore allocazione memoria per stringa\n");
                // Libera la memoria già allocata
                for (unsigned short i = 0; i < num_elementi; i++) {
                    free(parametri[i]);
                }
                free(parametri);
                return NULL;
            }

            // Copia il parametro
            strncpy(parametri[num_elementi], token_copy, len);
            parametri[num_elementi][len] = '\0'; // Termina la stringa

            num_elementi++;
        }

        token = strtok(NULL, DELIM);
    }
    *param = num_elementi;
    return parametri;
}

void trim(char *token) {
    char *start = token;
    // Rimuove spazi iniziali
    while (isspace((unsigned char) *start)) start++;

    // Se la stringa è vuota dopo il trimming iniziale
    if (*start == '\0') {
        *token = '\0';
        return;
    }

    // Rimuove spazi finali
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char) *end)) end--;
    *(end + 1) = '\0';

    // Sposta la stringa trimmata all'inizio del buffer
    if (start != token) {
        memmove(token, start, strlen(start) + 1);
    }

    if (token[0] == TOKEN_1 || token[0] == TOKEN_2) {
        unsigned short j = 1;
        while (isspace((unsigned char) token[j])) {
            j++;
        }
        memmove(&token[1], &token[j], strlen(&token[j]) + 1);
    }
}

int checkMemory(unsigned short *num_elementi, unsigned short *capacita,
                unsigned long dimensione_elemento, unsigned long dimensione_puntatore, void ***array) {
    // Verifica se è necessario espandere l'array
    if (*num_elementi >= *capacita) {
        // Incrementa la capacità
        unsigned short nuova_capacita = (*capacita) + 1;

        // Esegue il realloc usando dimensione_puntatore
        void **nuovo_array = realloc(*array, nuova_capacita * dimensione_puntatore);

        if (nuovo_array == NULL) {
            printf("Errore nell'espansione dell'array: memoria insufficiente\n");
            // Libera la memoria già allocata
            for (unsigned short i = 0; i < *num_elementi; i++) {
                free((*array)[i]);
            }
            free(*array);
            return 0; // Segnala fallimento
        }

        // Aggiorna il puntatore e la capacità
        *array = nuovo_array;
        *capacita = nuova_capacita;
    }

    // Alloca memoria per il nuovo elemento
    (*array)[*num_elementi] = malloc(dimensione_elemento);

    if ((*array)[*num_elementi] == NULL) {
        printf("Errore nell'allocazione del nuovo elemento: memoria insufficiente\n");
        // Libera la memoria già allocata
        for (unsigned short i = 0; i < *num_elementi; i++) {
            free((*array)[i]);
        }
        free(*array);
        return 0; // Segnala fallimento
    }

    return 1; // Segnala successo
}

FILE *apriCatalogo(char mode[3]) {
    FILE *file = fopen(NOME_FILE, mode);

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDirectory attuale: %s\n", cwd);

    if (file == NULL) {
        // Se il file non esiste e stiamo cercando di aprirlo in lettura
        if (mode[0] == 'r') {
            // Se il file non esiste e stiamo in modalità di lettura, proviamo a crearlo
            file = fopen(NOME_FILE, "wb");
            if (file == NULL) {
                fprintf(stderr, "Errore: impossibile creare il file %s\n", NOME_FILE);
                exit(-1);
            }
            printf("File %s creato correttamente\n", NOME_FILE);
            fclose(file);
            // Riapri il file nella modalità originale richiesta
            file = fopen(NOME_FILE, mode);
            if (file == NULL) {
                fprintf(stderr, "Errore: impossibile aprire il file %s in modalità %s\n", NOME_FILE, mode);
                exit(-1);
            }
        } else {
            fprintf(stderr, "Errore apertura file %s in modalità %s\n", NOME_FILE, mode);
            exit(-1);
        }
    }

    return file;
}

void ShellSort(gioco_t *giochi, unsigned int *dim, unsigned short mode) {
    // Se c'è solo un elemento o nessuno, non c'è nulla da ordinare
    if (*dim <= 1) return;

    int i, j, k, gap; // Cambiato j da unsigned short a int
    gioco_t x;
    unsigned short a[5] = {9, 5, 3, 2, 1}; // a = vettore dei gap

    for (k = 0; k < sizeof(a) / sizeof(a[0]); k++) {
        // ciclo ripetuto per tutti i gap
        gap = a[k];
        // Se il gap è maggiore o uguale alla dimensione, passa al gap successivo
        if (gap >= *dim) continue;

        for (i = gap; i < *dim; i++) {
            x = giochi[i];
            if (mode == MODE_1) {
                // Utilizzo del ciclo for con controllo j>=0 come prima condizione
                for (j = i - gap; j >= 0 && (x.copie_vendute > giochi[j].copie_vendute); j -= gap) {
                    giochi[j + gap] = giochi[j];
                }
                giochi[j + gap] = x;
            } else {
                // Per la modalità 2 (media valutazione)

                for (j = i - gap; j >= 0; j -= gap) {
                    if (calcolaStatistiche(2, &giochi[i]) < calcolaStatistiche(2, &giochi[j]))
                        giochi[j + gap] = giochi[j];
                    else
                        break;
                }
                giochi[j + gap] = x;
            }
        }
    }
}
