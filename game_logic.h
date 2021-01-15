/** @file game_logic.h
 *  @brief Prototipi delle funzioni per la gestione del gioco
 *
 *  Contiene i protoptipi delle funzioni che gestiscono
 *  il gioco mini laska
 */
#ifndef MINI_LASKA_GAME_LOGIC_H

#include "librerie/generic_dynamic_vet/dynarr.h"

#define LATO_SCACCHIERA (7) /**< Costante che definisce la lunghezza di ciascun lato della scacchiera */

#define MAX_DIM_NAME_PLAYER (20) /**< massimo numero di caratteri per il nome dei giocatori */
#define MAX_MOSSE_ANNULLABILI (3) /**< massimo di mosse annullabili a partita */
#define MAX_AIUTI_DISPONIBILI (7) /**< massimo di aiuti disponibili in modalià player vs player */
#define MAX_AIUTI_DISP_WALLE (11) /**< massimo di aiuti disponibili in modalità player vs cpu contro wall-e */
#define MAX_AIUTI_DISP_VIKI (9) /**< massimo di aiuti disponibili in modalità player vs cpu contro viki */
#define MAX_AIUTI_DISP_HAL (5) /**< massimo di aiuti disponibili in modalità player vs cpu contro hal-9000 */

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
 * @brief struttura che definisce una posizione sulla scacchiera
 *
 * struttura che mappa una posizione sulla scacchiera indicandone la riga (int) e la cella (char)
 */
struct posizione {
    int riga;
    char colonna;
};
typedef struct posizione posizione_t;

/**
 * @brief struttura che definisce le coordinate di una mossa
 *
 * struttura che definisce due posizioni per mappare una mossa dei giocatori
 * (posizione della pedina e posizione in cui si vuole spostare la data pedina)
 */
struct mossa {
    posizione_t posizionePedina;
    posizione_t posizioneFinale;
};
typedef struct mossa mossa_t;

DYN_ARR_CREATE(mossa_t); /**< definisco globalmente un vettore dinamico (costituito da una struct) di elementi di tipo struct mossa che poi gestirò tramite la libreria apposita */

/**
 * @brief struct utilizzato per salvare una mossa effettuata in dettaglio
 *
 * codesto struct permette di salvare oltre alla mossa effettuata le eventuali pedine eliminate durante una conquista
 * cosi da non perdere traccia di quest'ultime. Ciò ci è necessario per annullare una mossa.
 */
struct mossa_dettagliata {
    mossa_t mossa;
    pedina_t *pedinaEliminata; /**< puntatore che tiene traccia di una pedina eventualmente eliminata durante la mossa */
    bool_t hasBeenPromoted; /**< variabile booleana che, se a TRUE, indica che una pedina è stata promossa durante la mossa */
    bool_t hasConquered; /**< variabile booleana che indica se la mossa è di conquista o meno */
};
typedef struct mossa_dettagliata mossa_dettagliata_t;

DYN_ARR_CREATE(mossa_dettagliata_t); /**< definisco globalmente un vettore dinamico (costituito da una struct) di elementi di tipo struct mossa_dettagliata che poi gestirò tramite la libreria apposita */

/**
 * @brief struttura che definisce un giocatore
 *
 * struttura che definisce nome utente del giocatore e corrispondente colore delle pedine
 * inoltre è prsente un contatore per le mosse annullate, in modo da fissarne un limite
 */
struct player {
    enum colore colore;
    char *nome;
    unsigned int totMosseAnnullate;
    unsigned int totAiutiUsati;
};
typedef struct player player_t;

/**
 * @brief struttura che definisce lo stato della partita
 *
 * scopo della seguente struct è quello di rappresentare lo stato di una partita a mini laska, quindi memorizzandone
 * la scacchiera (matrice di cella), il colore del turno corrente, se la partita è conclusa o meno e un vettore
 * contenente lo storico delle mosse.
 */
struct partita {
    player_t player1;
    player_t player2;
    enum colore turnoCorrente;
    cella_t scacchiera[LATO_SCACCHIERA * LATO_SCACCHIERA]; /**< matrice flattened di struct cella che rappresenta la scacchiera */
    bool_t isEnded; /**< variabile booleana che definisce se la partita è conclusa (true) o non è conclusa (false) */
    dyn_arr_mossa_dettagliata_t mosseDettagliatePartita;
};
typedef struct partita partita_t;

/**
 * @brief funzione di inizializzazione della partita
 *
 * funzione che passata la partita ne inizializza gli elementi a inizio gioco impostando il turno iniziale a BIANCO,
 * isEnded a FALSE e inoltre la funzione allocherà le pedine della scacchiera e l'array delle mosse tramite malloc,
 * perciò bisognerà liberare la memoria con la funzione freePartita a fine partita.
 *
 * @param *partita - struct partita
 * @param ROWS - numero di righe della scacchiera
 * @param COLS - numero di colonne della scacchiera
 *
 * @return 1 se l'inizializzazione è andata a buon fine, 0 altrimenti
 */
bool_t init_game(partita_t *partita, size_t ROWS, size_t COLS);

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
bool_t controlloMossa(const cella_t *scacchiera, size_t ROWS, size_t COLS, mossa_t mossa, enum colore turnoCorrente);

