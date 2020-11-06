#include <stdio.h>
#include <stdlib.h>
#include "prototipi.h"

int init_game(Colonna *scacchiera, int ROWS, int COLS) {
    /*variabili utilizzate per ciclare la matrice di Pedine*/
    int r, c;

    /*cicli che mi permettono di scorrere la matrice di Pedine*/
    for (r = 0; r < ROWS; r++) {

        for (c = 0; c < COLS; c++) {
            /*inizializzo le Pedine nelle prime tre righe della matrice scacchiera e sulle caselle bianche della scacchiera di colore NERO*/
            if ((r + c) % 2 == 0) {

                if (r < ROWS / 2) {
                    scacchiera[atPosition(r, c, COLS)].pedine[0] = (Pedina *) malloc(sizeof(Pedina));

                    /*controllo che la malloc sia andata a buon fine*/
                    if (scacchiera[atPosition(r, c, COLS)].pedine[0] == NULL) {
                        return 0;
                    }

                    scacchiera[atPosition(r, c, COLS)].pedine[0]->colore = NERO;
                    scacchiera[atPosition(r, c, COLS)].pedine[0]->isPromossa = 0;

                    scacchiera[atPosition(r, c, COLS)].altezza = 1;
                } else if (r > ROWS / 2) {
                    /*inizializzo le Pedine nelle ultime tre righe della matrice scacchiera e sulle caselle bianche della scacchiera di colore BIANCO*/
                    scacchiera[atPosition(r, c, COLS)].pedine[0] = (Pedina *) malloc(sizeof(Pedina));

                    /*controllo che la malloc sia andata a buon fine*/
                    if (scacchiera[atPosition(r, c, COLS)].pedine[0] == NULL) {
                        return 0;
                    }

                    scacchiera[atPosition(r, c, COLS)].pedine[0]->colore = BIANCO;
                    scacchiera[atPosition(r, c, COLS)].pedine[0]->isPromossa = 0;

                    scacchiera[atPosition(r, c, COLS)].altezza = 1;
                } else {
                    /*delle rimanenti pedine ne inizializzo l'altezza a -1 per indicare che in quella posizione non si trova alcuna pedina*/
                    scacchiera[atPosition(r, c, COLS)].pedine[0] = NULL;
                    scacchiera[atPosition(r, c, COLS)].altezza = 0;
                }

                /*inizializzo le resyanti due pedine della colonna a null*/
                scacchiera[atPosition(r, c, COLS)].pedine[1] = NULL;
                scacchiera[atPosition(r, c, COLS)].pedine[2] = NULL;
            }
        }
    }

    return 1;
}

void draw(Colonna *scacchiera, int lato) {
    /*variabili utilizzate per ciclare i vari for*/
    int rig, col, i, k, t;

    /*scorro le righe della scacchiera da 0 a lato - 1*/
    for (rig = 0; rig < lato + 1; rig++) {
        printf("+");

        /*ogni colonna stampo il contorno delle caselle*/
        for (col = 0; col < lato; col++) {
            printf("----------");
        }

        printf("+\n");

        /*controllo di non essere arrivato all'ultima riga*/
        if (rig != lato)

            /*ciclo altre tre volte per definire l'altezza di ogni riga*/
            for (i = 0; i < 3; i++) {
                printf("|");

                /*ciclo nuovamente le colonne per colorarle e disegnare le pedine*/
                for (k = 0; k < lato; k++) {

                    /*controllo che permette di colorare la cella di nero o bianco*/
                    if ((rig + k) % 2 != 0)
                        printf("##########");
                    else {
                        /*controllo se mi trovo nella posizione centrale della riga*/
                        if (i == 1)

                            /*se la pedina nella casella in cui mi trovo ha altezza -1 significa che è nulla e non la rappresento, altrimenti si*/
                            if (scacchiera[atPosition(rig, k, lato)].altezza == 0)
                                printf("    //    ");
                            else {
                                printf("    ");

                                /*Scorro la torre*/
                                for (t = 0; t < 3; t++) {
                                    if (scacchiera[atPosition(rig, k, lato)].pedine[t] != NULL)
                                        printf("%c", scacchiera[atPosition(rig, k, lato)].pedine[t]->colore);
                                    else
                                        printf(" ");
                                }

                                printf("   ");
                            }
                        else if (i == 2) /*stampo le coordinate della casella attuale*/
                            printf(" %c%d       ", k + 97, abs(rig - lato));
                        else
                            printf("          ");
                    }

                    /*controllo se mi trovo a fine ciclo definisco il contorno della scacchiera*/
                    if (k == lato - 1)
                        printf("|\n");
                }
            }
    }

}

