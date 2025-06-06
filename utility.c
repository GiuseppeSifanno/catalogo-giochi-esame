#include "utility.h"

void tolower_str(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char) tolower(str[i]);
    }
}

gioco_t acquisisciGioco() {
    gioco_t gioco;
    char risposta[MAX_CHAR];

    // Inizializza tutti i campi a zero/vuoto
    memset(&gioco, 0, sizeof(gioco_t));

    do {
        printf("\nTitolo:");
        fgets(gioco.titolo, MAX_CHAR, stdin);
        // Rimuovi il carattere di nuova riga
        gioco.titolo[strlen(gioco.titolo) - 1] = '\0';
        tolower_str(gioco.titolo);
    } while (strlen(gioco.titolo) == 0);

    do {
        printf("Editore:");
        fgets(gioco.editore, MAX_CHAR, stdin);
        // Rimuovi il carattere di nuova riga
        gioco.editore[strlen(gioco.editore) - 1] = '\0';
        tolower_str(gioco.editore);
    }while (strlen(gioco.editore) == 0);

    do {
        printf("Sviluppatore:");
        fgets(gioco.sviluppatore, MAX_CHAR, stdin);
        // Rimuovi il carattere di nuova riga
        gioco.sviluppatore[strlen(gioco.sviluppatore) - 1] = '\0';
        tolower_str(gioco.sviluppatore);
    }while (strlen(gioco.sviluppatore) == 0);

    do {
        printf("Descrizione:");
        fgets(gioco.descrizione, MAX_CHAR, stdin);
        // Rimuovi il carattere di nuova riga
        gioco.descrizione[strlen(gioco.descrizione) - 1] = '\0';
        tolower_str(gioco.descrizione);
    }while (strlen(gioco.descrizione) == 0);

    do {
        printf("Anno di pubblicazione (>= %d):", ANNO_MIN);
        scanf("%hu", &gioco.anno_pubblicazione);
        CLEAR_INPUT_BUFFER();
    } while (gioco.anno_pubblicazione < ANNO_MIN);

    do {
        printf("Numero di copie vendute (>= 0):");
        scanf("%lu", &gioco.copie_vendute);
        CLEAR_INPUT_BUFFER();
    } while (gioco.copie_vendute < 0);

    for (int i = 0; i < MAX_GENERI; i++) {
        printf("Genere n.%d:", (i + 1) );
        fgets(gioco.generi[i], MAX_CHAR, stdin);
        // Rimuovi il carattere di nuova riga
        gioco.generi[i][strlen(gioco.generi[i]) - 1] = '\0';
        tolower_str(gioco.generi[i]);

        if (i < MAX_GENERI - 1) {
            while (1) {
                printf("Vuoi inserire un altro genere? (Si/No):");
                fgets(risposta, MAX_CHAR, stdin);
                // Rimuovi il carattere di nuova riga
                risposta[strlen(risposta) - 1] = '\0';
                tolower_str(risposta);

                if (strcmp(risposta, "si") == 0)
                    break; // continua il ciclo

                if (strcmp(risposta, "no") == 0) {
                    i = MAX_GENERI; // forza uscita dal ciclo
                    break;
                }

                printf("Risposta non valida. Scrivi 'Si' o 'No' (case insensitive).\n");
            }
        }
    }
    return gioco;
}

unsigned short isAlredyAdded(gioco_t new_gioco) {
    // Apri il file in modalità lettura
    FILE *file = apriCatalogo("rb");
    gioco_t gioco;

    // 1 non presente nel catalogo, 0 altrimenti
    unsigned short int valido;

    while (fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        // Controllo sui campi principali
        if (gioco.anno_pubblicazione == new_gioco.anno_pubblicazione    &&
            gioco.copie_vendute == new_gioco.copie_vendute              &&
            strcmp(gioco.titolo, new_gioco.titolo) == 0                 &&
            strcmp(gioco.descrizione, new_gioco.descrizione) == 0       &&
            strcmp(gioco.editore, new_gioco.editore) == 0               &&
            strcmp(gioco.sviluppatore, new_gioco.sviluppatore) == 0) {

            valido = 0; // Presupponi che sia lo stesso gioco

            // Se almeno un genere è diverso, allora non è lo stesso gioco
            for (unsigned short i = 0; i < MAX_GENERI; i++) {
                if (strcmp(gioco.generi[i], new_gioco.generi[i]) != 0) {
                    valido = 1;
                    break;
                }
            }

            // Se non è valido, è già presente nel catalogo
            if (valido == 0) {
                fclose(file);
                return 1; // Gioco già presente
            }
        }
    }
    fclose(file);
    return 0; // Gioco non presente quindi valido = 1
}

