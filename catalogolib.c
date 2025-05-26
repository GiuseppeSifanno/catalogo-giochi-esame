//
// Created by Graziano Semerano on 22/05/25.
//

#include "catalogolib.h"

unsigned short aggiungiGioco(gioco_t gioco) {

    //controlliamo se il gioco è già presente nel catalogo
    if (isAlredyAdded(gioco) == 1) return 1;

    //apro il file in modalità scrittura
    FILE *file = apriCatalogo("ab");

    //scrivo il gioco nel file
    if (fwrite(&gioco, sizeof(gioco_t), 1, file) != 1) {
        fprintf(stderr, "Errore scrittura file\n");
        fclose(file);
        exit(-1);
    }

    printf("\nGioco aggiunto correttamente\n");
    //chiudo il file e salvo le modifiche
    fclose(file);
    return 0;
}

unsigned short modificaGioco(const long offset, const gioco_t *gioco) {
    FILE *file = apriCatalogo("rb+");

    if (fseek(file, offset, SEEK_SET) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        exit(-1);
    }
    if (fwrite(gioco, sizeof(gioco_t), 1, file) != 1) {
        fprintf(stderr, "Errore aggiornamento del file\n");
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

unsigned short cancellaGioco(long offset) {
    FILE *file = apriCatalogo("rb");
    FILE *new_file = fopen("new_catalogo.dat", "wb");
    gioco_t gioco;
    long pos = ftell(file);
    while (fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        if (offset != pos) {
            if (fwrite(&gioco, sizeof(gioco_t), 1, new_file) != 1) {
                fprintf(stderr, "Errore scrittura nel nuovo file\n");
                fclose(file);
                fclose(new_file);
                return 0;
            }
            pos = ftell(file);
        }
    }
    fclose(file);
    fclose(new_file);
    remove("catalogo.dat");
    rename("new_catalogo.dat", "catalogo.dat");
    return 1;
}

unsigned short isAlredyAdded(gioco_t new_gioco) {
    gioco_t gioco;
    //apro il file in modalità lettura
    FILE *file = apriCatalogo("rb");

    //valido: 0 non presente nel catalogo (valido), 1 altrimenti
    unsigned short int valido = 0;

    while (fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        // Controllo sui campi principali
        if (gioco.anno_pubblicazione == new_gioco.anno_pubblicazione &&
            gioco.copie_vendute == new_gioco.copie_vendute &&
            strcmp(gioco.titolo, new_gioco.titolo) == 0 &&
            strcmp(gioco.descrizione, new_gioco.descrizione) == 0 &&
            strcmp(gioco.editore, new_gioco.editore) == 0 &&
            strcmp(gioco.sviluppatore, new_gioco.sviluppatore) == 0) {
            
            // Se i campi principali corrispondono, controlla i generi
            valido = 1; // Presupponiamo che sia lo stesso gioco
            
            // Se almeno un genere è diverso, allora non è lo stesso gioco
            for (unsigned short i = 0; i < MAX_GENERI; i++) {
                if (strcmp(gioco.generi[i], new_gioco.generi[i]) != 0) {
                    valido = 0;
                    break;
                }
            }
            
            if (valido == 1) {
                fclose(file);
                return 1; // Gioco già presente
            }
        }
    }
    
    fclose(file);
    return 0; // Gioco non presente
}

gioco_t ricercaSpecifica(long offset) {
    //apro il file in modalità lettura
    FILE *file = apriCatalogo("rb");

    //mi posizione in una posizione specifica
    if (fseek(file, offset, SEEK_SET) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        exit(-1);
    }

    gioco_t gioco;
    //leggo il dato dal catalogo e lo salvo
    if (fread(&gioco, sizeof(gioco_t), 1, file) != 1) {
        fprintf(stderr, "Errore lettura file\n");
        fclose(file);
        exit(-1);
    }

    //chiudo il file
    fclose(file);
    return gioco;
}

long *ricercaGlobale(char query[MAX_CHAR], unsigned short *num_elementi) {
    unsigned short num_param = 0, valido = 0;

    //trasforma la stringa in ingresso in minuscolo
    for (int i = 0; query[i]; i++) {
        query[i] = (char) tolower(query[i]);
    }

    gioco_t gioco;
    ///parametri recuperati durante l'analisi della query in ingresso dell'utente
    char **parametri = analisiQuery(query, &num_param);

    if (parametri == NULL) {
        printf("Errore di analisi query\n");
        exit(-1);
    }

    ///////////////////////
    // Rimuovere terminata la fase di sviluppo del programma
    printf("Numero totale di parametri: %d\n", num_param);

    for (unsigned short i = 0; i < num_param; i++) {
        printf("Parametro %d: %s\n", i + 1, parametri[i]);
    }
    ///////////////////////

    unsigned short capacita = 1;
    *num_elementi = 0; // Inizializza a 0
    long *offset = calloc(capacita, sizeof(long));

    if (offset == NULL) {
        // Libera la memoria allocata per i parametri
        for (unsigned short i = 0; i < num_param; i++) {
            free(parametri[i]);
        }
        free(parametri);
        printf("Errore di allocazione memoria\n");
        exit(-1);
    }

    //apro il file in modalità lettura
    FILE *file = apriCatalogo("rb");
    // Verifica lettura record
    long pos = ftello(file);
    int recordLetti = 0;
    
    while (fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        recordLetti++;
        valido = 0;
        for (unsigned short i = 0; i < num_param; i++) {
            // Crea una copia del parametro per non modificare l'originale
            char param_copy[MAX_CHAR];
            strncpy(param_copy, parametri[i], MAX_CHAR);
            param_copy[MAX_CHAR - 1] = '\0'; // Assicura terminazione

            switch (param_copy[0]) {
                case TOKEN_1: { // $ - Anno di pubblicazione
                    // Rimuovi il token iniziale
                    char* value_str = param_copy + 1;
                    // Converti la stringa in numero
                    unsigned short anno = (unsigned short)strtol(value_str, NULL, 10);
                    if (gioco.anno_pubblicazione == anno)
                        valido = 1;
                }
                break;
                case TOKEN_2: { // # - Genere
                    // Rimuovi il token iniziale
                    char* genere = param_copy + 1;
                    for (unsigned short j = 0; j < MAX_GENERI; j++) {
                        if (strstr(gioco.generi[j], genere) != NULL) {
                            valido = 1;
                            break;
                        }
                    }
                }
                break;
                default: { // Ricerca generale
                    if (strstr(gioco.titolo, parametri[i]) != NULL) valido = 1;
                    if (strstr(gioco.editore, parametri[i]) != NULL) valido = 1;
                    if (strstr(gioco.sviluppatore, parametri[i]) != NULL) valido = 1;
                }
                break;
            }
            if (valido == 1) break;
        }
        if (valido == 1) {
            // Quando trovo un risultato, incremento la capacità e rialloco manualmente
            if (*num_elementi >= capacita) {
                capacita++;
                long *nuovo_offset = realloc(offset, capacita * sizeof(long));
                if (nuovo_offset == NULL) {
                    // In caso di errore, libera la memoria e termina
                    for (unsigned short i = 0; i < num_param; i++) {
                        free(parametri[i]);
                    }
                    free(parametri);
                    free(offset);
                    fclose(file);
                    return NULL;
                }
                offset = nuovo_offset;
            }
            
            offset[*num_elementi] = pos;
            (*num_elementi)++;
        }
        pos = ftello(file);
    }
    printf("Numero di record letti: %d\n", recordLetti);

    // Libera la memoria allocata per i parametri
    for (unsigned short i = 0; i < num_param; i++) {
        free(parametri[i]);
    }
    free(parametri);

    fclose(file);
    return offset;
}

unsigned short inserisciRecensione(char *username[MAX_CHAR], recensioni_t *recensione, long *offset) {
    FILE *file = apriCatalogo("rb+");
    gioco_t gioco;

    if (fseek(file, *offset, SEEK_SET) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        exit(-1);
    }

    if (fread(&gioco, sizeof(gioco_t), 1, file) != 1) {
        fprintf(stderr, "Errore lettura file\n");
        fclose(file);
        exit(-1);
    }

    for (unsigned short i = 0; i < MAX_RECENSIONI; i++) {
        if (strlen(gioco.recensioni[i].nome_utente) == 0 || gioco.recensioni[i].nome_utente[0] == '\0') {
            gioco.recensioni[i] = *recensione;
            printf("\nRecensione inserita correttamente\n");
            fclose(file);
            return 1;
        }
    }
    fprintf(stderr, "Recensione non inserita. Trovato il limite massimo di recensioni per un gioco.\n");
    fclose(file);
    return 0;
}

recensioni_t *visualizzaRecensioni(long offset) {
    //recupero il gioco dal catalogo tramite l'offset
    gioco_t gioco = ricercaSpecifica(offset);

    unsigned short num_recensioni = 0, capacita = 1;
    // Crea un puntatore a una lista di recensioni
    recensioni_t *recensioni = calloc(capacita, sizeof(recensioni_t));
    
    if (recensioni == NULL) {
        fprintf(stderr, "Errore di allocazione memoria\n");
        return NULL;
    }

    for (unsigned short i = 0; i < MAX_RECENSIONI; i++) {
        //cerco le recensioni non vuote
        if (gioco.recensioni[i].nome_utente[0] != '\0') {
            // Quando trovo una recensione, verifico se è necessario espandere l'array
            if (num_recensioni >= capacita) {
                void **ptr = (void**)&recensioni;
                if (checkMemory(&num_recensioni, &capacita, sizeof(recensioni_t), sizeof(recensioni_t*), &ptr) == 0) {
                    return NULL; // checkMemory ha già liberato la memoria
                }
                recensioni = (recensioni_t*)(*ptr);
            } else {
                // Se non è necessario espandere l'array, incremento manualmente num_recensioni
                num_recensioni++;
            }

            //copio la recensione all'interno della lista
            memcpy(&recensioni[num_recensioni-1], &gioco.recensioni[i], sizeof(recensioni_t));
        }
    }

    //sono state trovare recensioni quindi ritorno il puntatore
    if (num_recensioni > 0) return recensioni;

    //libero l'area di memoria allocata
    free(recensioni);
    return NULL;
}

float calcolaStatistiche(unsigned short mode, long offset) {
    gioco_t gioco = ricercaSpecifica(offset);
    if (mode == 1) return (float) gioco.copie_vendute;

    float media = 0, num_recensioni = 0;
    for (unsigned short i = 0; i < MAX_RECENSIONI; i++) {
        if (gioco.recensioni[i].nome_utente[0] != '\0') {
            media += (float) gioco.recensioni[i].valutazione;
            num_recensioni++;
        }
    }
    return media / num_recensioni;
}

gioco_t *ordinaStatistiche(unsigned short mode) {
    FILE *file = apriCatalogo("rb");
    
    // Ottieni la dimensione del file
    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        return NULL;
    }
    
    long file_size = ftell(file);
    unsigned int num_elementi = 0;
    
    // Verifica che il file non sia vuoto
    if (file_size > 0) {
        num_elementi = file_size / sizeof(gioco_t);
    } else {
        // File vuoto, restituisci NULL
        fclose(file);
        return NULL;
    }
    
    // Alloca memoria per i giochi
    gioco_t *giochi = NULL;
    if (num_elementi > 0) {
        giochi = calloc(num_elementi, sizeof(gioco_t));
        if (giochi == NULL) {
            fprintf(stderr, "Errore di allocazione memoria\n");
            fclose(file);
            return NULL;
        }
        
        // Torna all'inizio del file
        fseek(file, 0, SEEK_SET);
        
        // Leggi tutti i giochi
        for (unsigned int i = 0; i < num_elementi; i++) {
            if(fread(&giochi[i], sizeof(gioco_t), 1, file) != 1){
                fprintf(stderr, "Errore lettura file\n");
                fclose(file);
                free(giochi);
                return NULL;
            }
        }
        
        // Ordina i giochi
        if (num_elementi > 1) {
            ShellSort(giochi, num_elementi, mode);
        }
    }
    
    fclose(file);
    return giochi;
}