int controlloMossa(Colonna *scacchiera, int ROWS, int COLS, Mossa mossa, enum colore turnoCorrente) {
    /*converto le coodrdinate alfabetiche in intere*/
    int colonnaPedinaInt = mossa.posizionePedina.colonna - 97;
    int colonnaFinaleInt = mossa.posizioneFinale.colonna - 97;

    /*salvo l'altezza della pedina da muovere su una variabile*/
    int altezzaPedina;

    /*variabile che utilizzo per controllare la validità di una mossa perchè se il colore che sta muovendo è il bianco
     * allora dovrò controllare che la pedina si muova correttamente (riga - 1) mentre se il colore è il nero si dovrà muovere
     * (riga + 1)*/
    int direzioneColore;

    /*inverto la coordinata della riga per poter accedere correttamente alla matrice*/
    mossa.posizionePedina.riga = abs(mossa.posizionePedina.riga - ROWS);
    mossa.posizioneFinale.riga = abs(mossa.posizioneFinale.riga - ROWS);

    altezzaPedina = scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].altezza;

    /*controllo che le coordinate delle colonne siano valide*/
    if (colonnaFinaleInt < 0 || colonnaFinaleInt > ROWS - 1 || colonnaPedinaInt < 0 || colonnaPedinaInt > ROWS - 1)
        return 0;

    /*controllo che le coordinate delle righe siano valide*/
    if (mossa.posizioneFinale.riga < 0 || mossa.posizioneFinale.riga > COLS - 1 || mossa.posizionePedina.riga < 0 ||
        mossa.posizionePedina.riga > COLS - 1)
        return 0;

    /*controllo se la pedina che l'utente vuole muovere è del proprio colore o non è una casella vuota*/
    if (altezzaPedina == 0 ||
        scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[altezzaPedina - 1]->colore !=
        turnoCorrente)
        return 0;

    /*-----------------------------controllo che la mossa sia valida-------------------------------------*/
    /*controllo se la casella su cui l'utente vuole muovere è già occupata da un'altra pedina*/
    if (scacchiera[atPosition(mossa.posizioneFinale.riga, colonnaFinaleInt, ROWS)].altezza != 0)
        return 0;

    /*controllo il turno corrente e in base setto la corretta direzione della pedina in funzione della matrice*/
    if (turnoCorrente == BIANCO)
        direzioneColore = -1;
    else
        direzioneColore = 1;

    /*controllo che la mossa sia effettivamente valida (che abbia mosso diagonalmente verso avanti, o indietro in caso la pedina sia promossa)*/
    if ((colonnaFinaleInt == colonnaPedinaInt + 1 || colonnaFinaleInt == colonnaPedinaInt - 1) &&
        (mossa.posizioneFinale.riga == mossa.posizionePedina.riga + (1 * direzioneColore) ||
         (mossa.posizioneFinale.riga == mossa.posizionePedina.riga - (1 * direzioneColore) &&
          scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[altezzaPedina -
                                                                                            1]->isPromossa))) {
        return 1;
    }
        /*controllo che, se la mossa si muove di due righe (per mangiare), questa sia effettivamente valida*/
    else if (((colonnaFinaleInt == colonnaPedinaInt + 2 || colonnaFinaleInt == colonnaPedinaInt - 2) &&
              (mossa.posizioneFinale.riga == mossa.posizionePedina.riga + (2 * direzioneColore) ||
               (mossa.posizioneFinale.riga == mossa.posizionePedina.riga - (2 * direzioneColore) &&
                scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[altezzaPedina -
                                                                                                  1]->isPromossa))) &&
             scacchiera[atItermediatePosition(mossa.posizionePedina.riga, mossa.posizioneFinale.riga,
                                              colonnaPedinaInt, colonnaFinaleInt, ROWS)].altezza != 0 &&
             scacchiera[atItermediatePosition(mossa.posizionePedina.riga, mossa.posizioneFinale.riga,
                                              colonnaPedinaInt, colonnaFinaleInt, ROWS)].pedine[altezzaPedina -
                                                                                                1]->colore !=
             turnoCorrente) {
        return 1;
    } else
        return 0;
}

