/** @file main.c
 *  @brief main file
 *
 *  Implementa il gioco mini laska attraverso le funzioni definite su mini_laska.h
 */
#include <stdio.h>
#include "mini_laska.h"

/**
 * main del programma dove viene gestito il gioco tramite l'utilizzo di strutture e funzioni (vedere file mini_laska.h)
 * */
int main() {
    /*variabile per ciclo for*/
    int i;

    /*variabile in cui salvo la mossa che l'utente vuole effettuare e variabile in cui salvo la modalità che l'utente desidera giocare */
    int numeroMossa, numeroModalita;

    /* variabile che indica la modalità di gioco scelta: true contro computer, false contro altro giocatore locale */
    bool_t modVsCPU;

    /*variabile che indica se la mossa inserita è valida o meno*/
    bool_t isInputValido;

    /* variabile contenete l'index della miglior mossa calcolata per la cpu*/
    mossa_t mossaMigliore;

    /*vettore dinamico mosse*/
    dyn_arr_mossa_t mosseDisponibili;

    /*creo la variabile partita*/
    partita_t partita;

    /*inizializzo la partita e controllo che tutto sia andato a buon fine*/
    if (!init_game(&partita, LATO_SCACCHIERA, LATO_SCACCHIERA)) {
        freePartita(partita, LATO_SCACCHIERA, LATO_SCACCHIERA);
        printf("Errore inizializzazione partita.");
        return 1;
    }

    isInputValido = FALSE;
    while (!isInputValido){
        printf("Seleziona la modalità di gioco:\n1) Player vs Computer\n2) Player vs Player\nmodalità: ");
        if (inputInt(&numeroModalita) && numeroModalita <= 2 && numeroModalita >= 1) {
            isInputValido = TRUE;
            if (numeroModalita == 1)
                modVsCPU = TRUE;
            else
                modVsCPU = FALSE;
        } else
            printf("Seleziona una modalità valida!\n");
    }

    /*ciclo che continua fino alla fine della partita*/
    while (!partita.isEnded) {
        /*stampo l'attuale situazione della partita*/
        printf("\n");
        draw(partita.scacchiera, LATO_SCACCHIERA);

        /*risetto la variabile isInputValido a FALSE per poter entrare nel ciclo che richiede la mossa*/
        isInputValido = FALSE;

        /*finchè l'utente non seleziona una mossa valida richiedo il numero mossa*/
        while (!isInputValido) {
            /*Stampo il colore cui spetta il turno*/
            printf("Turno: %s;\n", partita.turnoCorrente == BIANCO ? "Bianco" : "Nero");

            if (partita.turnoCorrente == NERO && modVsCPU) {
                mosseDisponibili = trovaMosseDisponibili(partita.scacchiera, LATO_SCACCHIERA, LATO_SCACCHIERA, partita.turnoCorrente);

                printf("Mosse disponibili:\n");
                for (i = 0; i < DYN_ARR_GET_SIZE(mosseDisponibili); ++i) {
                    printf("%d) ", i + 1);
                    stampaMossa(DYN_ARR_GET_ELEM(mosseDisponibili, i));
                    printf(" | ");
                }

                minimax(&partita, 5, NERO, NERO, &mossaMigliore);

                printf("\nMossa del computer: ");
                stampaMossa(mossaMigliore);

                /*effettuo la mossa*/
                muoviPedina(&partita, LATO_SCACCHIERA, mossaMigliore);
                isInputValido = TRUE;
            } else {
                /*ottengo le mosse disponibili*/
                mosseDisponibili = trovaMosseDisponibili(partita.scacchiera, LATO_SCACCHIERA, LATO_SCACCHIERA, partita.turnoCorrente);

                /*controllo se la partita è conclusa*/
                if (/*mosseDisponibili.size*/ DYN_ARR_GET_SIZE(mosseDisponibili) == 0) {
                    partita.isEnded = TRUE;
                    break;
                }

                printf("Mosse disponibili:\n");
                for (i = 0; i < /*mosseDisponibili.size*/ DYN_ARR_GET_SIZE(mosseDisponibili); ++i) {
                    printf("%d) ", i + 1);
                    /*stampaMossa(mosseDisponibili.mosse[i]);*/
                    stampaMossa(DYN_ARR_GET_ELEM(mosseDisponibili, i));
                    printf(" | ");
                }

                /* se non è la prima mossa della partita do la possibilità ai giocatori di arrendersi e di annullare l'ultima mossa effettuata */
                if (DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) > 0)
                    printf("%d) annulla l'ultima mossa | %d) Resa", i + 1, i + 2);

                printf("\nInserisci il numero corrispondente all'azione che vuoi effettuare: ");

                /*se la funzione controlloMossa ritorna 1 allora la mossa è valida ed esco dal ciclo altrimenti richiedo nuovamente la mossa*/
                if (inputInt(&numeroMossa) && numeroMossa <= /*mosseDisponibili.size*/ DYN_ARR_GET_SIZE(mosseDisponibili) && numeroMossa >= 1) {
                    /*effettuo la mossa*/
                    muoviPedina(&partita, LATO_SCACCHIERA, /*mosseDisponibili.mosse[numeroMossa - 1]*/
                                DYN_ARR_GET_ELEM(mosseDisponibili, numeroMossa - 1));

                    /*libero la memoria allocata per l'array mosseDisponibili*/
                    /*freeVet(&mosseDisponibili);*/
                    DYN_ARR_DESTROY(mosseDisponibili);

                    /*indico che la mossa è andata a buon fine*/
                    isInputValido = TRUE;
                } else if (numeroMossa == /*mosseDisponibili.size*/ DYN_ARR_GET_SIZE(mosseDisponibili) + 1 &&
                           DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) > 0) {
                    /* annullo l'ultima mossa dell'altro giocatore */
                    annullaUltimaMossa(&partita, LATO_SCACCHIERA);
                    isInputValido = TRUE;
                } else if (numeroMossa == /*mosseDisponibili.size*/ DYN_ARR_GET_SIZE(mosseDisponibili) + 2 &&
                           DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) > 0) {
                    /* il giocatore si è arreso */
                    partita.isEnded = TRUE;
                    break;
                } else
                    printf("\nMossa non valida! Inserire nuovamente una mossa valida.\n");
            }
        }
    }

    /* stampo il vincitore della partita */
    if (partita.isEnded && partita.turnoCorrente == BIANCO) {
        printf("\nIl giocatore Nero ha vinto!!\n");
    } else if (partita.isEnded && partita.turnoCorrente == NERO) {
        printf("\nIl giocatore Bianco ha vinto!!\n");
    }

    /*Libero la memoria allocata per la partita*/
    freePartita(partita, LATO_SCACCHIERA, LATO_SCACCHIERA);

    return 0;
}
