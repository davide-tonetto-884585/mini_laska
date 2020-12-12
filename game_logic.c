#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "game_logic.h"
#include "graphics/graphics.h"
#include "utility/utility.h"

bool_t init_game(partita_t *partita, size_t ROWS, size_t COLS) {
    /*variabili utilizzate per ciclare la matrice di Pedine*/
    int r, c;

    /*cicli che mi permettono di scorrere la matrice*/
    for (r = 0; r < ROWS; r++) {

        for (c = 0; c < COLS; c++) {
            /*inizializzo le Pedine nelle prime tre righe della matrice scacchiera e sulle caselle bianche della scacchiera di colore NERO*/
            /*if ((r + c) % 2 == 0) {*/

            if (r < ROWS / 2) {
                partita->scacchiera[atPosition(r, c, COLS)].pedine[0] = (pedina_t *) malloc(sizeof(pedina_t));

                /*controllo che la malloc sia andata a buon fine*/
                if (partita->scacchiera[atPosition(r, c, COLS)].pedine[0] == NULL) {
                    return FALSE;
                }

                partita->scacchiera[atPosition(r, c, COLS)].pedine[0]->colore = NERO;
                partita->scacchiera[atPosition(r, c, COLS)].pedine[0]->isPromossa = FALSE;

                partita->scacchiera[atPosition(r, c, COLS)].altezza = 1;
            } else if (r > ROWS / 2) {
                /*inizializzo le Pedine nelle ultime tre righe della matrice scacchiera e sulle caselle bianche della scacchiera di colore BIANCO*/
                partita->scacchiera[atPosition(r, c, COLS)].pedine[0] = (pedina_t *) malloc(sizeof(pedina_t));

                /*controllo che la malloc sia andata a buon fine*/
                if (partita->scacchiera[atPosition(r, c, COLS)].pedine[0] == NULL) {
                    return FALSE;
                }

                partita->scacchiera[atPosition(r, c, COLS)].pedine[0]->colore = BIANCO;
                partita->scacchiera[atPosition(r, c, COLS)].pedine[0]->isPromossa = FALSE;

                partita->scacchiera[atPosition(r, c, COLS)].altezza = 1;
            } else {
                /*delle rimanenti pedine ne inizializzo l'altezza a -1 per indicare che in quella posizione non si trova alcuna pedina*/
                partita->scacchiera[atPosition(r, c, COLS)].pedine[0] = NULL;
                partita->scacchiera[atPosition(r, c, COLS)].altezza = 0;
            }

            /*inizializzo le resyanti due pedine della cella a null*/
            partita->scacchiera[atPosition(r, c, COLS)].pedine[1] = NULL;
            partita->scacchiera[atPosition(r, c, COLS)].pedine[2] = NULL;

            /*}*/
        }
    }

    partita->turnoCorrente = BIANCO;
    partita->isEnded = FALSE;
    DYN_ARR_INIT_DEFAULT(partita->mosseDettagliatePartita);

    return TRUE;
}

