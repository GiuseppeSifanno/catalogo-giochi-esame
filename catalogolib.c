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

    //chiudo il file e salvo le modifiche
    fclose(file);
    return 0;
}

unsigned short modificaGioco(const long offset, const gioco_t *gioco) {
    //necessario rb+ per poter modificare il file
    FILE *file = apriCatalogo("rb+");

    //posizione il puntatore nel file in cui si trova il gioco
    if (fseek(file, offset, SEEK_SET) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        exit(-1);
    }

    //sovrascrivo i dati del gioco nel file
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

    //variabile usata per creare il nome del nuovo file
    char temp[20] = "new_";
    strcat(temp, NOME_FILE);

    //apro il nuovo file in modalità scrittura
    FILE *new_file = fopen(temp, "wb");

    if (new_file == NULL) {
        fprintf(stderr, "Errore apertura nuovo file\n");
        fclose(file);
        return 0;
    }

    gioco_t gioco;
    long pos; //posizione del file
    //leggo tutti i giochi dal file e li scrivo nel nuovo file
    while (pos = ftello(file), fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        //scrivo solo i giochi che non corrispondono all'offset
        if (pos != offset) {
            if (fwrite(&gioco, sizeof(gioco_t), 1, new_file) != 1) {
                fprintf(stderr, "Errore scrittura nel nuovo file\n");
                fclose(file);
                fclose(new_file);
                return 0;
            }
        }
    }
    //chiudo i file per salvare le modifiche
    fclose(file);
    fclose(new_file);

    //elimino il file originale
    if (remove(NOME_FILE) != 0) {
        fprintf(stderr, "Errore cancellazione file\n");
        return 0;
    }

    //rinomino il nuovo file con il nome di quello originale
    if (rename(temp, NOME_FILE) != 0) {
        fprintf(stderr, "Errore rinominazione file\n");
        return 0;
    }

    return 1;
}

gioco_t ricercaSpecifica(long offset) {
    //apro il file in modalità lettura
    FILE *file = apriCatalogo("rb");

    //posizione il puntatore nel file in cui si trova il gioco
    if (fseek(file, offset, SEEK_SET) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        exit(-1);
    }

    gioco_t gioco;
    //leggo il gioco dal catalogo e lo salvo
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
    ///array per i dati recuperati durante l'analisi della query
    char **parametri = analisiQuery(query, &num_param);

    if (parametri == NULL) {
        fprintf(stderr, "Errore di analisi query\n");
        exit(-1);
    }

    unsigned short capacita = 1; //capacità iniziale
    *num_elementi = 0; // numero di elementi trovati
    //array per le posizioni dei giochi trovati
    long *offset = calloc(capacita, sizeof(long));

    if (offset == NULL) {
        // Libera la memoria allocata per i parametri
        for (unsigned short i = 0; i < num_param; i++) {
            free(parametri[i]);
        }
        free(parametri);
        fprintf(stderr, "Errore di allocazione memoria\n");
        exit(-1);
    }

    //apro il file in modalità lettura
    FILE *file = apriCatalogo("rb");

    long pos = ftello(file); //salvo la posizione del file corrente

    //leggo tutti i giochi dal file
    while (fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        valido = 0;
        for (unsigned short i = 0; i < num_param; i++) {
            // Crea una copia del parametro per non modificare l'originale
            char param_copy[MAX_CHAR];
            strncpy(param_copy, parametri[i], MAX_CHAR);
            param_copy[MAX_CHAR - 1] = '\0'; // Assicuro la terminazione della stringa

            switch (param_copy[0]) {
                case TOKEN_1: { // $ - Anno di pubblicazione
                    char *value_str = param_copy + 1; // Rimuovi il token iniziale

                    // Conversione della stringa in numero
                    unsigned short anno = (unsigned short)strtol(value_str, NULL, 10);
                    if (gioco.anno_pubblicazione == anno)
                        valido = 1;
                }
                break;
                case TOKEN_2: { // # - Genere
                    char* genere = param_copy + 1; // Rimuovi il token iniziale

                    //Comparo il genere in input con quelli presenti nel gioco
                    for (unsigned short j = 0; j < MAX_GENERI; j++) {
                        if (strstr(gioco.generi[j], genere) != NULL) {
                            //se trovo un match, allora il gioco è valido
                            valido = 1;
                            break;
                        }
                    }
                }
                break;
                default: { // Ricerca generale
                    // Ricerca in tutti i campi del gioco
                    if (strstr(gioco.titolo, parametri[i]) != NULL) valido = 1;
                    if (strstr(gioco.editore, parametri[i]) != NULL) valido = 1;
                    if (strstr(gioco.sviluppatore, parametri[i]) != NULL) valido = 1;
                }
                break;
            }
            if (valido == 1) break;
        }

        if (valido == 1) {
            if (checkMemory(num_elementi, &capacita, sizeof(long), sizeof(long*), (void ***)&offset) == 0) {
                // In caso di errore, libera la memoria e termina
                for (unsigned short i = 0; i < num_param; i++) {
                    free(parametri[i]);
                }
                free(parametri);
                free(offset);
                fclose(file);
                return NULL;
            }
            
            offset[*num_elementi] = pos;
            (*num_elementi)++;
        }
        pos = ftello(file);
    }

    // Libera la memoria allocata per i parametri
    for (unsigned short i = 0; i < num_param; i++) {
        free(parametri[i]);
    }
    free(parametri);

    fclose(file);
    return offset;
}