void muoviPedina(Colonna *scacchiera, int ROWS, Mossa mossa) {
    /*variabile usata per cicli for*/
    int i;

    /*salvo le altezze delle pedina*/
    int altezzaPedinaDaConquistare, altezzaPedinaDaMuovere;

    /*converto le coodrdinate alfabetiche in intere*/
    int colonnaPedinaInt = mossa.posizionePedina.colonna - 97;
    int colonnaFinaleInt = mossa.posizioneFinale.colonna - 97;

    /*inverto la coordinata della riga per poter accedere correttamente alla matrice*/
    mossa.posizionePedina.riga = abs(mossa.posizionePedina.riga - ROWS);
    mossa.posizioneFinale.riga = abs(mossa.posizioneFinale.riga - ROWS);

    /*controllo se la mossa è di conquista*/
    if ((colonnaFinaleInt == colonnaPedinaInt + 2 || colonnaFinaleInt == colonnaPedinaInt - 2) &&
        (mossa.posizioneFinale.riga == mossa.posizionePedina.riga - 2 ||
         mossa.posizioneFinale.riga == mossa.posizionePedina.riga + 2)) {

        altezzaPedinaDaMuovere = scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].altezza;
        altezzaPedinaDaConquistare = scacchiera[atItermediatePosition(mossa.posizionePedina.riga,
                                                                      mossa.posizioneFinale.riga, colonnaPedinaInt,
                                                                      colonnaFinaleInt, ROWS)].altezza;

        /*gestisco la colonna che conquista (se l'altezza della pedina è 3 allora shiftA non riuscirà ad effettuare lo spostamento e ritornerà 0 saltando il blocco)*/
        if (ShiftA(scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine,
                   altezzaPedinaDaMuovere,
                   0,
                   3)) {

            scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[0] =
                    scacchiera[atItermediatePosition(mossa.posizionePedina.riga,
                                                     mossa.posizioneFinale.riga,
                                                     colonnaPedinaInt,
                                                     colonnaFinaleInt, ROWS)].pedine[altezzaPedinaDaConquistare - 1];

            scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].altezza++;
        } else {
            /*se la colonna che sta conquistando è gia alta tre allora la pedina più alta della colonna conquistata verrà semplicemente rimossa dal gioco*/
            free(scacchiera[atItermediatePosition(mossa.posizionePedina.riga,
                                                  mossa.posizioneFinale.riga,
                                                  colonnaPedinaInt,
                                                  colonnaFinaleInt, ROWS)].pedine[altezzaPedinaDaConquistare - 1]);
        }

        /*gestisco la colonna che viene conquistatarimuovendo la pedina più alya della colonna*/
        scacchiera[atItermediatePosition(mossa.posizionePedina.riga, mossa.posizioneFinale.riga, colonnaPedinaInt,
                                         colonnaFinaleInt, ROWS)].pedine[altezzaPedinaDaConquistare - 1] = NULL;

        scacchiera[atItermediatePosition(mossa.posizionePedina.riga, mossa.posizioneFinale.riga, colonnaPedinaInt,
                                         colonnaFinaleInt, ROWS)].altezza--;
    }

    /*sposto la pedina e le sue configurazioni nella nuova casella*/
    for (i = 0; i < scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].altezza; i++) {
        scacchiera[atPosition(mossa.posizioneFinale.riga, colonnaFinaleInt,
                              ROWS)].pedine[i] = scacchiera[atPosition(
                mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[i];

        /*Setto il vecchio array pedine a null*/
        scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].pedine[i] = NULL;
    }

    /*setto la nuova altezza della colonna di destinazione*/
    scacchiera[atPosition(mossa.posizioneFinale.riga, colonnaFinaleInt, ROWS)].altezza = scacchiera[atPosition(
            mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].altezza;

    /*rendo la vecchia posizione della pedina nulla*/
    scacchiera[atPosition(mossa.posizionePedina.riga, colonnaPedinaInt, ROWS)].altezza = 0;
}

enum colore switchTurno(enum colore turnoCorrente) {
    if (turnoCorrente == BIANCO)
        return NERO;
    else
        return BIANCO;
}

inline int atPosition(int row, int col, int width) {
    return row * width + col;
}

inline int atItermediatePosition(int initialRow, int finalRow, int initialCol, int finalCol, int width) {
    return ((initialRow + finalRow) / 2) * width + ((initialCol + finalCol) / 2);
}

void freePedine(Colonna *scacchiera, int ROWS, int COLS) {
    int r, c, i;

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            if (scacchiera[atPosition(r, c, COLS)].altezza != 0)
                for (i = 0; i < scacchiera[atPosition(r, c, COLS)].altezza; i++)
                    free(scacchiera[atPosition(r, c, COLS)].pedine[i]);
        }
    }
}

int ShiftA(Pedina **v, int n, int p, int v_size) {
    int i;
    if (n < v_size) {
        for (i = n; i > p; i--)
            v[i] = v[i - 1];
        /*la cella di indice p è libera e può essere usata all'occorrenza*/
        return 1;
    } else
        return 0;
}