bool_t controlloMossa(const cella_t *scacchiera, size_t ROWS, size_t COLS, mossa_t mossa, enum colore turnoCorrente) {
    /*converto le coodrdinate alfabetiche in intere*/
    int colonnaPedinaInt = mossa.posizionePedina.colonna - 97;
    int colonnaFinaleInt = mossa.posizioneFinale.colonna - 97;

    /*salvo l'altezza della pedina da muovere su una variabile*/
    int altezzaPedina;

    /*variabile che utilizzo per controllare la validità di una mossa perchè se il colore che sta muovendo è il bianco
     * allora dovrò controllare che la pedina si muova correttamente (riga - 1) mentre se il colore è il nero si dovrà muovere
     * (riga + 1)*/
    int direzioneColore;

    /*controllo che le coordinate delle colonne siano valide*/
    if (colonnaFinaleInt < 0 || colonnaFinaleInt > COLS - 1 || colonnaPedinaInt < 0 || colonnaPedinaInt > COLS - 1)
        return FALSE;

    /*controllo che le coordinate delle righe siano valide*/
    if (mossa.posizioneFinale.riga < 1 || mossa.posizioneFinale.riga > ROWS || mossa.posizionePedina.riga < 1 || mossa.posizionePedina.riga > ROWS)
        return FALSE;

    /*inverto la coordinata della riga per poter accedere correttamente alla matrice*/
    mossa.posizionePedina.riga = abs(mossa.posizionePedina.riga - (int) ROWS);
    mossa.posizioneFinale.riga = abs(mossa.posizioneFinale.riga - (int) ROWS);

    altezzaPedina = scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].altezza;

    /*controllo se la pedina che l'utente vuole muovere è del proprio colore o non è una casella vuota*/
    if (altezzaPedina == 0 || scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[altezzaPedina - 1]->colore != turnoCorrente)
        return FALSE;

    /*-----------------------------controllo che la mossa sia valida-------------------------------------*/
    /*controllo se la casella su cui l'utente vuole muovere è già occupata da un'altra pedina*/
    if (scacchiera[atPosition(mossa.posizioneFinale.riga, colonnaFinaleInt, ROWS)].altezza != 0)
        return FALSE;

    /*controllo il turno corrente e in base setto la corretta direzione della pedina in funzione della matrice*/
    if (turnoCorrente == BIANCO)
        direzioneColore = -1;
    else
        direzioneColore = 1;

    /*controllo che la mossa sia effettivamente valida (che abbia mosso diagonalmente verso avanti, o indietro in caso la pedina sia promossa)*/
    if ((colonnaFinaleInt == colonnaPedinaInt + 1 || colonnaFinaleInt == colonnaPedinaInt - 1) &&
        (mossa.posizioneFinale.riga == mossa.posizionePedina.riga + (1 * direzioneColore) ||
         (mossa.posizioneFinale.riga == mossa.posizionePedina.riga - (1 * direzioneColore) &&
          scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[altezzaPedina - 1]->isPromossa))) {
        return TRUE;
    }
        /*controllo che, se la mossa si muove di due righe (per mangiare), questa sia effettivamente valida*/
    else if (((colonnaFinaleInt == colonnaPedinaInt + 2 || colonnaFinaleInt == colonnaPedinaInt - 2) &&
              (mossa.posizioneFinale.riga == mossa.posizionePedina.riga + (2 * direzioneColore) ||
               (mossa.posizioneFinale.riga == mossa.posizionePedina.riga - (2 * direzioneColore) &&
                scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[altezzaPedina - 1]->isPromossa))) &&
             scacchiera[atItermediatePosition(mossa, ROWS)].altezza != 0 &&
             scacchiera[atItermediatePosition(mossa, ROWS)].pedine[scacchiera[atItermediatePosition(mossa, ROWS)].altezza - 1]->colore != turnoCorrente) {
        return TRUE;
    } else
        return FALSE;
}