unsigned short inserisciRecensione(recensioni_t *recensione, long *offset) {
    FILE *file = apriCatalogo("rb+");
    gioco_t gioco = ricercaSpecifica(*offset);

    for (unsigned short i = 0; i < MAX_RECENSIONI; i++) {
        if (gioco.recensioni[i].nome_utente[0] == '\0') {
            fclose(file);
            memcpy(&gioco.recensioni[i], recensione, sizeof(recensioni_t));
            //modifico le informazioni del gioco con la nuova recensione
            modificaGioco(*offset, &gioco);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

recensioni_t *visualizzaRecensioni(long offset, unsigned short *num_recensioni) {
    //recupero il gioco dal catalogo tramite l'offset
    gioco_t gioco = ricercaSpecifica(offset);

    unsigned short capacita = 1;
    // Crea un puntatore a una lista di recensioni
    recensioni_t *recensioni = calloc(capacita, sizeof(recensioni_t));
    *num_recensioni = 0;

    if (recensioni == NULL) {
        fprintf(stderr, "Errore di allocazione memoria\n");
        return NULL;
    }

    for (unsigned short i = 0; i < MAX_RECENSIONI; i++) {
        //cerco le recensioni non vuote
        if (gioco.recensioni[i].nome_utente[0] != '\0') {
            // Quando trovo una recensione, verifico se è necessario espandere l'array
            if (*num_recensioni >= capacita) {
                void **ptr = (void**)&recensioni;
                if (checkMemory(num_recensioni, &capacita, sizeof(recensioni_t), sizeof(recensioni_t*), &ptr) == 0) {
                    return NULL; // checkMemory ha già liberato la memoria
                }
                //casting
                recensioni = (recensioni_t*) *ptr;
            }
            memcpy(&recensioni[*num_recensioni], &gioco.recensioni[i], sizeof(recensioni_t));
            (*num_recensioni)++;
        }
    }

    //sono state trovate recensioni quindi ritorno il puntatore
    if (*num_recensioni > 0) return recensioni;

    //libero l'area di memoria allocata
    free(recensioni);
    return NULL;
}

float calcolaStatistiche(gioco_t *gioco) {
    float media = 0, num_recensioni = 0;
    for (unsigned short i = 0; i < MAX_RECENSIONI; i++) {
        //cerco le recensioni non vuote
        if (gioco -> recensioni[i].nome_utente[0] != '\0') {
            media += (float) gioco -> recensioni[i].valutazione;
            num_recensioni++;
        }
    }
    if (num_recensioni == 0) return 0;

    return media / num_recensioni;
}

gioco_t *ordinaStatistiche(unsigned short mode, unsigned long *num_elementi) {
    FILE *file = apriCatalogo("rb");
    
    // Ottieni la dimensione del file
    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        return NULL;
    }
    
    long file_size = ftell(file);
    
    // Verifica che il file non sia vuoto
    if (file_size > 0) {
        *num_elementi = file_size / sizeof(gioco_t);
    } else {
        // File vuoto, restituisci NULL
        fclose(file);
        return NULL;
    }
    
    // Alloca memoria per i giochi
    gioco_t *giochi = NULL;
    if (*num_elementi > 0) {
        giochi = calloc(*num_elementi, sizeof(gioco_t));
        if (giochi == NULL) {
            fprintf(stderr, "Errore di allocazione memoria\n");
            fclose(file);
            return NULL;
        }
        
        // Torna all'inizio del file
        fseek(file, 0, SEEK_SET);
        
        // Leggi tutti i giochi
        for (unsigned int i = 0; i < *num_elementi; i++) {
            if(fread(&giochi[i], sizeof(gioco_t), 1, file) != 1){
                fprintf(stderr, "Errore lettura file\n");
                fclose(file);
                free(giochi);
                return NULL;
            }
        }

        fclose(file);
        // Ordina i giochi
        shellSort(giochi, num_elementi, mode);
        return giochi;
    }
    
    fclose(file);
    return NULL;
}

unsigned short acquistaGioco(long offset) {
    FILE *file = apriCatalogo("rb");
    gioco_t gioco;

    if (fseek(file, offset, SEEK_SET) != 0) {
        fprintf(stderr, "Errore posizione file\n");
        fclose(file);
        exit(-1);
    }

    if (fread(&gioco, sizeof(gioco_t), 1, file) != 1) {
        fprintf(stderr, "Errore lettura file\n");
        fclose(file);
        exit(-1);
    }

    //chiudo il file poiché non più necessario
    fclose(file);

    //incremento il numero di copie vendute del gioco
    gioco.copie_vendute++;

    //modifico il gioco con le nuove informazioni nel catalogo
    if (modificaGioco(offset, &gioco) == 1) return 1;

    return 0;
}
