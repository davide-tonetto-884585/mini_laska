/** @file mini_laska.h
 *  @brief Prototipi delle funzioni per la gestione del gioco
 *
 *  Contiene i protoptipi delle funzioni che gestiscono
 *  il gioco mini laska
 */

#include "librerie/dynamic_vet.h"

#define LATO_SCACCHIERA (7) /**< Costante che definisce la lunghezza di ciascun lato della scacchiera */

/**
 * @brief enum che definisce di che colore possono essere le pedine
 *
 * Tramite questo enum si definiscono i due colori delle pedine presenti nella scacchiera
 * ai quali viene assegnato un valore char corrispondente alla lettera che li rappresenta nella scacchiera
 * sul terminale
 */
enum colore {
    BIANCO = 'b',
    NERO = 'n'
};

/**
 * @brief struttura che definisce una pedina indicandone il colore e se è stata promossa o meno
 *
 * scopo della seguente struct è rappresentare e gestire le informazioni di una pedina le cui caratteristiche sono
 * definite dal colore e dal suo stato (promossa o non promossa)
 */
struct pedina {
    enum colore colore;
    bool_t isPromossa;
};
typedef struct pedina pedina_t;

/**
 * @brief struttura che definisce una cella costituita da massimo tre pedine
 *
 * scopo della seguente struct è quello di rappresentare ciò che può essere presente in ciascuna
 * delle celle della scacchiera quindi nessuna pedina in caso la cella sia vuota oppure
 * una o più pedine (con un massimo di tre)
 */
struct cella {
    pedina_t *pedine[3]; /**< array di puntatori a pedina che rappresenta le pedine presenti nella data cella */
    int altezza; /**< intero che rappresenta l'attuale altezza della cella di pedine (da 0 a 3) */
};
typedef struct cella cella_t;

/**
 * @brief struttura che definisce lo stato della partita
 *
 * scopo della seguente struct è quello di rappresentare lo stato di una partita a mini laska, quindi memorizzandone
 * la scacchiera (matrice di cella), il colore del turno corrente, se la partita è conclusa o meno e un vettore
 * contenente lo storico delle mosse.
 */
struct partita {
    enum colore turnoCorrente;
    cella_t scacchiera[LATO_SCACCHIERA][LATO_SCACCHIERA]; /**< matrice di struct cella che rappresenta la scacchiera */
    bool_t isEnded; /**< variabile booleana che definisce se la partita è conclusa (true) o non è conclusa (false) */
    vettore_dinamico_mossa_t mossePartita;
};
typedef struct partita partita_t;

/**
 * @brief funzione di inizializzazione della partita
 *
 * funzione che passata la matrice rappresentante la scacchiera ne inizializza gli elementi a inizio gioco,
 * la funzione allocherà le pedine della scacchiera tramite malloc, perciò bisognerà liberare la memoria con
 * la funzione freePedine a fine partita
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param ROWS - numero di righe della scacchiera
 * @param COLS - numero di colonne della scacchiera
 *
 * @return 1 se l'inizializzazione è andata a buon fine, 0 altrimenti
 */
bool_t init_game(cella_t *scacchiera, size_t ROWS, size_t COLS);

/**
 * @brief funzione che stampa la scacchiera in base alla matrice delle pedine passata per argomento
 *
 * funzione che passata la matrice rappresentante la scacchiera la stampa a terminale
 * (rappresenta le pedine bianche con b (B se promosse) e nere con n (N se promosse)
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param lato - lunghezza di ciascun lato della scacchiera
 *
 * @return void
 */
void draw(cella_t *scacchiera, size_t lato);

/**
 * @brief funzione che controlla se ci sono pedine da promuovere e, in caso, le promuove
 *
 * funzione che passata la matrice controlla se ci sono pedine a fondo scacchiera del colore opposto a quello di
 * partenza e, in caso, le promuove cambiando lo stato di pedina.isPromossa a true
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param lato - lunghezza di ciascun lato della scacchiera
 *
 * @return void
 */
void controlloPromozione(cella_t *scacchiera, size_t ROWS, size_t COLS);

/**
 * @brief funzione che controlla se la mossa passata è valida secondo le regole del gioco
 *
 * funzione che passata una determinata mossa ne controlla l'effettiva validità in base alle regole del gioco,
 * la funzione non effettua lo spostamento ma ne verifica solo la validità, inoltre non prevede la priorità
 * per le mosse di conquista (per fare ciò usare la funzione trovaMosseDisponibili
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param ROWS - numero di righe della scacchiera
 * @param COLS - numero di colonne della scacchiera
 * @param mossa - struttura mossa che contiene la posizione iniziale e finale della mossa da controllare
 * @param turnoCorrente - colore del giocatore che sta effettuando la mossa
 *
 * @return 1 se la mossa è valida, 0 altrimenti
 */