char **analisiQuery(char query[MAX_CHAR], unsigned short *param) {
    unsigned short capacita = 1, num_elementi = 0;
    char **parametri = calloc(capacita, sizeof(char *));

    if (parametri == NULL) {
        fprintf(stderr, "Errore di allocazione memoria\n");
        exit(-1);
    }

    // Crea una copia della stringa query per strtok
    char query_copy[MAX_CHAR];
    strncpy(query_copy, query, MAX_CHAR);
    query_copy[MAX_CHAR - 1] = '\0'; // Assicura terminazione

    // Variabile contenente i token
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
                fprintf(stderr, "Errore allocazione memoria per stringa\n");
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
        // Recupera il prossimo token
        token = strtok(NULL, DELIM);
    }
    // Salva il numero di parametri, per poterli recuperare in seguito
    *param = num_elementi;
    return parametri;
}

void trim(char *token) {
    char *start = token;
    // Rimuovi spazi iniziali
    while (isspace((unsigned char) *start)) start++;

    // Se la stringa è vuota dopo il trimming iniziale
    if (*start == '\0') {
        *token = '\0';
        return;
    }

    // Rimuovi spazi finali
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char) *end)) end--;
    *(end + 1) = '\0';

    // Sposta la stringa trimmata all'inizio del buffer
    if (start != token) {
        memmove(token, start, strlen(start) + 1);
    }

    // Rimuovi gli spazi tra il simbolo e il resto della stringa
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

        // Esegui il realloc usando dimensione_puntatore
        void **nuovo_array = realloc(*array, nuova_capacita * dimensione_puntatore);

        if (nuovo_array == NULL) {
            fprintf(stderr, "Errore nell'espansione dell'array: memoria insufficiente\n");

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
        fprintf(stderr, "Errore nell'allocazione del nuovo elemento: memoria insufficiente\n");

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

    if (file == NULL) {
        // Se il file non esiste e stiamo cercando di aprirlo in lettura
        if (mode[0] == 'r') {
            // Se il file non esiste e stiamo in modalità di lettura, proviamo a crearlo
            file = fopen(NOME_FILE, "wb");
            if (file == NULL) {
                fprintf(stderr, "Errore: impossibile creare il file %s\n", NOME_FILE);
                exit(-1);
            }
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

void shellSort(gioco_t *giochi, unsigned long *dim, unsigned short mode) {
    // Se c'è solo un elemento o nessuno, non c'è nulla da ordinare
    if (*dim <= 1) return;

    int j;
    unsigned short a[5] = {9, 5, 3, 2, 1}; // a = vettore dei gap

    for (int k = 0; k < sizeof(a) / sizeof(a[0]); k++) {
        // ciclo ripetuto per tutti i gap
        int gap = a[k];

        // Se il gap è maggiore o uguale alla dimensione, passa al gap successivo
        if (gap >= *dim) continue;

        for (int i = gap; i < *dim; i++) {
            gioco_t x = giochi[i];
            if (mode == MODE_1) {
                for (j = i - gap; j >= 0 && (x.copie_vendute > giochi[j].copie_vendute); j -= gap) {
                    giochi[j + gap] = giochi[j];
                }
                giochi[j + gap] = x;
            } else {
                // Per la modalità 2 (media valutazione)
                for (j = i - gap; j >= 0; j -= gap) {
                    if (calcolaStatistiche(&giochi[i]) < calcolaStatistiche(&giochi[j]))
                        giochi[j + gap] = giochi[j];
                    else
                        break;
                }
                giochi[j + gap] = x;
            }
        }
    }
}