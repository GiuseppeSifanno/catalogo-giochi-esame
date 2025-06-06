# Gestione catalogo online di videogiochi

ll caso di studio deve simulare una parte delle funzionalità di una piattaforma online per la catalogazione di videogiochi.

## Informazioni generali

- **Nome file del catalogo** (`NOME_FILE`): `catalogo.dat`
- **Directory del catalogo**: area di lavoro
- **Dimensione massima stringhe** (`MAX_CHAR`): 255
- **Offset**: posizione in byte all'interno del file binario.
- **Blocco**: struttura fissa che rappresenta un gioco.
- **Parametro specifico**: identificato da un simbolo speciale (`#` o `$`).

## Definizione dei campi di un gioco

Il gioco è rappresentato da una *struttura* dati di tipo **gioco_t**, i campi di quest'ultima sono:

- titolo (*stringa*)
- editore (*stringa*)
- sviluppatore (*stringa*)
- breve descrizione testuale del gioco (*stringa*)
- anno di pubblicazione (*unsigned short int*)
- copie vendute (*unsigned long*)
- uno o più generi (*array di stringhe*)
- una o più recensioni (*array di strutture*) con:
  - nome utente (*stringa*)
  - valutazione (*uint8_t*, da 0 a 5)
  - descrizione (*stringa*, opzionale)

## Ruoli presenti nel progetto

All'interno del progetto ci sono due ruoli:

