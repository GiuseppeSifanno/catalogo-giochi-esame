#include <string.h>
#include "gioco.h"
#include "catalogolib.h"

void caricaGiochi() {
    gioco_t giochi[20] = {
    {
        .copie_vendute = 1000000,
        .anno_pubblicazione = 2020,
        .recensioni = {0},
        .titolo = "cyberquest",
        .editore = "future games",
        .sviluppatore = "future devs",
        .descrizione = "un'avventura futuristica nel mondo virtuale.",
        .generi = { "azione", "avventura" }
    },
    {
        .copie_vendute = 500000,
        .anno_pubblicazione = 2018,
        .recensioni = {0},
        .titolo = "castle siege",
        .editore = "medieval studios",
        .sviluppatore = "knight works",
        .descrizione = "difendi il castello contro ondate di nemici.",
        .generi = { "strategia", "rts" }
    },
    {
        .copie_vendute = 750000,
        .anno_pubblicazione = 2019,
        .recensioni = {0},
        .titolo = "mystic forest",
        .editore = "enigma games",
        .sviluppatore = "mystic devs",
        .descrizione = "esplora una foresta magica piena di misteri.",
        .generi = { "rpg", "fantasy" }
    },
    {
        .copie_vendute = 1200000,
        .anno_pubblicazione = 2021,
        .recensioni = {0},
        .titolo = "speed racer",
        .editore = "fast wheels",
        .sviluppatore = "turbo studio",
        .descrizione = "corse ad alta velocità in circuiti mozzafiato.",
        .generi = { "corse", "simulazione" }
    },
    {
        .copie_vendute = 300000,
        .anno_pubblicazione = 2017,
        .recensioni = {0},
        .titolo = "puzzle world",
        .editore = "brainy games",
        .sviluppatore = "puzzle masters",
        .descrizione = "sfide logiche per mettere alla prova la tua mente.",
        .generi = { "puzzle", "educativo" }
    },
    {
        .copie_vendute = 600000,
        .anno_pubblicazione = 2020,
        .recensioni = {0},
        .titolo = "galactic wars",
        .editore = "spacex games",
        .sviluppatore = "star devs",
        .descrizione = "battaglie spaziali epiche per il controllo della galassia.",
        .generi = { "strategia", "sci-fi" }
    },
    {
        .copie_vendute = 450000,
        .anno_pubblicazione = 2019,
        .recensioni = {0},
        .titolo = "zombie escape",
        .editore = "survival studios",
        .sviluppatore = "undead devs",
        .descrizione = "sopravvivi all'apocalisse zombie con astuzia e coraggio.",
        .generi = { "horror", "survival" }
    },
    {
        .copie_vendute = 950000,
        .anno_pubblicazione = 2021,
        .recensioni = {0},
        .titolo = "dragon slayer",
        .editore = "epic games",
        .sviluppatore = "dragon devs",
        .descrizione = "unisciti all'epica battaglia contro i draghi.",
        .generi = { "rpg", "fantasy" }
    },
    {
        .copie_vendute = 800000,
        .anno_pubblicazione = 2018,
        .recensioni = {0},
        .titolo = "sky fighters",
        .editore = "air combat corp",
        .sviluppatore = "flight devs",
        .descrizione = "combattimenti aerei con aerei da caccia moderni.",
        .generi = { "azione", "simulazione" }
    },
    {
        .copie_vendute = 350000,
        .anno_pubblicazione = 2016,
        .recensioni = {0},
        .titolo = "deep sea",
        .editore = "oceanic games",
        .sviluppatore = "blue devs",
        .descrizione = "esplora le profondità dell'oceano e scopri i suoi segreti.",
        .generi = { "avventura", "esplorazione" }
    },
    {
        .copie_vendute = 700000,
        .anno_pubblicazione = 2019,
        .recensioni = {0},
        .titolo = "alien invasion",
        .editore = "cosmic games",
        .sviluppatore = "alien devs",
        .descrizione = "difendi la terra dall'invasione aliena.",
        .generi = { "azione", "sci-fi" }
    },
    {
        .copie_vendute = 650000,
        .anno_pubblicazione = 2020,
        .recensioni = {0},
        .titolo = "stealth ops",
        .editore = "covert studios",
        .sviluppatore = "shadow devs",
        .descrizione = "missioni segrete dietro le linee nemiche.",
        .generi = { "stealth", "azione" }
    },
    {
        .copie_vendute = 400000,
        .anno_pubblicazione = 2017,
        .recensioni = {0},
        .titolo = "farm life",
        .editore = "country games",
        .sviluppatore = "green devs",
        .descrizione = "gestisci la tua fattoria e coltiva raccolti.",
        .generi = { "simulazione", "casual" }
    },
    {
        .copie_vendute = 900000,
        .anno_pubblicazione = 2021,
        .recensioni = {0},
        .titolo = "robo battle",
        .editore = "tech games",
        .sviluppatore = "robot devs",
        .descrizione = "combatti con robot avanzati in arene futuristiche.",
        .generi = { "azione", "multiplayer" }
    },
    {
        .copie_vendute = 550000,
        .anno_pubblicazione = 2018,
        .recensioni = {0},
        .titolo = "magic academy",
        .editore = "fantasy studios",
        .sviluppatore = "wizard devs",
        .descrizione = "impara magie e incantesimi nella scuola di magia.",
        .generi = { "rpg", "fantasy" }
    },
    {
        .copie_vendute = 300000,
        .anno_pubblicazione = 2016,
        .recensioni = {0},
        .titolo = "city builder",
        .editore = "urban games",
        .sviluppatore = "builder devs",
        .descrizione = "costruisci e gestisci la tua città.",
        .generi = { "strategia", "simulazione" }
    },
    {
        .copie_vendute = 1000000,
        .anno_pubblicazione = 2019,
        .recensioni = {0},
        .titolo = "space miner",
        .editore = "galactic studios",
        .sviluppatore = "miner devs",
        .descrizione = "estrai risorse in asteroidi nello spazio profondo.",
        .generi = { "strategia", "sci-fi" }
    },
    {
        .copie_vendute = 850000,
        .anno_pubblicazione = 2020,
        .recensioni = {0},
        .titolo = "battle royale x",
        .editore = "extreme games",
        .sviluppatore = "battle devs",
        .descrizione = "combattimenti all'ultimo sangue in una mappa gigante.",
        .generi = { "azione", "multiplayer" }
    },
    {
        .copie_vendute = 450000,
        .anno_pubblicazione = 2017,
        .recensioni = {0},
        .titolo = "jungle run",
        .editore = "adventure works",
        .sviluppatore = "runner devs",
        .descrizione = "corri e salta attraverso giungle pericolose.",
        .generi = { "platform", "avventura" }
    },
    {
        .copie_vendute = 250000,
        .anno_pubblicazione = 2015,
        .recensioni = {0},
        .titolo = "retro racer",
        .editore = "classic games",
        .sviluppatore = "retro devs",
        .descrizione = "gare arcade in stile anni '80.",
        .generi = { "corse", "arcade" }
    }
};

    for (int i = 0; i < 20; i++) {
        aggiungiGioco(giochi[i]);
    }
}


