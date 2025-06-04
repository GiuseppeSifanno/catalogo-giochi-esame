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
    FILE *file = apriCatalogo("wb+");

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

    char temp[20] = "new_";
    strcat(temp, NOME_FILE);
    FILE *new_file = fopen(temp, "wb");

    if (new_file == NULL) {
        fprintf(stderr, "Errore apertura nuovo file\n");
        fclose(file);
        return 0;
    }

    gioco_t gioco;

    long pos;
    while (pos = ftello(file), fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        if (pos != offset) {
            if (fwrite(&gioco, sizeof(gioco_t), 1, new_file) != 1) {
                fprintf(stderr, "Errore scrittura nel nuovo file\n");
                fclose(file);
                fclose(new_file);
                return 0;
            }
        }
    }

    fclose(file);
    fclose(new_file);

    if (remove(NOME_FILE) != 0) {
        fprintf(stderr, "Errore cancellazione file\n");
        return 0;
    }

    if (rename(temp, NOME_FILE) != 0) {
        fprintf(stderr, "Errore rinominazione file\n");
        return 0;
    }

    return 1;
}

unsigned short isAlredyAdded(gioco_t new_gioco) {
    //apro il file in modalità lettura
    FILE *file = apriCatalogo("rb");
    gioco_t gioco;

    //1 non presente nel catalogo, 0 altrimenti
    unsigned short int valido = 1;

    while (fread(&gioco, sizeof(gioco_t), 1, file) == 1) {
        // Controllo sui campi principali
        if (gioco.anno_pubblicazione == new_gioco.anno_pubblicazione    &&
            gioco.copie_vendute == new_gioco.copie_vendute              &&
            strcmp(gioco.titolo, new_gioco.titolo) == 0                 &&
            strcmp(gioco.descrizione, new_gioco.descrizione) == 0       &&
            strcmp(gioco.editore, new_gioco.editore) == 0               &&
            strcmp(gioco.sviluppatore, new_gioco.sviluppatore) == 0) {
            
            // Controllo su tutti i generi
            valido = 0; // Presupponiamo che sia lo stesso gioco
            
            // Se almeno un genere è diverso, allora non è lo stesso gioco
            for (unsigned short i = 0; i < MAX_GENERI; i++) {
                if (strcmp(gioco.generi[i], new_gioco.generi[i]) != 0) {
                    valido = 1;
                    break;
                }
            }

            //se non è valido, quindi è già presente nel catalogo
            if (valido == 0) {
                fclose(file);
                return 1; // Gioco già presente
            }
        }
    }
    
    fclose(file);
    return 0; // Gioco non presente quindi valido = 1
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
        fprintf(stderr, "Errore di analisi query\n");
        exit(-1);
    }

    unsigned short capacita = 1; //capacità iniziale
    *num_elementi = 0; // Inizializza a 0
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
                    char *value_str = param_copy + 1; // Rimuovi il token iniziale

                    unsigned short anno = (unsigned short)strtol(value_str, NULL, 10); // Converti la stringa in numero
                    if (gioco.anno_pubblicazione == anno)
                        valido = 1;
                }
                break;
                case TOKEN_2: { // # - Genere
                    char* genere = param_copy + 1; // Rimuovi il token iniziale

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
        if (gioco -> recensioni[i].nome_utente[0] != '\0') {
            media += (float) gioco -> recensioni[i].valutazione;
            num_recensioni++;
        }
    }
    if (num_recensioni == 0) return 0;

    return media / num_recensioni;
}

gioco_t *ordinaStatistiche(unsigned short mode, unsigned int *num_elementi) {
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
        
        // Ordina i giochi
        if (*num_elementi > 1) {
            shellSort(giochi, num_elementi, mode);
        }
    }
    
    fclose(file);
    return giochi;
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