dyn_arr_mossa_t trovaMosseDisponibili(const cella_t *scacchiera, size_t ROWS, size_t COLS, enum colore turnoCorrente) {
    /*variabili usate per ciclare la scacchiera*/
    int i, j;

    /*vettori dinamici usati per memorizzare le mosse*/
    dyn_arr_mossa_t mosseValide, mosseValideConquista;

    /*variabili usate per memorizzare l'altezza della pedina e la direzione in base al colore*/
    int altezzaColonna;
    int direzioneColore;

    /*variabile usata per salvare la mossa da testare*/
    mossa_t mossa;

    /*inizializzo i vettori*/
    DYN_ARR_INIT(mosseValide, 5);
    DYN_ARR_INIT(mosseValideConquista, 5);

    /* if (!initVet(&mosseValide, 10) || !initVet(&mosseValideConquista, 10)) {
        exit(EXIT_FAILURE);
    } */

    /*setto la direzione*/
    if (turnoCorrente == BIANCO)
        direzioneColore = 1;
    else
        direzioneColore = -1;

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {

            /*evito di controllare le celle nere*/
            if ((i + j) % 2 != 0)
                continue;

            /*se la cella è vuota la salto*/
            if (scacchiera[atPosition(i, j, COLS)].altezza == 0)
                continue;

            altezzaColonna = scacchiera[atPosition(i, j, COLS)].altezza;

            /*controllo che il colore della pedina sia corretto*/
            if (scacchiera[atPosition(i, j, COLS)].pedine[altezzaColonna - 1]->colore == turnoCorrente) {
                mossa.posizionePedina.riga = (int) ROWS - i;
                mossa.posizionePedina.colonna = (char) (j + 97);

                /*controllo prima se ci sono mosse di conquista valide (regola obbligo di mangiare)*/
                mossa.posizioneFinale.riga = mossa.posizionePedina.riga + (2 * direzioneColore);
                mossa.posizioneFinale.colonna = (char) (mossa.posizionePedina.colonna + 2);

                if (controlloMossa(scacchiera, ROWS, COLS, mossa, turnoCorrente))
                    DYN_ARR_PUSH(mosseValideConquista, mossa);

                mossa.posizioneFinale.colonna = (char) (mossa.posizionePedina.colonna - 2);

                if (controlloMossa(scacchiera, ROWS, COLS, mossa, turnoCorrente))
                    DYN_ARR_PUSH(mosseValideConquista, mossa);

                /*se la pedina è promossa allora controllo anche le altre due posizioni*/
                if (scacchiera[atPosition(i, j, COLS)].pedine[altezzaColonna - 1]->isPromossa) {
                    mossa.posizioneFinale.riga = mossa.posizionePedina.riga - (2 * direzioneColore);
                    mossa.posizioneFinale.colonna = (char) (mossa.posizionePedina.colonna + 2);


                    if (controlloMossa(scacchiera, ROWS, COLS, mossa, turnoCorrente))
                        DYN_ARR_PUSH(mosseValideConquista, mossa);

                    mossa.posizioneFinale.colonna = (char) (mossa.posizionePedina.colonna - 2);

                    if (controlloMossa(scacchiera, ROWS, COLS, mossa, turnoCorrente))
                        DYN_ARR_PUSH(mosseValideConquista, mossa);
                }

                if (DYN_ARR_GET_SIZE(mosseValideConquista) > 0)
                    continue;/*rende il vettore perchè tanto è obbligato a fare mosse di conquista se ne ha trovate*/

                /*controllo se ci sono mosse di spostamento valide per la pedina*/
                mossa.posizioneFinale.riga = mossa.posizionePedina.riga + (1 * direzioneColore);
                mossa.posizioneFinale.colonna = (char) (mossa.posizionePedina.colonna + 1);

                if (controlloMossa(scacchiera, ROWS, COLS, mossa, turnoCorrente))
                    DYN_ARR_PUSH(mosseValide, mossa);

                mossa.posizioneFinale.colonna = (char) (mossa.posizionePedina.colonna - 1);

                if (controlloMossa(scacchiera, ROWS, COLS, mossa, turnoCorrente))
                    DYN_ARR_PUSH(mosseValide, mossa);

                /*se la pedina è promossa allora controllo anche le altre due posizioni*/
                if (scacchiera[atPosition(i, j, COLS)].pedine[altezzaColonna - 1]->isPromossa) {
                    mossa.posizioneFinale.riga = mossa.posizionePedina.riga - (1 * direzioneColore);
                    mossa.posizioneFinale.colonna = (char) (mossa.posizionePedina.colonna + 1);

                    if (controlloMossa(scacchiera, ROWS, COLS, mossa, turnoCorrente))
                        DYN_ARR_PUSH(mosseValide, mossa);

                    mossa.posizioneFinale.colonna = (char) (mossa.posizionePedina.colonna - 1);

                    if (controlloMossa(scacchiera, ROWS, COLS, mossa, turnoCorrente))
                        DYN_ARR_PUSH(mosseValide, mossa);
                }
            }
        }
    }

    /*ritorno il vettore corretto e libero quello che non verrà ritornato*/
    if (DYN_ARR_GET_SIZE(mosseValideConquista) > 0) {
        DYN_ARR_DESTROY(mosseValide);
        return mosseValideConquista;
    }

    DYN_ARR_DESTROY(mosseValideConquista);
    return mosseValide;
}