1.  [Amministratore](#amministratore)
2.  [Visitatore del catalogo](#visitatore)

> **Nota**: Quest'ultimi non hanno un'importanza molto significativa, infatti non sono presenti meccanismi di sicurezza per l'accesso ad ognuno dei due ruoli, è presente una semplice scelta all'avvio del programma e l'inserimento di un nome utente per l'utente.

> **Nota per le recensioni:** Per quest'ultime non è stato predisposto nessun meccanismo di gestione come eliminazione o modifica ecc...

### Amministratore

Funzionalità di un'amministratore:
1. [Aggiunta di un gioco](#funzione-di-aggiunta)
2. [Ricerca di un gioco](#funzione-di-ricerca)
\* [Modifica di un gioco](#funzione-di-modifica)
\* [Cancellazione di un gioco](#funzione-di-cancellazione)
\* [Visualizzazione delle recensioni](#funzione-di-visualizzazione-delle-recensioni)
\* [Visualizzazione delle statistiche](#visualizzare-le-statistiche)
3. [Ordinamento dei giochi](#funzione-di-ordinamento-giochi)

### Visitatore

Funzionalità disponibili:
1. [Ricerca di un gioco](#funzione-di-ricerca)
\* [Inserimento di una recensione](#funzione-di-inserimento-recensione)
\* [Visualizzazione delle recensioni](#funzione-di-visualizzazione-delle-recensioni)
\* [Acquisto di un gioco](#funzione-di-acquisto)
2. [Ordinamento dei giochi](#funzione-di-ordinamento-giochi)

# Funzionionalità del sistema

## Funzione di ricerca

La funzione di ricerca è il fulcro, non solo per poter ricercare i giochi ma anche per poter operare su di loro. Quasi tutte le funzioni si basano su quest'ultima per poter funzionare in maniera più efficiente e rapida.

Prima di poter operare su un gioco è necessario cercarlo, per farlo l'utente dovrà digitare i parametri che a lui interessa utilizzare durante la ricerca. Verrà dunque composta un query contenente i parametri necessari per la ricerca. In alcuni casi la ricerca di un gioco non viene fatta tramite query, detta anche *globalmente* su tutto il file alla ricerca di qualsiasi gioco che corrisponda ai parametri, ma con l'altro metodo di ricerca, quello *specifico* che si posiziona direttamente al gioco voluto.

### 1° metodo, globale

#### Com'è composta la query

La query, una semplice stringa di caratteri, può essere composta da uno o più parametri. I parametri (*titolo, editore, generi, ecc...*) **devono** essere separati tra loro da un delimitatore, una virgola " *,* ", i parametri *generici* (titolo, editore, sviluppatore) possono essere scritti liberamente, per i parametri *specifici* (anno di pubblicazione, genere) è necessario precedere il testo con il simbolo *\$* per l'anno di pubblicazione mentre *\#* per il genere (è possibile utilizzarli più volte per creare delle combinazioni).

Inserita la query, è necessaria analizzarla così da poterne ricavare i parametri che verranno poi utilizzati nella ricerca

| Simbolo |    Campo associato    |     Esempio     |
|:-------:|:---------------------:|:---------------:|
|   `$`   | Anno di pubblicazione |     `$2020`     |
|   `#`   | Genere del videogioco | `#horror, #rpg` |

#### Analisi della query

Ad occuparsi di questa operazione è la funzione *analisiQuery*, inizialmente viene predisposto un array di stringhe, creato e gestito in maniera dinamica, all'interno di quest'ultimo verranno inseriti tutti i **parametri**.

Prima di tokenizzare è necessario creare una copia della query sulla quale si opera. La query viene divisa in token, ognuno di questi viene dunque "pulito" eliminando spazi all'inizio e alla fine e rendendola completamente *lower case*. Terminato la pulizia della stringa, se avrà una lunghezza superiore a 0, dunque è presente almeno un carattere (escluso lo spazio), significa che abbiamo trovato un parametro valido. Prima di inserire il parametro all'interno dell'array bisogna gestire in maniera opportuna la [memoria](#altre-funzioni-utili). Se non ci sono problemi, il parametro potrà essere inserito all'interno dell'array.
Lo stesso procedimento verrà ripetuto per ogni token generato.

#### Operazione di ricerca

La ricerca viene ottimizzata in base alla tipologia dei parametri inseriti dall'utente.
1. Parametri **specifici**
\* Per il campo *anno di pubblicazione* la ricerca verrà effettuata solo su quest'ultimo se e solo se il valore da cercare è **preceduto** dal simbolo *\$* (esempio: *$2024*)
    * Per il campo *genere* la ricerca verrà effettuata solo su quest'ultimo se e solo se il valore da cercare è **preceduto** dal simbolo *#* (esempio: *#horror*, *#fps*). Il confronto con il parametro avviene per tutti i generi del gioco fin quando non c'è una corrispondenza. La comparazione da esito positivo anche se non c'è un match esatto con il genere
2. Parametri **generici**
    * Per i campi *titolo*, *editore*, *sviluppatore* non viene applicato nessun tipo di operazione aggiuntiva, il confronto anche in questo caso non necessita di **matching esatto**.
      Qualsiasi parametro contenente un qualsiasi valore apparterrà a questa categoria (esempio: 2024 può essere considerato come titolo, **non** verrà considerato come *anno di pubblicazione* poiché non è preceduto dal simbolo *$*, lo stesso medesimo comportamento si verifica anche per il genere).

I token dei parametri *specifici* sono definite come MACRO all'interno del programma, TOKEN_1 (\$) e TOKEN_2 (#)

Essendo un tipo di ricerca flessibile non viene indicata la tipologia dei parametri *generici*, per questa ragione il controllo è fatto con tutti i parametri generici. Ciò è dovuto anche da una possibile coincidenza di alcuni campi come editore e sviluppatore.

Se durante la ricerca **almeno** un campo ha avuto riscontro positivo allora il gioco può essere pertinente alla ricerca dell'utente.
Anche per questa operazione di ricerca si sfrutta un'array che viene ridimensionato in base all'evenienza. Ciò che verrà inserito all'interno di questo array non è altro che la posizione del blocco in cui è stato trovato il gioco. Dunque la sua posizione effettiva nel file binario, non è necessario utilizzare un contatore per tenere traccia del numero di blocchi letti nel file, infatti è possibile recuperare il dato tramite una funzione specifica *ftello()*, il valore restituito, detto anche **offset**, dalla funzione è ciò che andrà a riempire la cella dell'array. L'array di offset è ciò che verrà restituito al chiamante.

### 2° metodo, specifico

In alcuni casi non è necessario dover cercare in tutto il file il gioco che ci interessa dovendo fare controlli su controlli, è possibile cercarlo direttamente tramite il suo *offset*, ci si posiziona sul blocco corretto tramite una funzione, che accetta l'offset come parametro e si legge quest'ultimo. Una volta fatto, il gioco verrà poi restituito così da poterlo gestire direttamente con tutti i suoi dati.

## Funzione di aggiunta

**DISPONIBILE SOLO PER L'AMMINISTRATORE**

Inizialmente viene creata una nuova istanza di tipo *gioco* con i propri dati tramite la funzione *[acquisisciGioco](#altre-funzioni-utili)*, successivamente si controlla se quel gioco non è già presente all'interno del catalogo, la funzione che si occupa di questa operazione è chiamata *[isAlredyAdded](#altre-funzioni-utili)*.

Se il gioco inserito in input dall'utente è valido allora il gioco verrà inserito all'interno del file.
\## Funzione di modifica

**DISPONIBILE SOLO PER L'AMMINISTRATORE**

La modifica di un gioco può essere effettuata solo dopo aver [cercato](#funzione-di-ricerca) e selezionato il gioco originale da voler modificare. Selezionato il gioco si potrà procedere nella modifica delle informazioni di quest'ultimo.

La funzione sfrutterà **l'offeset** per potersi posizionare direttamente nel blocco del gioco da voler modificare, terminato il posizionamento si potrà procedere con la scrittura dei nuovi dati.
\## Funzione di cancellazione

**DISPONIBILE SOLO PER L'AMMINISTRATORE**

La funzione si occupa della cancellazione di un gioco all'interno del catalogo. La posizione del blocco, quindi del gioco da cancellare è rappresentata dall'**offset**, ottenuto durante la ricerca e selezione del gioco.

## Funzione di inserimento recensione

**DISPONIBILE SOLO PER IL VISITATORE**

La funzione si occupa dell'inserimento di una recensione all'interno del catalogo per un gioco specifico, è necessario cercare e selezionare il gioco desiderato, quindi sfruttare la [ricerca specifica](#2-metodo,-ricerca-specifica) per recuperare il gioco e salvarlo in una variabile, si andranno poi a scorrere tutte le celle contenenti le recensioni affinché non se ne trovi una libera (dove non c'è un nome utente). La recensione verrà copiata nella prima cella disponibile altrimenti non verrà inserita se non ci sono più celle libere.

**In questo caso non viene sfruttato un meccanismo dinamico per le recensioni poiché per i file binari non esiste un meccanismo automatico che "shifta" i singoli dati riorganizzando tutti i dati, inoltre rendere la dimensione dei blocchi diversa tra loro comporterebbe numerosi problemi durante la lettura del file da parte di altre funzioni. Ad esempio se per un gioco ci sono 10 recensioni, quest'ultimo avrà una sua dimensione all'interno del file, differente da un altro gioco che potrebbe averne 0,3,20, ecc...
Per questa ragione si è optato per un numero fisso di recensioni, la modifica di quest'ultimo dopo aver inserito dei dati all'interno del file binario comporterebbe sempre a problemi legati alla dimensione dei blocchi**

## Funzione di visualizzazione delle recensioni

**DISPONIBILE PER IL VISITATORE E L'AMMINISTRATORE**

La funzione ha il compito di restituire tutte le recensioni di un determinato gioco, per questa ragione bisogna prima cercare il gioco poi, una volta selezionato, sarà possibile utilizzare questa funzione, infatti quest'ultima richiede l'offset del gioco in maniera tale da poterlo cercare, leggere e inserirlo all'interno di una variabile così da poter avere tutti i dati a disposizione. Letto il gioco, si passa a recuperare tutte le recensioni valide, ovvero quelle che hanno un nome utente, infatti ogni recensione **deve** averne uno. Tutte le recensioni senza un nome utente verranno ignorate, quando viene trovata un recensione valida, quest'ultima viene inserita all'interno di un'array, gestito sempre in maniera dinamica, viene inoltre incrementato un contatore sul numero di recensioni che verrà sfruttato per scorrere l'array fino all'n-esima cella.

Solo in questo case non viene sfruttata la funzione *checkMemory* poiché può creare problemi con la gestione di un'array di struct.

## Visualizzare le statistiche

**DISPONIBILE PER IL VISITATORE E L'AMMINISTRATORE**

La visualizzazione delle statistiche può avvenire secondo due modalità:

1.  Per copie vendute
2.  Media valutazione

In questo caso non esiste una funzione che effettua quest'operazione ma attraverso l'istanza del gioco si mostrano le copie vendute (1 metodo), mentre per il secondo metodo si sfrutta una funzione per il calcolo della media della valutazione di un singolo gioco, per questa ragione la funzione denominata *calcolaStatistiche* accetta solo l'istanza del gioco.

## Funzione di ordinamento giochi

**DISPONIBILE PER IL VISITATORE E L'AMMINISTRATORE**

Per questa funzione esistono due metodi di ordinamento:
1. Per copie vendute
2. Per media di valutazione

In entrambi i casi l'algoritmo utilizzato è il **[shell sort](#altre-funzioni-utili)**

Innanzitutto bisogna assicurarsi che all'interno del catalogo ci siano dei giochi, altrimenti l'ordinamento non avrebbe senso, fatto ciò si passa alla lettura di tutti i giochi i quali verranno inseriti all'interno di un'array che verrà poi ordinato, l'ordinamento non viene eseguito se il numero di giochi nel catalogo **è uguale** a 1.

Il criterio di ordinamento è deciso dalla modalità selezionata dall'utente, dunque per il 1° metodo verranno utilizzati direttamente i dati del gioco contenuto nella cella che si sta valutando, mentre per il 2° è necessario che vengano analizzate tutte le recensioni per poter ottenere la media delle valutazioni, dunque si sfrutta la funzione che si occupa di ciò (*calcolaStatistiche*), ciò che restituirà verrà poi utilizzato per il confronto.

Terminato l'ordinamento, l'array ordinato sarà consultabile.

## Funzione di acquisto

**DISPONIBILE SOLO PER IL VISITATORE**

In base al gioco selezionato, viene recuperato tramite la *[ricerca specifica](#2-metodo-ricerca-specifica)* e poi aggiornato il contatore delle copie vendute di 1. Il gioco verrà modificato nel catalogo tramite la funzione [modifica](#funzione-di-modifica).

# Altre funzioni utili

Oltre alla funzioni che interagiscono e riguardano esclusivamente il catalogo e i giochi al suo interno, ci sono anche altre funzioni o dati necessari per il funzionamento del sistema

Quest'ultimi sono contenuti all'interno del file *utility.c/.h*. Le funzioni sono:

- *trim*, pulizia delle stringhe

- *acquisisciGioco*, acquisisce i dati di un gioco e nel mentre esegue i controlli sui vincoli d'integrità di ogni dato.

- *apriCatalogo*, funzione per agevolare il meccanismo di apertura del file

- *analisiQuery*, parsing delle query

- *checkMemory*, funzione necessaria per la gestione e l'allocazione (o reallocazione) di una struttura dati, alla base di questa funzione ci sono i puntatori con i quali è possibile tenere traccia delle aree di memoria che vengono modificate in base all'evenienza, tramite questa funzione è possible svincolarsi dal creare array di dati con dimensione fissa che potrebbero occupare molta più memoria di quella che effettivamente necessitano. Terminate le sue operazioni, il vettore, se necessario, risulterà con una capacità maggiore.

- *shellSort*, funzione per l'ordinamento dei giochi, **l'ordinamente non avviene anche sul file, solo in un'array**. L' algoritmo che garantisce un tempo di esecuzione non troppo elevato (*n log n*) rispetto ad altri algoritmi di ordinamento. La tipologia di ordinamento è **decrescente**

- *isAlredyAdded*, viene chiamata prima della funzione di aggiunta così da prevenire inserimenti inutili all'interno del catalogo. Il tipo di controllo che viene applicato in questo caso, sui campi, è a *matching esatto* dunque **tutti** i campi esclusi quelli inerenti alle *recensioni*. Se anche un singolo carattere è diverso allora il gioco verrà considerato come nuovo dunque potrà essere aggiunto all'interno del catalogo.
