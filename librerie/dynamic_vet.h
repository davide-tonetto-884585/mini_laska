/** @file dynamic_vet.h
 *  @brief Prototipi delle funzioni per la gestione dei vettori dinamici di Mossa
 *
 *  Contiene i protoptipi delle funzioni che gestiscono
 *  il vettori dinamici di Mossa
 */


#ifndef MINI_LASKA_DYNAMIC_VET_H
#define MINI_LASKA_DYNAMIC_VET_H

typedef int bool; /**< typedef che definisce l'alias "bool" per gli int 0 e 1 */

/**
 * @brief struttura che definisce una posizione sulla scacchiera
 *
 * struttura che mappa una posizione sulla scacchiera indicandone la riga (int) e la colonna (char)
 */
typedef struct Posizione {
    int riga;
    char colonna;
} Posizione;

/**
 * @brief struttura che definisce le coordinate di una mossa
 *
 * struttura che definisce due posizioni per mappare una mossa dei giocatori
 * (posizione della pedina e posizione in cui si vuole spostare la data pedina)
 */
typedef struct Mossa {
    Posizione posizionePedina;
    Posizione posizioneFinale;
} Mossa;

/**
 * @brief Struct per gestione array dinamici di Massa
 * */
typedef struct VettoreDinamicoMosse {
    Mossa *mosse; /**< data */
    size_t size; /**< dimensione logica, celle effettivamente utilizzate */
    size_t capacity;/**< dimensione fisica, la massima dimensione utilizzabile */
} VettoreDinamicoMosse;

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
bool initVet(VettoreDinamicoMosse *vet, size_t capacity);

/**
 * @brief funzione che inserisce un elemento in coda all'array dinamico
 *
 * @param vet - vettore dinamico a cui inserire la Mossa
 * @param mossa - Mossa da inserire
 *
 * @return bool - 1 se è andato a buon fine, 0 altrimenti
 */
bool pushBack(VettoreDinamicoMosse *vet, Mossa mossa);

/**
 * @brief funzione che libera la memoria allocata per un vettore dinamico
 *
 * @param vet - vettore dinamico da deallocare
 *
 * @return void
 */
void freeVet(VettoreDinamicoMosse *vet);

#endif //MINI_LASKA_DYNAMIC_VET_H