/**
 * @brief funzione che restituisce tutte le mosse possibili che può effettuare un giocatore
 *
 * funzione che restituisce tutte le mosse disponibili che può effettuare un determinato giocatore dando priorità
 * a quelle di conquista secondo le regole del mini laska, utilizzare freeVet per deallocare il vettore restituito
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param ROWS - numero di righe della scacchiera
 * @param COLS - numero di colonne della scacchiera
 * @param colore - colore del giocatore di cui si vogliono sapere le mosse disponibili
 *
 * @return vettore_dinamico_mossa - vettore dinamico di struct mossa (andrà deallocato)
 */
dyn_arr_mossa_t trovaMosseDisponibili(const cella_t *scacchiera, size_t ROWS, size_t COLS, enum colore colore);

/**
 * funzione che muove la pedina indicata dalle coordinate iniziali alle coordinate finali, conquista se
 * la mossa lo richiede ed effettua la promozione in caso di necessità, inoltre agigorna il turno corrente e aggiunge
 * il resoconto della mossa (mossa_dettagliata) al vettore dinamico sito in partita.
 * La funzione non effettua controlli sulla validità della mossa, per fare ciò utilizzare la funzione controlloMossa
 *
 * @param *partita - puntatore alla partita
 * @param COLS - numero di colonne della scacchiera
 * @param mossa - struttura mossa che contiene la posizione iniziale e finale della mossa da effettuare
 *
 * @return mossa_dettagliata - resoconto della mossa
 */
mossa_dettagliata_t muoviPedina(partita_t *partita, size_t COLS, mossa_t mossa);

/**
 * funzione che passata la partita permette di annullare l'ultima mossa effettuata
 *
 * @param partita - puntatore alla partita di cui si vuole annullare l'ultima mossa
 * @param COLS - numero colonne della scacchiera
 *
 * @return bool_t - FALSE se durante la partita non si sono ancora effettuate mosse
 */
bool_t annullaUltimaMossa(partita_t *partita, size_t COLS);

/**
 * funzione che implementa l'algoritmo minimax per il calcolo della mossa migliore per il computer
 *
 * @param partita - puntatore alla partita corrente
 * @param ROWS - numero righe scacchiera
 * @param COLS - numero colonne scacchiera
 * @param maxDepth - profondità massima di calcolo da parte dell'algoritmo
 * @param currentDepth - profondità corrente (passare sempre 0)
 * @param maxPlayer - giocatore da massimizzare
 * @param turno - turno corrente nella partita
 * @param mossaMigliore - puntatore a mossa_t dove si salverà la miglio mossa calcolata
 * @param evaluateBoard - puntatore a funzione che valuta la scacchiera e restituisce un intero (negativo see la scacchiera è a favore del nero,
 * positivo altrimenti), parametri della funzione sonovla matrice flattened rappresentante la scacchiera, le righe e le colonne
 *
 * @return int
 */
int _minimax(partita_t *partita, size_t ROWS, size_t COLS, int maxDepth, int currentDepth, enum colore maxPlayer,
             enum colore turno, mossa_t *mossaMigliore, int (*evaluateBoard)(const cella_t *, size_t, size_t));

/**
 * stub della funzione _minimax che implementa la strategia della cpu tramite l'algoritmo di minimax
 *
 * @param partita - puntatore alla partita corrente
 * @param maxDepth - profondità massima di calcolo da parte dell'algoritmo
 * @param maxPlayer - giocatore da massimizzare
 * @param turno - turno corrente nella partita
 * @param mossaMigliore - puntatore a mossa_t dove si salverà la miglio mossa calcolata
 * @param evaluateBoard - puntatore a funzione che valuta la scacchiera e restituisce un intero, parametri della funzione sono
 * la matrice flattened rappresentante la scacchiera, le righe e le colonne
 *
 * @return int
 */
int minimax(partita_t *partita, int maxDepth, enum colore maxPlayer, enum colore turno,
            mossa_t *mossaMigliore, int (*evaluateBoard)(const cella_t *, size_t, size_t));

/**
 * funzione usata dall'algoritmo di minimax per valutare la scacchiera (startegia di gioco della cpu)
 *
 * @param scacchiera
 * @param ROWS
 * @param COLS
 *
 * @return int - valutazione della scacchiera (positiva se bianco vince, negativa se nero vince)
 */
int evaluateBoard(const cella_t *scacchiera, size_t ROWS, size_t COLS);

/**
 * @brief funzione che si occupa di liberare la memoria allocata tramite malloc per le pedine della
 * scacchiera e per il vettore delle mosse
 *
 * funzione che passata la scacchiera della partita libera tramite free() le pedine e il vettore dinamico delle mosse
 *
 * @param partita - variabile partita contenente le informazioni della partita e i puntatori alle memorie da deallocare
 * @param COLS - numero di colonne della scacchiera
 * @param ROWS - numero di righe della scacchiera
 *
 * @return void
 */
void freePartita(partita_t partita, size_t ROWS, size_t COLS);

#define MINI_LASKA_GAME_LOGIC_H

#endif /*MINI_LASKA_GAME_LOGIC_H*/
