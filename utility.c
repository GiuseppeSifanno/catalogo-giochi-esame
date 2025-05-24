//
// Created by Giuseppe on 23/05/2025.
//

#include "utility.h"
#include "gioco.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tolower_str(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

gioco_t inserisciGioco() {
    gioco_t gioco;
    int result;
    char risposta[MAX_CHAR];

    printf("Inserisci titolo: ");
    scanf(" %[^\n]", gioco.titolo);
    tolower_str(gioco.titolo);

    printf("Inserisci editore: ");
    scanf(" %[^\n]", gioco.editore);
    tolower_str(gioco.editore);

    printf("Inserisci sviluppatore: ");
    scanf(" %[^\n]", gioco.sviluppatore);
    tolower_str(gioco.sviluppatore);

    printf("Inserisci descrizione: ");
    scanf(" %[^\n]", gioco.descrizione);
    tolower_str(gioco.descrizione);

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

    for (int i = 0; i < MAX_GENERI; i++) {
        printf("Inserisci genere %d: ", i + 1);
        scanf(" %[^\n]", gioco.generi[i]);
        tolower_str(*gioco.generi);
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
    return gioco;
}


char **analisiQuery(char query[MAX_CHAR], unsigned short *param) {
    unsigned short capacita = 1, num_elementi = *param;
    char **parametri = calloc(capacita,sizeof(char *));

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
            if (checkMemory(&num_elementi, &capacita, sizeof(char *), sizeof(char *), (void***)&parametri) == 0) {
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
    while (isspace((unsigned char)*start)) start++;

    // Se la stringa è vuota dopo il trimming iniziale
    if (*start == '\0') {
        *token = '\0';
        return;
    }

    // Rimuove spazi finali
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    // Sposta la stringa trimmata all'inizio del buffer
    if (start != token) {
        memmove(token, start, strlen(start) + 1);
    }

    if (token[0] == TOKEN_1 || token[0] == TOKEN_2) {
        unsigned short j = 1;
        while (isspace((unsigned char)token[j])) {
            j++;
        }
        memmove(&token[1], &token[j], strlen(&token[j]) + 1);
    }
}

/**
 * Gestisce l'allocazione dinamica della memoria per array di elementi generici.
 * Verifica se è necessario espandere un array e alloca memoria per un nuovo elemento.
 * 
 * @param num_elementi Puntatore al numero attuale di elementi nell'array
 * @param capacita Puntatore alla capacità attuale dell'array (verrà incrementata se necessario)
 * @param dimensione_elemento Dimensione in byte del nuovo elemento da allocare
 * @param dimensione_puntatore Dimensione in byte di ciascun puntatore nell'array (es. sizeof(int*))
 * @param array Puntatore triplo all'array da gestire
 * @return 1 se l'allocazione è avvenuta con successo, 0 in caso di errore
 */
int checkMemory(const unsigned short *num_elementi, unsigned short *capacita, 
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