bool_t controlloMossa(cella_t *scacchiera, size_t ROWS, size_t COLS, mossa_t mossa, enum colore turnoCorrente);

/**
 * @brief funzione che restituisce tutte le mosse possibili che può effettuare un giocatore
 *
 * funzione che restituisce tutte le mosse disponibili che può effettuare un determinato giocatore dando priorità
 * a quelle di conquista secondo le regole del mini laska, utilizzare freeVet per deallocare il vettore restituito
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param ROWS - numero di righe della scacchiera
 * @param COLS - numero di colonne della scacchiera
 * @param turnoCorrente - colore del giocatore di cui si vogliono sapere le mosse disponibili
 *
 * @return vettore_dinamico_mossa - vettore dinamico di struct mossa (andrà deallocato)
 */
vettore_dinamico_mossa_t
trovaMosseDisponibili(cella_t *scacchiera, size_t ROWS, size_t COLS, enum colore turnoCorrente);

/**
 * @brief funzione che muove la pedina indicata dalle coordinate iniziali alle coordinate finali e conquista se
 * la mossa lo richiede la funzione non effettua controlli sulla validità della mossa, per fare ciò utilizzare
 * la funzione controlloMossa
 *
 * funzione che restituisce tutte le mosse disponibili che può effettuare un determinato giocatore dando priorità
 * a quelle di conquista secondo le regole del mini laska, utilizzare freeVet per deallocare il vettore restituito
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param COLS - numero di colonne della scacchiera
 * @param mossa - struttura mossa che contiene la posizione iniziale e finale della mossa da effettuare
 *
 * @return vettore_dinamico_mossa - vettore dinamico di struct mossa (andrà deallocato)
 */
void muoviPedina(cella_t *scacchiera, size_t COLS, mossa_t mossa);

/**
 * @brief funzione che passato il turno attuale lo cambia
 *
 * funzione che restituisce il colore opposto a quello passato e che cosi facendo permette
 * di cambiare turno durante la partita
 *
 * @param turnoCorrente - colore del turno attuale
 *
 * @return colore - colore opposto a quello passato
 */
enum colore switchTurno(enum colore turnoCorrente);

/**
 * @brief funzione che restitruisce la posizione dell'elemento di una matrice flattened
 *
 * funzione che passati riga e cella di un elemento di una matrice li converte alla posizione
 * corrispondente nella matrice flattened
 *
 * @param row - riga dell'elemento
 * @param col - cella dell'elemento
 * @param width - numero di colonne della matrice
 *
 * @return size_t - posizione corrispondente nella matrice flattened
 */
size_t atPosition(size_t row, size_t col, size_t width);

/**
 * @brief funzione che trova la posizione intermedia tra due celle (posizione della pedina da conquistare)
 *
 * funzione che passati riga e cella di un elemento di una matrice li converte alla posizione
 * corrispondente nella matrice flattened però restituendo quella intermedia nella quale si trova la pedina che
 * verrà conquistata.
 *
 * @param mossaConquista - mossa che definisc la cella iniziale e la cella finale
 * @param width - numero di colonne della matrice
 *
 * @return size_t - posizione corrispondente alla pedina da conquistare nella matrice flattened
 */
size_t atItermediatePosition(mossa_t mossaConquista, size_t width);

/**
 * @brief funzione che si occupa di liberare la memoria allocata tramite malloc per le pedine della
 * scacchiera che si trovano ancora in gioco a fine partita
 *
 * funzione che passata la scacchiera della partita libera tramite free() le pedine ancora rimaste in partita che
 * erano state allocate tramite la malloc
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param COLS - numero di colonne della scacchiera
 * @param ROWS - numero di righe della scacchiera
 *
 * @return void
 */
void freePedine(cella_t *scacchiera, size_t ROWS, size_t COLS);

/**
 * @brief funzione che shifta gli elementi di un array di puntatori a pedina
 *
 * funzione che passato un array di puntatori a pedina ne shifta gli elementi a partire da un elemento indicato
 *
 * @param **v - array di *pedina
 * @param p - posizione da liberare
 * @param n - dimensione logica dell'array
 * @param v_size - dimensione fisica dell'array
 *
 * @return bool - 1 se l'operazione è andtata a buon fine, 0 altrimenti
 */
bool_t ShiftA(pedina_t **v, int n, int p, int v_size);

/**
 * @brief funzione che stampa a terminale una mossa
 *
 * @param mossa - mossa da stampare
 *
 * @return void
 */
void stampaMossa(mossa_t mossa);

/**
 * @brief funzione che richiede un parametro in input all'utente e controlla che sia di tipo int valido
 *
 * @param var - puntatore alla variabile in cui salvare l'input
 *
 * @return bool - 1 se l'input è valido, 0 altrimenti
 */
bool_t inputInt(int *var);
