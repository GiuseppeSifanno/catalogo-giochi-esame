/**
 * @file catalogolib.h
 * @brief Header della libreria di funzioni per il catalogo dei giochi
 */
#ifndef CATALOGOLIB_H
#define CATALOGOLIB_H

#include "gioco.h"
#include "utility.h"
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NOME_FILE
/**
 *  @brief Nome del file del catalogo
 */
#define NOME_FILE "catalogo.dat"
#endif

#ifndef MODE_1
/**
 *  @brief Modalità di visualizzazione delle statistiche per copie vendute.
 */
#define MODE_1 1
#endif

#ifndef MODE_2
/**
 *  @brief Modalità di visualizzazione delle statistiche per media valutazione.
 */
#define MODE_2 2
#endif

#ifndef MIN_VALUTAZIONE
/**
 * @brief Valore minimo della valutazione di un gioco
 */
#define MIN_VALUTAZIONE 1
#endif

#ifndef MAX_VALUTAZIONE
/**
 * @brief Valore massimo della valutazione di un gioco
 */
#define MAX_VALUTAZIONE 5
#endif

/**
 *  @brief Aggiunge un gioco all'interno del catalogo
 *  @param [in] gioco Istanza del gioco da aggiungere
 *  @returns
 *      @return 1 se il gioco è <b>già presente</b>
 *      @return 0 se il gioco è stato aggiunto
 */
unsigned short aggiungiGioco(gioco_t gioco);

/**
 *  @brief Modifica il contenuto di un gioco contenuto all'interno del file
 *  @param [in] offset Posizione del blocco nel file in cui si trova il gioco
 *  @param [in] gioco Istanza del gioco contenente le nuove informazioni
 *  @returns
 *      @return 1 se il gioco è stato modificato
 *      @return 0 se il gioco <b>non</b> è stato modificato
 *
 */
unsigned short modificaGioco(long offset, const gioco_t *gioco);

/**
 * @brief Cancella un record all'interno del file
 * @param [in] offset Posizione del blocco nel file in cui si trova il gioco
 * @returns
 *      @return 1 se il gioco è stato cancellato
 *      @return 0 se il gioco <b>non</b> è stato cancellato o si è verificato qualche problema
 */
unsigned short cancellaGioco(long offset);

/**
 *  @brief Controlla se un gioco è già presente o no nel file
 *  @param [in] new_gioco Istanza del nuovo gioco da inserire
 *  @returns
 *      @return 1 se il gioco è già presente nel catalogo
 *      @return 0 se il gioco <b>non</b> è presente nel catalogo
 */
unsigned short isAlredyAdded(gioco_t new_gioco);

/**
 * @brief Ricerca un gioco specifico
 * @param [in] offset Posizione del blocco nel file in cui si trova il gioco
 * @return Istanza del gioco recuperato
 */
gioco_t ricercaSpecifica(long offset);

/**
 * @brief Effettua una ricerca su tutto il file in base ai parametri inseriti in ingresso dall'utente
 * @param [in] query Stringa utilizzata per la ricerca
 * @param [out] num_elementi Numero degli elementi recuperati
 * @return Puntatore alla lista con la posizione di ogni gioco
 */
long *ricercaGlobale(char query[MAX_CHAR], unsigned short *num_elementi);

/**
 * @brief La funzione inserisce una recensione per un gioco nel file
 * @param [in] recensione Recensione inserita dall'utente
 * @param [in] offset Posizione del blocco nel file in cui si trova il gioco
 * @returns
 *  @return 1 se la recensione è stata inserita correttamente
 *  @return 0 se la recensione <b>non</b> è stata inserita
 */
unsigned short inserisciRecensione(recensioni_t *recensione, long *offset);

/**
 * @brief Recupera le recensioni di un gioco specifico
 * @param [in] offset Posizione del blocco nel file in cui si trova il gioco
 * @param [out] num_recensioni  Numero di recensioni del gioco
 * @returns
 *  @return Puntatore alla lista con le recensioni
 *  @return NULL se non ci sono recensioni per il gioco o si è verificato qualche problema
 */
recensioni_t *visualizzaRecensioni(long offset, unsigned short *num_recensioni);

/**
 * @brief Calcola la media delle valutazioni del gioco selezionato
 * @param [in] gioco Istanza del gioco
 *
 * @returns
 *  @return Valore delle statistiche di un gioco
 *  @return 0 se non ci sono recensioni
 */
float calcolaStatistiche(gioco_t *gioco);

/**
 * @brief Ordina tutti i giochi all'interno del catalogo, in base alla modalità specificata in ingresso
 * @param [in] mode Modalità di visualizzazione delle statistiche (copie vendute o media valutazione).
 * 1 per le copie vendute
 * 2 per la media valutazione
 * @param [out] num_elementi Numero di giochi totali recuperati dal catalogo
 * @return Puntatore alla lista con i giochi ordinati
 */
gioco_t *ordinaStatistiche(unsigned short mode, unsigned int *num_elementi);

/**
 * @brief Acquista il gioco e incrementa il numero delle @ref gioco_t::copie_vendute "copie_vendute"
 * @param [in] offset Posizione del blocco nel file in cui si trova il gioco
 * @returns
 *  @return 1 se l'acquisto è andato a buon fine
 *  @return 0 se l'acquisto <b>non</b> è andato a buon fine
 */
unsigned short acquistaGioco(long offset);

#endif //CATALOGOLIB_H
