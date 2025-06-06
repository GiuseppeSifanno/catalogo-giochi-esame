/**
 * @file gioco.h
 * @brief Definizione delle strutture e dei tipi utilizzati per la gestione dei dati di un gioco all'interno del catalogo.
 */
#ifndef GIOCO_H
#define GIOCO_H

#include <stdint.h>

#ifndef MAX_CHAR
/**
 * @brief Lunghezza massima caratteri
 */
#define MAX_CHAR 255
#endif

#ifndef MAX_GENERI
/**
 * @brief Numero massimo di generi
 */
#define MAX_GENERI 10
#endif

#ifndef MAX_RECENSIONI
/**
 * @brief Numero massimo di recensioni
 */
#define MAX_RECENSIONI 10
#endif

#ifndef ANNO_MIN
/**
 * @brief Anno minimo di pubblicazione
 */
#define ANNO_MIN 1950
#endif

/**
 * Questa struttura è utilizzata per memorizzare le recensioni inserite dagli utenti,
 * includendo il loro nome utente, una descrizione della recensione e infine una valutazione numerica
 *
 * @struct recensioni
 *
 * @brief Struttura che memorizza le recensioni degli utenti.
 *
 * @typedef recensioni_t
 */
typedef struct recensioni {
    /**
     * Nome utente di chi ha inserito la recensione
     */
    char nome_utente[MAX_CHAR];
    /**
     * Rappresentazione numerica della valutazione
     */
    uint8_t valutazione;
    /**
     * Rappresenta la descrizione fornita dall'utente al momento della recensione
     * Più in particolare contiene commenti e/o feedback. L'inserimento di questo campo è <b>opzionale</b>
     */
    char descrizione [MAX_CHAR];
}recensioni_t;

/**
 * @struct gioco
 *
 * @brief Struttura che memorizza i dati di un gioco.
 *
 * Questa struttura è utilizzata per memorizzare i dati di un gioco,
 * includendo il titolo, l'editore, lo sviluppatore, la descrizione, l'anno di pubblicazione,
 * il numero di copie vendute, i generi del gioco e le sue recensioni.
 *
 * @typedef gioco_t
 */
typedef struct gioco {
    /**
     * Numero di copie vendute del gioco
     */
    unsigned long copie_vendute;
    /**
     * Anno di pubblicazione del gioco
     */
    unsigned short anno_pubblicazione;
    /**
     *Struttura per le recensioni del gioco.
     * @see recensioni_t
     */
    recensioni_t recensioni[MAX_RECENSIONI];
    /**
     * Titolo del gioco
     */
    char titolo [MAX_CHAR];
    /**
     * Editore del gioco
     */
    char editore [MAX_CHAR];
    /**
     * Sviluppatore del gioco
     */
    char sviluppatore [MAX_CHAR];
    /**
     * Descrizione del gioco
     */
    char descrizione [MAX_CHAR];
    /**
     * Generi del gioco
     */
    char generi[MAX_GENERI][MAX_CHAR];
}gioco_t;

/**
 *
 */
void caricaGiochi();

#endif //GIOCO_H
