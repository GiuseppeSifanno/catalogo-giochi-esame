//
// Created by Graziano Semerano on 22/05/25.
//

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

#ifndef ANNO_MIN
/**
 * @brief Anno minimo di pubblicazione
 */
#define ANNO_MIN 1950
#endif

/**
 * @struct recensioni
 * @brief Struttura che memorizza le recensioni degli utenti.
 *
 * Questa struttura è utilizzata per memorizzare le recensioni inserite dagli utenti,
 * includendo il loro nome utente, una descrizione della recensione ed infine una valutazione numerica
 *
 * @typedef recensioni_t
 *
 * @var char recensioni::nome_utente
 * Nome utente di chi ha inserito la recensione
 *
 * @var uint8_t recensioni::valutazione
 * Rappresentazione numerica della valutazione
 *
 * @var char recensioni::descrizione
 * Rappresenta la descrizione fornita dall'utente al momento della recensione
 * Più in particolare contiene commenti e/o feedback. L'inserimento di questo campo è <b>opzionale</b>
 */
typedef struct recensioni {
    char nome_utente[MAX_CHAR];
    uint8_t valutazione;
    char descrizione [MAX_CHAR];
}recensioni_t;

/**
 * @struct gioco
 * @brief Struttura che memorizza i dati di un gioco.
 *
 * Questa struttura è utilizzata per memorizzare i dati di un gioco,
 * includendo il titolo, l'editore, lo sviluppatore, la descrizione, l'anno di pubblicazione,
 * il numero di copie vendute, i generi del gioco e le sue recensioni.
 *
 * @typedef gioco_t
 *
 * @var char gioco::titolo
 * Titolo del gioco
 *
 * @var char gioco::editore
 * Editore del gioco
 *
 * @var char gioco::sviluppatore
 * Sviluppatore del gioco
 *
 * @var char gioco::descrizione
 * Descrizione del gioco
 *
 * @var unsigned short gioco::anno_pubblicazione
 * Anno di pubblicazione del gioco
 *
 * @var unsigned long gioco::copie_vendute
 * Numero di copie vendute del gioco
 *
 * @var char gioco::generi
 * Lista di generi attribuiti al gioco
 *
 * @var recensioni_t recensioni
 * Rifarsi a @c
 */
typedef struct gioco {
    char titolo [MAX_CHAR];
    char editore [MAX_CHAR];
    char sviluppatore [MAX_CHAR];
    char descrizione [MAX_CHAR];
    unsigned short anno_pubblicazione;
    unsigned long copie_vendute;
    char generi[MAX_GENERI][MAX_CHAR];
    recensioni_t recensioni;
}gioco_t;

#endif //GIOCO_H
