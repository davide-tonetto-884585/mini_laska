/** @file utility.h
 *  @brief Prototipi delle funzioni di utility utilizzate nel programma
 */
#ifndef MINI_LASKA_UTILITY_H

#include "../game_logic.h"

#define INPUT_SIZE (20) /**< costante che definisce la grandezza del buffer di input */

/**
 * funzione che ritorna il minimo tra i due valori
 *
 * @param a
 * @param b
 * @return
 */
int min(int a, int b);

/**
 * funzione che ritorna il massimo tra i due valori
 *
 * @param a
 * @param b
 * @return
 */
int max(int a, int b);

/**
 * @brief funzione che passato il turno attuale lo cambia
 *
 * funzione che cambia il colore passato e che cosi facendo permette
 * di cambiare turno durante la partita
 *
 * @param turnoCorrente - colore del turno attuale
 *
 * @return void
 */
void switchTurno(enum colore *turnoCorrente);

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
 * corrispondente nella matrice flattened per? restituendo quella intermedia nella quale si trova la pedina che
 * verr? conquistata.
 *
 * @param mossaConquista - mossa che definisc la cella iniziale e la cella finale
 * @param width - numero di colonne della matrice
 *
 * @return size_t - posizione corrispondente alla pedina da conquistare nella matrice flattened
 */
size_t atItermediatePosition(mossa_t mossaConquista, size_t width);

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
 * @return bool - 1 se l'operazione ? andtata a buon fine, 0 altrimenti
 */
bool_t ShiftADX(pedina_t **v, int n, int p, int v_size);

/**
 * @brief funzione che stampa a terminale una mossa
 *
 * @param mossa - mossa da stampare
 *
 * @return void
 */
void stampaMossa(mossa_t mossa);

/**
 * funzione che verifica se due mosse sono uguali
 *
 * @param m1
 * @param m2
 *
 * @return bool
 */
bool_t equalsMossa(mossa_t m1, mossa_t m2);

/**
 * @brief funzione che richiede un parametro in input all'utente e controlla che sia di tipo int valido
 *
 * @param var - puntatore alla variabile in cui salvare l'input
 *
 * @return bool - 1 se l'input ? valido, 0 altrimenti
 */
bool_t inputInt(int *var);

/**
 * @brief funzione che si occupa di creare un numero random in un range di valori passati
 *
 * @param max - numero massimo del range di valori
 * @param min - numero minimo del range di valori
 *
 * @return unsigned int - numero random estratto dal range di valori compreso tra max e min
 */
unsigned int numeriRandom(unsigned int min, unsigned int max);

#define MINI_LASKA_UTILITY_H

#endif /*MINI_LASKA_UTILITY_H*/
