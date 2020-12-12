/** @file graphics.h
 *  @brief Prototipi delle funzioni per la gestione dela grafica del gioco
 *
 *  Contiene i protoptipi delle funzioni che gestiscono
 *  la visualizzazione a terminale del gioco
 */
#ifndef MINI_LASKA_GRAPHICS_H
#include "../game_logic.h"

/**
 * @brief funzione che stampa la scacchiera in base alla matrice delle pedine passata per argomento funziona in
 * windows e da una grafica pi? accattivante
 *
 * funzione che passata la matrice rappresentante la scacchiera la stampa a terminale e inoltre, se passata una mossa la
 * indica per facilitarne l'individuzione
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param lato - lunghezza di ciascun lato della scacchiera
 * @param mossa - puntatore a una eventuale ossa che verr? evidenziata per facilitare la lettura
 *
 * @return void
 */
void draw(const cella_t *scacchiera, size_t lato, const mossa_t *mossa);

/**
 * @brief funzione che stampa la scacchiera in base alla matrice delle pedine passata per argomento (va bene per qualsiasi sistema operativo)
 *
 * funzione che passata la matrice rappresentante la scacchiera la stampa a terminale
 * (rappresenta le pedine bianche con b (B se promosse) e nere con n (N se promosse) e inoltre, se passata una mossa la
 * indica per facilitarne l'individuzione
 *
 * @param *scacchiera - matrice flattened contenente lo stato attuale della scacchiera della partita
 * @param lato - lunghezza di ciascun lato della scacchiera
 * @param mossa - puntatore a una eventuale ossa che verr? evidenziata per facilitare la lettura
 *
 * @return void
 */
void multiPlatformDraw(const cella_t *scacchiera, size_t lato, const mossa_t *mossa);

#if defined(_WIN32) || defined(_WIN64)
/* funzione che gestisce la selezione dell'opzione */
void arrow_pos(int real_pos, int arrow_pos);

/* menu interattivo che setta la modalità di gioco */
void menu_mod(bool_t *modVsCPU);
#endif

/* stampa il titolo "mini lasca" */
void titolo();

#define MINI_LASKA_GRAPHICS_H

#endif /*MINI_LASKA_GRAPHICS_H*/
