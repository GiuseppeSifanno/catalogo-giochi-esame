//
// Created by Giuseppe on 23/05/2025.
//

#ifndef CATALOGO_UTILITY_H
#define CATALOGO_UTILITY_H

#include "catalogolib.h"
#include <stdio.h>

#ifndef DELIM
/**
 * @brief Delimitatore usato per separare i parametri inseriti all'interno della query
 */
#define DELIM ","
#endif

#ifndef TOKEN_1
/**
 * @brief Token usato per identificare un parametro che rappresenta l'anno di pubblicazione del gioco
 */
#define TOKEN_1 '$'
#endif

#ifndef TOKEN_2
/**
 * @brief Token usato per identificare un parametro che rappresenta il genere del gioco
 */
#define TOKEN_2 '#'
#endif

/**
 * @brief Si occupa di analizzare la stringa in entrata, tutti i parametri identificati verranno poi
 *  memorizzati all'interno di una struttura e restituiti
 * @param [in] query Stringa utilizzata per la ricerca, da analizzare
 * @param [out] param Numero dei parametri recuperati dalla query
 * @return Struttura contenente tutti i parametri divisi tra di loro
 */
char** analisiQuery(char query[MAX_CHAR], unsigned short *param);

/**
 * @brief Rimuove spazi all'inizio e alla fine della stringa passata in ingresso
 * @param [in, out] token Carattere di una stringa
 */
void trim(char *token);

/**
 * @brief Gestisce l'allocazione dinamica della memoria per gli array composti da elementi generici.
 *      Verifica se è necessario espandere un array e alloca memoria per un nuovo elemento.
 *
 * @param num_elementi Puntatore al numero attuale di elementi nell'array
 * @param capacita Puntatore alla capacità attuale dell'array (verrà incrementata, di 1, se necessario)
 * @param dimensione_elemento Dimensione in byte del nuovo elemento da allocare
 * @param dimensione_puntatore Dimensione in byte di ciascun puntatore nell'array (es. sizeof(int*))
 * @param array Puntatore triplo all'array da gestire
 * @returns
 *  @return 1 se l'allocazione è avvenuta con successo
 *  @return 0 in caso di errore
 */
int checkMemory(unsigned short *num_elementi, unsigned short *capacita,
    unsigned long dimensione_elemento, unsigned long dimensione_puntatore, void ***array);

/**
 * @brief Acquisizione dei dati di un nuovo gioco
 * @return Istanza del nuovo gioco inserito
 */
gioco_t inserisciGioco();

/**
 * @brief Apre il catalogo ed effettua i controlli necessari
 * @param mode Modalità con cui aprire il file, coerenti ai tipi accettati dalla funzione fopen()
 * @return Puntatore al file appena aperto
 */
FILE *apriCatalogo(char mode[3]);

/**
 * @brief Algoritmo di ordinamento per copie vendute o per media valutazione
 *
 * @param [in, out] giochi Array di giochi da ordinare
 * @param [in] dim Dimensione dell'array
 * @param [in] mode Modalità di ordinamento (copie vendute o media valutazione)
 *
 */
void ShellSort(gioco_t *giochi, unsigned int dim, unsigned short mode);

#endif //CATALOGO_UTILITY_H