mossa_dettagliata_t muoviPedina(partita_t *partita, size_t COLS, mossa_t mossa) {
    /*variabile usata per cicli for*/
    int i;

    /*salvo le altezze delle pedina*/
    int altezzaColonnaDaConquistare, altezzaColonnaDaMuovere;

    pedina_t *commander;

    /*resoconto mossa*/
    mossa_dettagliata_t mossaDettagliata;

    /*converto le coodrdinate alfabetiche in intere*/
    int colonnaPedinaInt = mossa.posizionePedina.colonna - 97;
    int colonnaFinaleInt = mossa.posizioneFinale.colonna - 97;

    mossaDettagliata.pedinaEliminata = NULL;
    mossaDettagliata.mossa = mossa;

    /*inverto la coordinata della riga per poter accedere correttamente alla matrice*/
    mossa.posizionePedina.riga = abs(mossa.posizionePedina.riga - (int) COLS);
    mossa.posizioneFinale.riga = abs(mossa.posizioneFinale.riga - (int) COLS);

    altezzaColonnaDaMuovere = partita->scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].altezza;

    /*controllo se la mossa è di conquista*/
    if ((colonnaFinaleInt == colonnaPedinaInt + 2 || colonnaFinaleInt == colonnaPedinaInt - 2) &&
        (mossa.posizioneFinale.riga == mossa.posizionePedina.riga - 2 ||
         mossa.posizioneFinale.riga == mossa.posizionePedina.riga + 2)) {

        mossaDettagliata.hasConquered = TRUE;

        altezzaColonnaDaConquistare = partita->scacchiera[atItermediatePosition(mossa, COLS)].altezza;

        /*gestisco la cella che conquista (se l'altezza della pedina è 3 allora shiftA non riuscirà ad effettuare lo spostamento e ritornerà 0 saltando così il blocco)*/
        if (ShiftADX(partita->scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].pedine,
                     altezzaColonnaDaMuovere, 0, 3)) {

            partita->scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].pedine[0] =
                    partita->scacchiera[atItermediatePosition(mossa, COLS)].pedine[altezzaColonnaDaConquistare - 1];

            partita->scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].altezza++;
            altezzaColonnaDaMuovere++;
        } else {
            /*se la cella che sta conquistando è gia alta tre allora la pedina più alta della cella conquistata
             * verrà semplicemente rimossa dal gioco e salvata nel resoconto della mossa (ciò per permettere l'undo della mossa) */
            /*free(scacchiera[atItermediatePosition(mossa, COLS)].pedine[altezzaColonnaDaConquistare - 1]);*/
            mossaDettagliata.pedinaEliminata = partita->scacchiera[atItermediatePosition(mossa, COLS)].pedine[
                    altezzaColonnaDaConquistare - 1];
        }

        /*gestisco la cella che viene conquistata rimuovendo la pedina più alya della cella*/
        partita->scacchiera[atItermediatePosition(mossa, COLS)].pedine[altezzaColonnaDaConquistare - 1] = NULL;

        partita->scacchiera[atItermediatePosition(mossa, COLS)].altezza--;
    } else
        mossaDettagliata.hasConquered = FALSE;

    /*sposto la pedina e le sue configurazioni nella nuova casella*/
    for (i = 0; i < altezzaColonnaDaMuovere; i++) {
        partita->scacchiera[atPosition(mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].pedine[i]
                = partita->scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].pedine[i];

        /*Setto il vecchio array pedine a null*/
        partita->scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].pedine[i] = NULL;
    }

    /*setto la nuova altezza della cella di destinazione*/
    partita->scacchiera[atPosition(mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].altezza
            = partita->scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].altezza;

    /*rendo la vecchia posizione della pedina nulla*/
    partita->scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].altezza = 0;

    commander = partita->scacchiera[atPosition(mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].pedine[altezzaColonnaDaMuovere - 1];

    /* controllo se la pedina va promossa */
    if (((mossa.posizioneFinale.riga == 0 && commander->colore == BIANCO) ||
         (mossa.posizioneFinale.riga == COLS - 1 && commander->colore == NERO)) && !commander->isPromossa) {

        commander->isPromossa = TRUE;
        mossaDettagliata.hasBeenPromoted = TRUE;
    } else
        mossaDettagliata.hasBeenPromoted = FALSE;

    /* cambio il turno */
    switchTurno(&partita->turnoCorrente);

    DYN_ARR_PUSH(partita->mosseDettagliatePartita, mossaDettagliata);
    return mossaDettagliata;
}

