/** @file dynamic_vet.h
 *  @brief Prototipi delle funzioni per la gestione dei vettori dinamici di mossa
 *
 *  Contiene i protoptipi delle funzioni che gestiscono
 *  il vettori dinamici di mossa
 */


#ifndef MINI_LASKA_DYNAMIC_VET_H
#define MINI_LASKA_DYNAMIC_VET_H

/**
 * @brief enum che implementa il tipo bool
 *
 * tramite questo enum si vuole implementare il tipo bool che assegna 0 a FALSE e 1 a TRUE
 */
enum bool {FALSE, TRUE};
typedef enum bool bool_t;

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

/**
 * @brief Struct per gestione array dinamici di Massa
 * */
struct vettore_dinamico_mossa {
    mossa_t *mosse; /**< data */
    size_t size; /**< dimensione logica, celle effettivamente utilizzate */
    size_t capacity;/**< dimensione fisica, la massima dimensione utilizzabile */
};
typedef struct vettore_dinamico_mossa vettore_dinamico_mossa_t;

/**
 * @brief funzione inizializzazione vettore dinamico
 *
 * funzione che dato un vettore dinamico lo inizializza allocandone memoria in base alla capacity
 * passata
 *
 * @param vet - vettore dinamico da inizializzare
 * @param capacity - capacità iniziale del vettore dinamico
 *
 * @return bool - 1 se è andato a buon fine, 0 altrimenti
 */
bool_t initVet(vettore_dinamico_mossa_t *vet, size_t capacity);

/**
 * @brief funzione che inserisce un elemento in coda all'array dinamico
 *
 * @param vet - vettore dinamico a cui inserire la mossa
 * @param mossa - mossa da inserire
 *
 * @return bool - 1 se è andato a buon fine, 0 altrimenti
 */
bool_t pushBack(vettore_dinamico_mossa_t *vet, mossa_t mossa);

/**
 * @brief funzione che libera la memoria allocata per un vettore dinamico
 *
 * @param vet - vettore dinamico da deallocare
 *
 * @return void
 */
void freeVet(vettore_dinamico_mossa_t *vet);

#endif /*MINI_LASKA_DYNAMIC_VET_H*/
