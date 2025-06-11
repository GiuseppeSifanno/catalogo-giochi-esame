#include "catalogolib.h"
#include "Unity-2.6.1/src/unity.h"
#include "Unity-2.6.1/src/unity_internals.h"

void setUp(void) {}

void tearDown(void) {}

void test_calcolaStatistiche(void) {
	gioco_t gioco = {
		.recensioni = {
			{
				.nome_utente = "Giuseppe",
				.valutazione = 0
			},
			{
				.nome_utente = "Giuseppe",
				.valutazione = 5
			},
			{
				.nome_utente = "Giuseppe",
				.valutazione = 4
			},
			{
				.nome_utente = "Giuseppe",
				.valutazione = 3
			},
			{
				.nome_utente = "Giuseppe",
				.valutazione = 2
			},
			{
				.nome_utente = "Giuseppe",
				.valutazione = 5
			},
			{
				.nome_utente = "Giuseppe",
				.valutazione = 4
			},
			{
				.nome_utente = "Giuseppe",
				.valutazione = 1
			}
		},
	};

	TEST_ASSERT_EQUAL_FLOAT(3, calcolaStatistiche(&gioco));
}

void test_aggiungiGioco(void) {
	gioco_t gioco ={
		.copie_vendute = 350000000,
		.anno_pubblicazione = 2011,
		.titolo = "test_titolo",
		.editore = "test_editore",
		.sviluppatore = "test_sviluppatore",
		.descrizione = "test_descrizione",
		.generi = {"test_genere1", "test_genere2"},
		.recensioni = {0}
	};
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, aggiungiGioco(gioco), "SE 1 E' GIA' PRESENTE");
}

void test_analisiQuery(void) {
	char query[MAX_CHAR] = "test_titolo";
	unsigned short num_elementi = 0;
	char **parametri = analisiQuery(query, &num_elementi);
	TEST_ASSERT_EQUAL_INT(1, num_elementi);
	TEST_ASSERT_EQUAL_STRING("test_titolo", parametri[0]);
	free(parametri);
}

void test_ricercaGlobale(void) {
	char query[MAX_CHAR] = "test_titolo";
	unsigned short num_elementi = 0;

	TEST_ASSERT_NOT_NULL(ricercaGlobale(query, &num_elementi));
}

void test_modificaGioco(void) {
	//gioco con numero di copie modificate
	gioco_t gioco = {
		.copie_vendute = 0,
		.anno_pubblicazione = 2011,
		.titolo = "test_titolo",
		.editore = "test_editore",
		.sviluppatore = "test_sviluppatore",
		.descrizione = "test_descrizione",
		.generi = {"test_genere1"},
		.recensioni = {0}
	};

	char query[MAX_CHAR] = "test_titolo";
	unsigned short num_elementi = 0;
	long *offset = ricercaGlobale(query, &num_elementi);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, modificaGioco(offset[0], &gioco), "SE 0 IL GIOCO POTREBBE NON ESSERE PRESENTE");
}

void test_inserisciRecensione(void) {
	char query[MAX_CHAR] = "test_titolo";
	unsigned short num_elementi = 0;
	long *offset = ricercaGlobale(query, &num_elementi);

	recensioni_t recensione = {
		.nome_utente = "test_utente",
		.valutazione = 5,
		.descrizione = "test_descrizione"
	};

	TEST_ASSERT_EQUAL_INT_MESSAGE(1, inserisciRecensione(&recensione, &offset[0]), "SE 0 POTREBBE NON ESSERE PRESENTE IL GIOCO");
}

void test_acquistaGioco(void) {
	char query[MAX_CHAR] = "test_titolo";
	unsigned short num_elementi = 0;
	long *offset = ricercaGlobale(query, &num_elementi);

	TEST_ASSERT_EQUAL_INT(1, acquistaGioco(offset[0]));
}

void test_eliminaGioco(void) {
	char query[MAX_CHAR] = "test_titolo";
	unsigned short num_elementi = 0;
	long *offset = ricercaGlobale(query, &num_elementi);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cancellaGioco(offset[0]), "SE 0 POTREBBE NON ESSERE PRESENTE IL GIOCO");
}

int main(void) {
	UNITY_BEGIN();

	RUN_TEST(test_calcolaStatistiche);
	RUN_TEST(test_aggiungiGioco);
	RUN_TEST(test_analisiQuery);
	RUN_TEST(test_ricercaGlobale);
	RUN_TEST(test_modificaGioco);
	RUN_TEST(test_inserisciRecensione);
	RUN_TEST(test_acquistaGioco);
	//eseguire per ultimo così da eliminare il gioco usato come test
	RUN_TEST(test_eliminaGioco);

	return UNITY_END();
}