bool_t annullaUltimaMossa(partita_t *partita, size_t COLS) {
    /*variabile usata per cicli for*/
    int i;

    /*salvo le altezze delle pedina*/
    int altezzaColonnaIntermedia, altezzaColonnaMovente;

    /* variabile in cui salvo l'ultima mossa della partita */
    mossa_dettagliata_t dettagliUltimaMossa;

    /* variabili coordinate colonne convertite a intero */
    int colonnaPedinaInt, colonnaFinaleInt;

    /* controllo che nella partita siano state effettuate mosse */
    if (DYN_ARR_GET_SIZE(partita->mosseDettagliatePartita) == 0)
        return FALSE;

    dettagliUltimaMossa = DYN_ARR_GET_ELEM(partita->mosseDettagliatePartita, DYN_ARR_GET_SIZE(partita->mosseDettagliatePartita) - 1);

    /*converto le coodrdinate alfabetiche in intere*/
    colonnaPedinaInt = dettagliUltimaMossa.mossa.posizionePedina.colonna - 97;
    colonnaFinaleInt = dettagliUltimaMossa.mossa.posizioneFinale.colonna - 97;

    /*inverto la coordinata della riga per poter accedere correttamente alla matrice*/
    dettagliUltimaMossa.mossa.posizionePedina.riga = abs(dettagliUltimaMossa.mossa.posizionePedina.riga - (int) COLS);
    dettagliUltimaMossa.mossa.posizioneFinale.riga = abs(dettagliUltimaMossa.mossa.posizioneFinale.riga - (int) COLS);

    altezzaColonnaMovente = partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].altezza;

    /* controllo se la mossa era di conquista */
    if (dettagliUltimaMossa.hasConquered) {

        altezzaColonnaIntermedia = partita->scacchiera[atItermediatePosition(dettagliUltimaMossa.mossa, COLS)].altezza;

        /* controllo se erano state rimosse pedine dalla partita */
        if (dettagliUltimaMossa.pedinaEliminata != NULL) {
            partita->scacchiera[atItermediatePosition(dettagliUltimaMossa.mossa, COLS)].pedine[altezzaColonnaIntermedia] = dettagliUltimaMossa.pedinaEliminata;
        } else {
            partita->scacchiera[atItermediatePosition(dettagliUltimaMossa.mossa, COLS)].pedine[altezzaColonnaIntermedia]
                    = partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].pedine[0];

            /* rimuovo l'elemento conquistato dalla pedina che aveva mosso */
            for (i = 0; i < altezzaColonnaMovente; i++) {
                partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].pedine[i]
                        = partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].pedine[i + 1];
            }

            partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].pedine[altezzaColonnaMovente - 1] = NULL;
            partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].altezza--;
            altezzaColonnaMovente--;
        }

        partita->scacchiera[atItermediatePosition(dettagliUltimaMossa.mossa, COLS)].altezza++;
    }

    /* riporto la pedina alla posizione iniziale */
    for (i = 0; i < altezzaColonnaMovente; i++) {
        partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].pedine[i] =
                partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].pedine[i];

        /*Setto il vecchio array pedine a null*/
        partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].pedine[i] = NULL;
    }

    partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].altezza =
            partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].altezza;

    partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizioneFinale.riga, colonnaFinaleInt, COLS)].altezza = 0;

    /* annullo la promozione se precedentemente la si è effettuata */
    if (dettagliUltimaMossa.hasBeenPromoted)
        partita->scacchiera[atPosition(dettagliUltimaMossa.mossa.posizionePedina.riga, colonnaPedinaInt, COLS)].pedine[
                altezzaColonnaMovente - 1]->isPromossa = FALSE;

    switchTurno(&partita->turnoCorrente);

    DYN_ARR_REMOVE_ELEM(partita->mosseDettagliatePartita, DYN_ARR_GET_SIZE(partita->mosseDettagliatePartita) - 1);

    return TRUE;
}

