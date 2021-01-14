/** @file main.c
 *  @mainpage main Mini Laska
 *  @brief Implementa il gioco mini laska attraverso le funzioni definite su mini_laska.h
 */
#include <stdio.h>
#include "game_logic.h"
#include "graphics/graphics.h"
#include "utility/utility.h"

#define MAX_DIM_NAME_PLAYER (20)

/**
 * main del programma dove viene gestito il gioco tramite l'utilizzo di strutture e funzioni (vedere file mini_laska.h)
 * */
int main() {

    /*variabile per ciclo for*/
    int i;
    unsigned int numRand;
    /*variabile in cui salvo la mossa che l'utente vuole effettuare e variabile in cui salvo la modalità che l'utente desidera giocare */
    int numeroMossa, numeroModalita, difficolta;
    char new_game = 'y';

    /*varibile che serve per far accettare o meno l'annullamento della mossa da parte del giocatore avversario*/
    char annullaMossa;
    char player1[MAX_DIM_NAME_PLAYER], player2[MAX_DIM_NAME_PLAYER];/*nome utente per i giocatori*/
    /* variabile che indica la modalità di gioco scelta: true contro computer, false contro altro giocatore locale */
    bool_t modVsCPU;

    /*variabile che indica se la mossa inserita è valida o meno*/
    bool_t isInputValido;


    /* variabile contenete l'index della miglior mossa calcolata per la cpu*/
    mossa_t mossaMigliore;

    /*vettore dinamico mosse*/
    dyn_arr_mossa_t mosseDisponibili;


    while (new_game == 'y' || new_game == 'Y') {
        /*creo la variabile partita*/
        partita_t partita;

        /*inizializzo la partita e controllo che tutto sia andato a buon fine*/
        if (!init_game(&partita, LATO_SCACCHIERA, LATO_SCACCHIERA)) {
            freePartita(partita, LATO_SCACCHIERA, LATO_SCACCHIERA);
            printf("Errore inizializzazione partita.");
            return 1;
        }

        /*numero random per assegnare il colore ai due giocatori*/
        numRand = numeriRandom(0, 5);

        titolo();

        /*#if defined(_WIN32) || defined(_WIN64)
        menu_mod(&modVsCPU);
        menu_diff(&difficolta);
        menu_nome(&modVsCPU, &partita, &player1[0], &player2[0], &difficolta);

        if(numRand>=0 && numRand<3){
            partita.player1.colore=BIANCO;
            partita.player2.colore=NERO;
        } else{
            partita.player2.colore=BIANCO;
            partita.player1.colore=NERO;
        }
        #else*/


        isInputValido = FALSE;
        while (!isInputValido) {
            printf("Seleziona la modalita' di gioco:\n1) Player vs Computer\n2) Player vs Player\nmodalita': ");
            if (inputInt(&numeroModalita) && numeroModalita <= 2 && numeroModalita >= 1) {
                isInputValido = TRUE;
                if (numeroModalita == 1)
                    modVsCPU = TRUE;
                else
                    modVsCPU = FALSE;
            } else
                printf("\nSeleziona una modalita' valida!\n");
        }

        isInputValido = FALSE;
        if (modVsCPU) {

            printf("\nGiocatore, inserisci il tuo nome utente: ");
            scanf("%19s", player1);
            getchar();

            partita.player1.nome = player1;

            if (numRand >= 0 && numRand < 3) {
                partita.player1.colore = BIANCO;
                partita.player2.colore = NERO;
            } else {
                partita.player2.colore = BIANCO;
                partita.player1.colore = NERO;
            }

            while (!isInputValido) {
                printf("\nSeleziona la difficolta' di gioco:\n1) Wall-E (facile)\n2) VIKI (medio)\n3) HAL 9000 (difficile)\nmodalita': ");
                if (inputInt(&difficolta) && difficolta <= 3 && difficolta >= 1) {
                    isInputValido = TRUE;
                } else
                    printf("\nSeleziona una modalita' valida!\n");
            }

            switch (difficolta) {
                case 1:
                    partita.player2.nome = "Wall-E";
                    break;
                case 2:
                    partita.player2.nome = "VIKI";
                    break;
                case 3:
                    partita.player2.nome = "HAL 9000";
                    break;
                default:
                    partita.player2.nome = "Computer";
            }

        } else {/*fa inserire i nomi ai giocatori*/

            printf("\nGiocatore uno, inserisci il tuo nome utente: ");
            scanf("%19s", player1);
            getchar();

            partita.player1.nome = player1;

            printf("\nGiocatore due, inserisci il tuo nome utente: ");
            scanf("%s", player2);
            getchar();

            partita.player2.nome = player2;

            if (numRand >= 0 && numRand < 3) {
                partita.player1.colore = BIANCO;
                partita.player2.colore = NERO;
            } else {
                partita.player2.colore = BIANCO;
                partita.player1.colore = NERO;
            }
        }

        /*#endif*/

        /*ciclo che continua fino alla fine della partita*/
        while (!partita.isEnded) {
            /*stampo l'attuale situazione della partita ed evidenzio l'ultima mossa effettuata*/
            printf("\n");

            if (DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) == 0)
                /* #if defined(_WIN32) || defined(_WIN64)
                draw(partita.scacchiera, LATO_SCACCHIERA, NULL);
                #else */
                multiPlatformDraw(partita.scacchiera, LATO_SCACCHIERA, NULL);
                /* #endif */
            else
                /* #if defined(_WIN32) || defined(_WIN64)
                draw(partita.scacchiera, LATO_SCACCHIERA,
                     &DYN_ARR_GET_ELEM(partita.mosseDettagliatePartita, DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) - 1).mossa);
                #else */
                multiPlatformDraw(partita.scacchiera, LATO_SCACCHIERA,
                                  &DYN_ARR_GET_ELEM(partita.mosseDettagliatePartita,
                                                    DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) - 1).mossa);
                /* #endif */

            /*risetto la variabile isInputValido a FALSE per poter entrare nel ciclo che richiede la mossa*/
            isInputValido = FALSE;

            /*Stampo il colore cui spetta il turno*/

            /* printf("Turno: %s;\n", partita.turnoCorrente == BIANCO ? "Bianco" : "Nero");*/

            if (partita.turnoCorrente == BIANCO) {
                if (partita.player1.colore == BIANCO)
                    printf("Turno di %s, colore BIANCO\n", partita.player1.nome);
                else
                    printf("Turno di %s, colore BIANCO\n", partita.player2.nome);
            } else {
                if (partita.player1.colore == NERO)
                    printf("Turno di %s, colore NERO\n", partita.player1.nome);
                else
                    printf("Turno di %s, colore NERO\n", partita.player2.nome);
            }

            mosseDisponibili = trovaMosseDisponibili(partita.scacchiera, LATO_SCACCHIERA, LATO_SCACCHIERA, partita.turnoCorrente);

            /*controllo se la partita è conclusa*/
            if (DYN_ARR_GET_SIZE(mosseDisponibili) == 0) {
                printf("Nessuna mossa disponibile!\n");
                partita.isEnded = TRUE;
                DYN_ARR_DESTROY(mosseDisponibili);
                break;
            }

            /*finchè l'utente non seleziona una mossa valida richiedo il numero mossa*/
            while (!isInputValido) {
                if (modVsCPU && (partita.turnoCorrente == partita.player2.colore)) {/*il computer è sempre il player 2*/
                    printf("Mosse disponibili:\n");
                    for (i = 0; i < DYN_ARR_GET_SIZE(mosseDisponibili); ++i) {
                        printf("%d) ", i + 1);
                        stampaMossa(DYN_ARR_GET_ELEM(mosseDisponibili, i));
                        printf(" | ");
                    }

                    switch (difficolta) {
                        case 1:
                            minimax(&partita, 1, partita.player2.colore, partita.player2.colore, &mossaMigliore, evaluateBoard);
                            break;
                        case 2:
                            minimax(&partita, 3, partita.player2.colore, partita.player2.colore, &mossaMigliore, evaluateBoard);
                            break;
                        case 3:
                            minimax(&partita, 8, partita.player2.colore, partita.player2.colore, &mossaMigliore, evaluateBoard);
                            break;
                        default:
                            minimax(&partita, 1, partita.player2.colore, partita.player2.colore, &mossaMigliore, evaluateBoard);
                    }

                    printf("\nMossa del computer: ");
                    stampaMossa(mossaMigliore);

                    /*effettuo la mossa*/
                    muoviPedina(&partita, LATO_SCACCHIERA, mossaMigliore);
                    isInputValido = TRUE;
                    /* } else if (partita.turnoCorrente == BIANCO && modVsCPU) {
                         minimax(&partita, 9, BIANCO, BIANCO, &mossaMigliore, evaluateBoard);

                         printf("\nMossa del computer: ");
                         stampaMossa(mossaMigliore);

                         muoviPedina(&partita, LATO_SCACCHIERA, mossaMigliore);
                         isInputValido = TRUE;*/
                } else {
                    printf("Mosse disponibili:\n");
                    for (i = 0; i < DYN_ARR_GET_SIZE(mosseDisponibili); ++i) {
                        printf("%d) ", i + 1);
                        stampaMossa(DYN_ARR_GET_ELEM(mosseDisponibili, i));
                        printf(" | ");
                    }

                    /* se non è la prima mossa della partita do la possibilità ai giocatori di arrendersi e di annullare l'ultima mossa effettuata */
                    if (DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) > 1)
                        printf("%d) Richiedi aiuto | %d) Annulla l'ultima mossa | %d) Resa", i + 1, i + 2, i + 3);
                    else
                        printf("%d) Richiedi aiuto ", i + 1);

                    printf("\nInserisci il numero corrispondente all'azione che vuoi effettuare: ");

                    /*se la funzione controlloMossa ritorna 1 allora la mossa è valida ed esco dal ciclo altrimenti richiedo nuovamente la mossa*/
                    if (inputInt(&numeroMossa) && numeroMossa <= DYN_ARR_GET_SIZE(mosseDisponibili) && numeroMossa >= 1) {
                        /*effettuo la mossa*/
                        muoviPedina(&partita, LATO_SCACCHIERA, DYN_ARR_GET_ELEM(mosseDisponibili, numeroMossa - 1));

                        /*indico che la mossa è andata a buon fine*/
                        isInputValido = TRUE;
                    } else if (numeroMossa == DYN_ARR_GET_SIZE(mosseDisponibili) + 1) {
                        minimax(&partita, 8, partita.turnoCorrente, partita.turnoCorrente, &mossaMigliore, evaluateBoard);
                        printf("\nMossa suggerita: ");
                        stampaMossa(mossaMigliore);
                        printf("\n");
                    } else if (numeroMossa == DYN_ARR_GET_SIZE(mosseDisponibili) + 2 &&
                               DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) > 1) {

                        if (!modVsCPU) {
                            /*giocatore contro giocatore, chiede all'avversario il consenso per annullare la mossa*/
                            if (partita.turnoCorrente == partita.player1.colore)
                                printf("\n%s accetti l'annullamento della mossa? (y/n)\n", partita.player2.nome);
                            else
                                printf("\n%s accetti l'annullamento della mossa? (y/n)\n", partita.player1.nome);

                            scanf("\n%c", &annullaMossa);
                            getchar();/*pulisce il flusso di input*/

                            if (annullaMossa == 'Y' || annullaMossa == 'y') {
                                /* annullo l'ultima mossa del giocatore */
                                /* annulla mossa avversaria */
                                annullaUltimaMossa(&partita, LATO_SCACCHIERA);
                                /* annulla mossa giocatore */
                                annullaUltimaMossa(&partita, LATO_SCACCHIERA);
                            } else {
                                if (partita.turnoCorrente == partita.player1.colore) {
                                    printf("\n****************************************************************************\n");
                                    printf("\nMi spiace %s, il tuo avversario %s ha RIFIUTATO la tua richiesta.. :c\n",
                                           partita.player1.nome, partita.player2.nome);
                                    printf("\n****************************************************************************\n");
                                } else {
                                    printf("\n****************************************************************************\n");
                                    printf("\nMi spiace %s, il tuo avversario %s ha RIFIUTATO la tua richiesta.. :c\n",
                                           partita.player2.nome, partita.player1.nome);
                                    printf("\n****************************************************************************\n");
                                }
                            }
                        } else {
                            /*il computer sceglie a caso se accettare o no*/
                            if (partita.turnoCorrente == partita.player1.colore) {
                                numRand = numeriRandom(0, 5);
                                if (numRand >= 0 && numRand < 3)
                                    annullaMossa = 'y';
                                else
                                    annullaMossa = 'n';
                            } else {
                                printf("\n%s accetti l'annullamento della mossa? (y/n)\n", partita.player1.nome);

                                scanf("\n%c", &annullaMossa);
                                getchar();/*pulisce il flusso di input*/
                            }

                            if (annullaMossa == 'Y' || annullaMossa == 'y') {
                                /* annullo l'ultima mossa del giocatore */
                                /* annulla mossa avversaria */
                                annullaUltimaMossa(&partita, LATO_SCACCHIERA);
                                /* annulla mossa giocatore */
                                annullaUltimaMossa(&partita, LATO_SCACCHIERA);
                            } else {
                                if (partita.turnoCorrente == partita.player1.colore) {
                                    printf("\n****************************************************************************\n");
                                    printf("\nmi spiace %s, il tuo avversario %s ha RIFIUTATO la tua richiesta.. :c\n",
                                           partita.player1.nome, partita.player2.nome);
                                    printf("\n****************************************************************************\n");
                                } else {
                                    printf("\n****************************************************************************\n");
                                    printf("\nmi spiace %s, il tuo avversario %s ha RIFIUTATO la tua richiesta.. :c\n",
                                           partita.player2.nome, partita.player1.nome);
                                    printf("\n****************************************************************************\n");
                                }
                            }
                        }
                        isInputValido = TRUE;
                    } else if (numeroMossa == DYN_ARR_GET_SIZE(mosseDisponibili) + 3 &&
                               DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita) > 1) {
                        /* il giocatore si è arreso */
                        partita.isEnded = TRUE;
                        break;
                    } else
                        printf("\nMossa non valida! Inserire nuovamente una mossa valida.\n");
                }
            }

            /*libero la memoria allocata per l'array mosseDisponibili*/
            DYN_ARR_DESTROY(mosseDisponibili);
        }


        /* stampo il vincitore della partita */
        if (partita.isEnded && partita.turnoCorrente == BIANCO) {

            if (partita.player1.colore == BIANCO)
                printf("\nHa vinto %s\n", partita.player2.nome);
            else
                printf("\nHa vinto %s\n", partita.player1.nome);

        } else if (partita.isEnded && partita.turnoCorrente == NERO) {
            if (partita.player1.colore == NERO)
                printf("\nHa vinto %s\n", partita.player2.nome);
            else
                printf("\nHa vinto %s\n", partita.player1.nome);
        }

        printf("Numero mosse: %lu\n", DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita));

        /*Libero la memoria allocata per la partita*/
        freePartita(partita, LATO_SCACCHIERA, LATO_SCACCHIERA);

        do {
            isInputValido = FALSE;
            printf("\n\nSi vuole fare un'altra partita? (y\\n)\n");
            scanf("\n%c", &new_game);
            getchar();/*pulisce il flusso di input*/

            if (new_game == 'y' || new_game == 'n' || new_game == 'Y' || new_game == 'N')
                isInputValido = TRUE;
        } while (!isInputValido);
    }

    return 0;
}
