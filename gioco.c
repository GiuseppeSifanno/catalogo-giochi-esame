#include <string.h>
#include "gioco.h"
#include "catalogolib.h"

void caricaGiochi() {
	gioco_t giochi[20] = {
		{
			.copie_vendute = 350000000,
			.anno_pubblicazione = 2011,
			.titolo = "minecraft",
			.editore = "mojang",
			.sviluppatore = "mojang",
			.descrizione = "un sandbox creativo dove costruire e sopravvivere in un mondo generato proceduralmente.",
			.generi = {"sandbox", "avventura"},
			.recensioni = {
				{.nome_utente = "block_master", .valutazione = 5, .descrizione = "gioco infinito, sempre qualcosa da fare!"},
				{.nome_utente = "creative_girl", .valutazione = 4, .descrizione = "adoro costruire ma manca una vera storia."},
				{.nome_utente = "redstone_pro", .valutazione = 5}
			}
		},
		{
			.copie_vendute = 240000000,
			.anno_pubblicazione = 2013,
			.titolo = "grand theft auto v",
			.editore = "rockstar games",
			.sviluppatore = "rockstar north",
			.descrizione = "open world criminale con missioni, libertà d’azione e satira sociale.",
			.generi = {"azione", "open world"},
			.recensioni = {
				{.nome_utente = "los_santos_king", .valutazione = 5, .descrizione = "grafica incredibile e trama avvincente."},
				{.nome_utente = "stealth_driver", .valutazione = 4},
				{.nome_utente = "gtafan89", .valutazione = 3, .descrizione = "online divertente ma pieno di hacker."}
			}
		},
		{
			.copie_vendute = 480000000,
			.anno_pubblicazione = 1996,
			.titolo = "pokemon rosso",
			.editore = "nintendo",
			.sviluppatore = "game freak",
			.descrizione = "cattura e allena pokémon per diventare il miglior allenatore.",
			.generi = {"rpg", "avventura"},
			.recensioni = {
				{.nome_utente = "trainer_ash", .valutazione = 5, .descrizione = "nostalgia pura, una pietra miliare."},
				{.nome_utente = "pokehunter", .valutazione = 4},
				{.nome_utente = "johto_fan", .valutazione = 3, .descrizione = "bello, ma preferisco la seconda generazione."}
			}
		},
		{
			.copie_vendute = 170000000,
			.anno_pubblicazione = 2017,
			.titolo = "playerunknown's battlegrounds",
			.editore = "kakao games",
			.sviluppatore = "pubg corporation",
			.descrizione = "sopravvivi in una battle royale dove vince l'ultimo rimasto.",
			.generi = {"sparatutto", "multiplayer"},
			.recensioni = {
				{.nome_utente = "sniper_queen", .valutazione = 4, .descrizione = "buono ma alcuni bug frustrano."},
				{.nome_utente = "lastmanstanding", .valutazione = 5},
				{.nome_utente = "casualgamer99", .valutazione = 2, .descrizione = "difficile per chi è alle prime armi."}
			}
		},
		{
			.copie_vendute = 100000000,
			.anno_pubblicazione = 2017,
			.titolo = "fortnite",
			.editore = "epic games",
			.sviluppatore = "epic games",
			.descrizione = "battle royale colorata con costruzioni e eventi stagionali.",
			.generi = {"sparatutto", "multiplayer"},
			.recensioni = {
				{.nome_utente = "buildpro123", .valutazione = 5},
				{.nome_utente = "xno_scopex", .valutazione = 4, .descrizione = "divertente ma troppo caotico a volte."},
				{
					.nome_utente = "oldschooler", .valutazione = 2,
					.descrizione = "preferivo la prima versione, ora è troppo commerciale."
				}
			}
		},
		{
			.copie_vendute = 32000000,
			.anno_pubblicazione = 2022,
			.titolo = "elden ring",
			.editore = "bandai namco",
			.sviluppatore = "fromsoftware",
			.descrizione = "action rpg open world con combattimenti impegnativi e ambientazione dark fantasy.",
			.generi = {"rpg", "azione"},
			.recensioni = {
				{
					.nome_utente = "tarnished_soul", .valutazione = 5,
					.descrizione = "capolavoro assoluto, difficile ma gratificante."
				},
				{.nome_utente = "casual_knight", .valutazione = 3},
				{.nome_utente = "lore_lover", .valutazione = 4, .descrizione = "lore affascinante, ma serviva una guida!"}
			}
		},
		{
			.copie_vendute = 60000000,
			.anno_pubblicazione = 2020,
			.titolo = "animal crossing: new horizons",
			.editore = "nintendo",
			.sviluppatore = "nintendo",
			.descrizione = "costruisci la tua isola, interagisci con abitanti e rilassati in un mondo tranquillo.",
			.generi = {"simulazione", "casual"},
			.recensioni = {
				{
					.nome_utente = "islandqueen", .valutazione = 5,
					.descrizione = "perfetto per rilassarsi dopo una giornata stressante."
				},
				{.nome_utente = "nookfan", .valutazione = 4},
				{.nome_utente = "fastgamer", .valutazione = 2, .descrizione = "troppo lento per i miei gusti."}
			}
		},
		{
			.copie_vendute = 17000000,
			.anno_pubblicazione = 2015,
			.titolo = "the witcher 3: wild hunt",
			.editore = "cd projekt",
			.sviluppatore = "cd projekt red",
			.descrizione = "gioco di ruolo con una trama profonda, scelte morali e un mondo dettagliato.",
			.generi = {"rpg", "fantasy"},
			.recensioni = {
				{
					.nome_utente = "geralt_of_rivia", .valutazione = 5,
					.descrizione = "la storia è eccezionale, e il mondo vivo."
				},
				{.nome_utente = "storyseeker", .valutazione = 5},
				{.nome_utente = "fps_fan", .valutazione = 3, .descrizione = "ottimo ma preferisco i giochi in prima persona."}
			}
		},
		{
			.copie_vendute = 110000000,
			.anno_pubblicazione = 2006,
			.titolo = "wii sports",
			.editore = "nintendo",
			.sviluppatore = "nintendo",
			.descrizione = "raccolta di sport interattivi pensati per il movimento con il wiimote.",
			.generi = {"sport", "casual"},
			.recensioni = {
				{.nome_utente = "familygamer", .valutazione = 5, .descrizione = "giocato con tutta la famiglia per anni."},
				{.nome_utente = "bowlingking", .valutazione = 4},
				{.nome_utente = "hardcoreguy", .valutazione = 2, .descrizione = "troppo semplice e ripetitivo."}
			}
		},
		{
			.copie_vendute = 44000000,
			.anno_pubblicazione = 2011,
			.titolo = "terraria",
			.editore = "re-logic",
			.sviluppatore = "re-logic",
			.descrizione = "esplora, costruisci e combatti in un mondo 2d generato proceduralmente.",
			.generi = {"sandbox", "azione"},
			.recensioni = {
				{.nome_utente = "builderjoe", .valutazione = 5},
				{
					.nome_utente = "bosskiller", .valutazione = 5,
					.descrizione = "i boss sono epici e la progressione è fantastica."
				},
				{.nome_utente = "casualminer", .valutazione = 3}
			}
		},
		{
			.copie_vendute = 20000000,
			.anno_pubblicazione = 2017,
			.titolo = "the legend of zelda: breath of the wild",
			.editore = "nintendo",
			.sviluppatore = "nintendo",
			.descrizione = "open world innovativo con esplorazione libera e combattimenti dinamici.",
			.generi = {"avventura", "rpg"},
			.recensioni = {
				{.nome_utente = "hyrulefan", .valutazione = 5, .descrizione = "un'esperienza magica e immersiva."},
				{.nome_utente = "link4ever", .valutazione = 5},
				{.nome_utente = "puzzlehater", .valutazione = 3, .descrizione = "troppi enigmi, poca azione."}
			}
		},
		{
			.copie_vendute = 34000000,
			.anno_pubblicazione = 2020,
			.titolo = "call of duty: warzone",
			.editore = "activision",
			.sviluppatore = "infinity ward",
			.descrizione = "battle royale frenetico con armi realistiche e modalità a squadre.",
			.generi = {"sparatutto", "multiplayer"},
			.recensioni = {
				{.nome_utente = "warzonegod", .valutazione = 4},
				{.nome_utente = "sniperwolf", .valutazione = 5, .descrizione = "molto competitivo, adoro la modalità gulag."},
				{.nome_utente = "lagmaster", .valutazione = 2, .descrizione = "server spesso instabili."}
			}
		},
		{
			.copie_vendute = 18000000,
			.anno_pubblicazione = 2016,
			.titolo = "overwatch",
			.editore = "blizzard",
			.sviluppatore = "blizzard",
			.descrizione = "sparatutto a squadre con eroi unici e gameplay cooperativo.",
			.generi = {"sparatutto", "multiplayer"},
			.recensioni = {
				{.nome_utente = "teamhealer", .valutazione = 5},
				{.nome_utente = "dpsmain", .valutazione = 4, .descrizione = "bilanciamento non sempre perfetto."},
				{.nome_utente = "saltypayload", .valutazione = 3}
			}
		},
		{
			.copie_vendute = 12500000,
			.anno_pubblicazione = 2019,
			.titolo = "sekiro: shadows die twice",
			.editore = "activision",
			.sviluppatore = "fromsoftware",
			.descrizione = "action game hardcore ambientato in un giappone fantasy feudale.",
			.generi = {"azione", "soulslike"},
			.recensioni = {
				{.nome_utente = "parrymaster", .valutazione = 5, .descrizione = "difficile ma incredibilmente soddisfacente."},
				{.nome_utente = "ragequitter", .valutazione = 2},
				{.nome_utente = "feudalninja", .valutazione = 4}
			}
		},
		{
			.copie_vendute = 8000000,
			.anno_pubblicazione = 2020,
			.titolo = "hades",
			.editore = "supergiant games",
			.sviluppatore = "supergiant games",
			.descrizione = "roguelike frenetico con una storia mitologica e dialoghi brillanti.",
			.generi = {"roguelike", "azione"},
			.recensioni = {
				{.nome_utente = "zagreboy", .valutazione = 5},
				{.nome_utente = "underworldrunner", .valutazione = 4, .descrizione = "ripetitivo ma sempre divertente."},
				{.nome_utente = "casualplayer", .valutazione = 3}
			}
		},
		{
			.copie_vendute = 20000000,
			.anno_pubblicazione = 2013,
			.titolo = "the last of us",
			.editore = "sony",
			.sviluppatore = "naughty dog",
			.descrizione = "dramma post-apocalittico con narrativa intensa e stealth gameplay.",
			.generi = {"azione", "avventura"},
			.recensioni = {
				{.nome_utente = "joellover", .valutazione = 5, .descrizione = "la storia più emozionante di sempre."},
				{.nome_utente = "stealthfan", .valutazione = 5},
				{.nome_utente = "zombiefreak", .valutazione = 4}
			}
		},
		{
			.copie_vendute = 14000000,
			.anno_pubblicazione = 2016,
			.titolo = "doom",
			.editore = "bethesda",
			.sviluppatore = "id software",
			.descrizione = "sparatutto frenetico contro orde di demoni infernali.",
			.generi = {"sparatutto", "azione"},
			.recensioni = {
				{.nome_utente = "doomslayer", .valutazione = 5},
				{.nome_utente = "metalhead", .valutazione = 4, .descrizione = "colonna sonora pazzesca."},
				{.nome_utente = "nostalgicgamer", .valutazione = 5}
			}
		},
		{
			.copie_vendute = 10000000,
			.anno_pubblicazione = 2015,
			.titolo = "rocket league",
			.editore = "psyonix",
			.sviluppatore = "psyonix",
			.descrizione = "partite di calcio con auto potenziate.",
			.generi = {"sport", "multiplayer"},
			.recensioni = {
				{.nome_utente = "aerialking", .valutazione = 5},
				{.nome_utente = "boostmaster", .valutazione = 4, .descrizione = "competitivo e spettacolare."},
				{.nome_utente = "newbiecar", .valutazione = 3}
			}
		},
		{
			.copie_vendute = 15000000,
			.anno_pubblicazione = 2015,
			.titolo = "undertale",
			.editore = "toby fox",
			.sviluppatore = "toby fox",
			.descrizione = "rpg indie con scelte morali e personaggi memorabili.",
			.generi = {"rpg", "indie"},
			.recensioni = {
				{.nome_utente = "pacifistplayer", .valutazione = 5},
				{
					.nome_utente = "genocider", .valutazione = 4,
					.descrizione = "una storia che cambia completamente in base a te."
				},
				{.nome_utente = "retrolover", .valutazione = 4}
			}
		},
		{
			.copie_vendute = 6000000,
			.anno_pubblicazione = 2020,
			.titolo = "ghost of tsushima",
			.editore = "sony",
			.sviluppatore = "sucker punch",
			.descrizione = "open world ambientato nel giappone feudale con combattimenti katana.",
			.generi = {"azione", "avventura"},
			.recensioni = {
				{.nome_utente = "samuraipath", .valutazione = 5},
				{.nome_utente = "stealthmaster", .valutazione = 5, .descrizione = "una poesia visiva e sonora."},
				{.nome_utente = "historynerd", .valutazione = 4}
			}
		}
	};

	for (int i = 0; i < 20; i++) {
		aggiungiGioco(giochi[i]);
	}
}