int _minimax(partita_t *partita, size_t ROWS, size_t COLS, int maxDepth, int currentDepth, enum colore maxPlayer,
             enum colore turno, mossa_t *mossaMigliore, int (*evaluateBoard)(const cella_t *, size_t, size_t)) {
    int i, bestScore, evaluation;
    mossa_t mossaContraria;

    dyn_arr_mossa_t mosseDisponibili = trovaMosseDisponibili(partita->scacchiera, ROWS, COLS, turno);

    if (maxDepth == currentDepth || DYN_ARR_GET_SIZE(mosseDisponibili) == 0) {
        DYN_ARR_DESTROY(mosseDisponibili);
        return evaluateBoard(partita->scacchiera, ROWS, COLS);
    }

    if (turno == BIANCO) {
        bestScore = INT_MIN;

        for (i = 0; i < DYN_ARR_GET_SIZE(mosseDisponibili); i++) {
            mossa_t mossa = DYN_ARR_GET_ELEM(mosseDisponibili, i);
            muoviPedina(partita, COLS, mossa);
            evaluation = _minimax(partita, ROWS, COLS, maxDepth, currentDepth + 1, maxPlayer, NERO, mossaMigliore, evaluateBoard);
            bestScore = max(bestScore, evaluation);

            if (evaluation == bestScore && BIANCO == maxPlayer && currentDepth == 0)
                *mossaMigliore = mossa;

            annullaUltimaMossa(partita, COLS);
        }
    } else {
        bestScore = INT_MAX;

        for (i = 0; i < DYN_ARR_GET_SIZE(mosseDisponibili); i++) {
            mossa_t mossa = DYN_ARR_GET_ELEM(mosseDisponibili, i);
            muoviPedina(partita, COLS, mossa);
            evaluation = _minimax(partita, ROWS, COLS, maxDepth, currentDepth + 1, maxPlayer, BIANCO, mossaMigliore, evaluateBoard);
            bestScore = min(bestScore, evaluation);

            if (evaluation == bestScore && NERO == maxPlayer && currentDepth == 0)
                *mossaMigliore = mossa;

            annullaUltimaMossa(partita, COLS);
        }
    }

    DYN_ARR_DESTROY(mosseDisponibili);

    return bestScore;
}

int minimax(partita_t *partita, int maxDepth, enum colore maxPlayer, enum colore turno,
            mossa_t *mossaMigliore, int (*evaluateBoard)(const cella_t *, size_t, size_t)) {
    return _minimax(partita, LATO_SCACCHIERA, LATO_SCACCHIERA, maxDepth, 0, maxPlayer, turno, mossaMigliore, evaluateBoard);
}

int evaluateBoard(const cella_t *scacchiera, size_t ROWS, size_t COLS) {
    int score = 0, i, j, segnoColore;

    for (i = 0; i < ROWS; ++i) {
        for (j = 0; j < COLS; ++j) {
            if (scacchiera[atPosition(i, j, COLS)].altezza != 0) {
                if (scacchiera[atPosition(i, j, COLS)].pedine[scacchiera[atPosition(i, j, COLS)].altezza - 1]->colore == BIANCO)
                    segnoColore = 1;
                else
                    segnoColore = -1;

                if (scacchiera[atPosition(i, j, COLS)].pedine[scacchiera[atPosition(i, j, COLS)].altezza - 1]->isPromossa)
                    score += 2 * segnoColore;
                else
                    score += 1 * segnoColore;
            }
        }
    }

    return score;
}

void freePartita(partita_t partita, size_t ROWS, size_t COLS) {
    int r, c, i;
    mossa_dettagliata_t mossaDettagliata;

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            if (partita.scacchiera[atPosition(r, c, COLS)].altezza != 0)
                for (i = 0; i < partita.scacchiera[atPosition(r, c, COLS)].altezza; i++)
                    free(partita.scacchiera[atPosition(r, c, COLS)].pedine[i]);
        }
    }

    for (i = 0; i < DYN_ARR_GET_SIZE(partita.mosseDettagliatePartita); ++i) {
        mossaDettagliata = DYN_ARR_GET_ELEM(partita.mosseDettagliatePartita, i);
        if (mossaDettagliata.pedinaEliminata != NULL)
            free(mossaDettagliata.pedinaEliminata);
    }

    DYN_ARR_DESTROY(partita.mosseDettagliatePartita);
}

