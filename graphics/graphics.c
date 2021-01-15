#include "graphics.h"
#include "../utility/utility.h"
#include <stdio.h>
#include <stdlib.h>

/* librerie usate per la grafica su windows */
/* #if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <conio.h>
#endif */

#define C_RED "\x1b[31m"
#define C_YELLOW "\x1b[33m"
#define C_RESET "\x1b[0m"

void stampaPedina(const cella_t *scacchiera, size_t rig, size_t col, size_t COLS, size_t altezza) {
    char square = (char) 219u;

    if (scacchiera[atPosition(rig, col, COLS)].pedine[altezza] != NULL) {
        printf("    ");
        if (scacchiera[atPosition(rig, col, COLS)].pedine[altezza]->isPromossa) {
            if (scacchiera[atPosition(rig, col, COLS)].pedine[altezza]->colore == BIANCO)
                printf(C_YELLOW "%c" C_RESET "%c" C_YELLOW "%c" C_RESET, square, square, square);
            else
                printf(C_RED "%c" C_RESET "%c" C_RED "%c" C_RESET, square, square, square);
        } else {
            if (scacchiera[atPosition(rig, col, COLS)].pedine[altezza]->colore == BIANCO)
                printf(C_YELLOW "%c%c%c" C_RESET, square, square, square);
            else
                printf(C_RED "%c%c%c" C_RESET, square, square, square);
        }
        printf("   ");
    } else
        printf("          ");
}

void draw(const cella_t *scacchiera, size_t lato, const mossa_t *mossa) {
    /*variabili utilizzate per ciclare i vari for*/
    int rig, col, i, k, t, colonnaPedinaInt, colonnaFinaleInt, mossaPedinaInt, mossaFinaleInt;
    char pedina;

    /* variabili usate per contenere gli elementi di disegno */
    char square = (char) 219u, v_line = (char) 179u, o_line = (char) 196u, top_sx = (char) 218u, top_dx = (char) 191u,
            low_sx = (char) 192u, low_dx = (char) 217u, little_square = (char) 254u;

    if (mossa != NULL) {
        /*converto le coodrdinate alfabetiche in intere*/
        colonnaPedinaInt = mossa->posizionePedina.colonna - 97;
        colonnaFinaleInt = mossa->posizioneFinale.colonna - 97;

        /*inverto la coordinata della riga per poter accedere correttamente alla matrice*/
        mossaPedinaInt = abs(mossa->posizionePedina.riga - (int) lato);
        mossaFinaleInt = abs(mossa->posizioneFinale.riga - (int) lato);
    }

    /*stampo il contorno superiore*/
    printf("%c", top_sx);

    for (col = 0; col < lato; col++)
        printf("%c%c%c%c%c%c%c%c%c%c", o_line, o_line, o_line, o_line, o_line, o_line, o_line, o_line, o_line, o_line);

    printf("%c\n", top_dx);

    /*scorro le righe della scacchiera da 0 a lato - 1*/
    for (rig = 0; rig < lato + 1; rig++) {

        /*controllo di non essere arrivato all'ultima riga*/
        if (rig != lato)

            /*ciclo altre tre volte per definire l'altezza di ogni riga*/
            for (i = 0; i < 5; i++) {
                printf("%c", v_line);

                /*ciclo nuovamente le colonne per colorarle e disegnare le pedine*/
                for (k = 0; k < lato; k++) {

                    /*controllo che permette di colorare la cella di nero o bianco*/
                    if ((rig + k) % 2 != 0)
                        printf("%c%c%c%c%c%c%c%c%c%c", square, square, square, square, square, square, square, square, square, square);
                    else {
                        /*se la pedina nella casella in cui mi trovo ha altezza 0 significa che ? nulla e non la rappresento, altrimenti si*/
                        switch (i) {
                            case 0:
                                if (mossa != NULL) {
                                    if ((k == colonnaPedinaInt && mossaPedinaInt == rig) || (k == colonnaFinaleInt && mossaFinaleInt == rig))
                                        printf("        %s ", "x");
                                    else
                                        printf("          ");
                                } else
                                    printf("          ");

                                break;
                            case 1:
                                stampaPedina(scacchiera, rig, k, lato, 2);

                                break;
                            case 2:
                                stampaPedina(scacchiera, rig, k, lato, 1);

                                break;
                            case 3:
                                stampaPedina(scacchiera, rig, k, lato, 0);

                                break;
                            case 4:
                                printf(" %c%d       ", k + 97, abs(rig - (int) lato));

                                break;
                            default:
                                printf("          ");
                        }
                    }

                    /*controllo se mi trovo a fine ciclo definisco il contorno della scacchiera*/
                    if (k == lato - 1)
                        printf("%c\n", v_line);
                }
            }
    }

    /*stampo il contorno inferiore*/
    printf("%c", low_sx);

    for (col = 0; col < lato; col++)
        printf("%c%c%c%c%c%c%c%c%c%c", o_line, o_line, o_line, o_line, o_line, o_line, o_line, o_line, o_line, o_line);

    printf("%c\n", low_dx);
}

void multiPlatformDraw(const cella_t *scacchiera, size_t lato, const mossa_t *mossa) {
    /*variabili utilizzate per ciclare i vari for*/
    int rig, col, i, k, t, colonnaPedinaInt, colonnaFinaleInt, mossaPedinaInt, mossaFinaleInt;
    char pedina;

    if (mossa != NULL) {
        /*converto le coodrdinate alfabetiche in intere*/
        colonnaPedinaInt = mossa->posizionePedina.colonna - 97;
        colonnaFinaleInt = mossa->posizioneFinale.colonna - 97;

        /*inverto la coordinata della riga per poter accedere correttamente alla matrice*/
        mossaPedinaInt = abs(mossa->posizionePedina.riga - (int) lato);
        mossaFinaleInt = abs(mossa->posizioneFinale.riga - (int) lato);
    }

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
                        if (i == 1) {
                            /*se la pedina nella casella in cui mi trovo ha altezza -1 significa che ? nulla e non la rappresento, altrimenti si*/
                            if (scacchiera[atPosition(rig, k, lato)].altezza == 0)
                                printf("          ");
                            else {
                                printf("    ");

                                /*Scorro la torre*/
                                for (t = 0; t < 3; t++) {
                                    if (scacchiera[atPosition(rig, k, lato)].pedine[t] != NULL) {
                                        if (scacchiera[atPosition(rig, k, lato)].pedine[t]->isPromossa) {
                                            if (scacchiera[atPosition(rig, k, lato)].pedine[t]->colore == BIANCO)
                                                pedina = 'B';
                                            else
                                                pedina = 'N';
                                        } else
                                            pedina = scacchiera[atPosition(rig, k, lato)].pedine[t]->colore;

                                        printf("%c", pedina);
                                    } else
                                        printf(" ");
                                }

                                printf("   ");
                            }
                        } else if (i == 2) /*stampo le coordinate della casella attuale*/
                            printf(" %c%d       ", k + 97, abs(rig - (int) lato));
                        else {
                            /* evidenzio la mossa se passata */
                            if (mossa != NULL) {
                                if ((k == colonnaPedinaInt && mossaPedinaInt == rig) || (k == colonnaFinaleInt && mossaFinaleInt == rig))
                                    printf("        %s ", "x");
                                else
                                    printf("          ");
                            } else
                                printf("          ");
                        }
                    }

                    /*controllo se mi trovo a fine ciclo definisco il contorno della scacchiera*/
                    if (k == lato - 1)
                        printf("|\n");
                }
            }
    }
}

/* stampa il titolo mini lasca */
void titolo() {
    printf("\n"
           "  __  __ _       _   _                         \n"
           " |  \\/  (_)     (_) | |                        \n"
           " | \\  / |_ _ __  _  | |     __ _ ___  ___ __ _ \n"
           " | |\\/| | | '_ \\| | | |    / _` / __|/ __/ _` |\n"
           " | |  | | | | | | | | |___| (_| \\__ \\ (_| (_| |\n"
           " |_|  |_|_|_| |_|_| |______\\__,_|___/\\___\\__,_|\n"
           "                                               \n"
           "                                               \n");
}


/****************************grafica windows commentata*******************************/
/* #ifdef _WIN32
 * funzione che gestisce la selezione dell'opzione *
void arrow_pos(int real_pos, int arrow_pos){
    if (real_pos == arrow_pos)
        printf("    > ");
    else
        printf("      ");
}

 * menu interattivo che setta la modalità di gioco *
void menu_mod(bool_t *modVsCPU){
    int pos = 1, key = 0;

    fflush(stdin);
    system("cls");

    while (key != 13){

        system("cls");
        titolo();
        printf("Seleziona la modalita' di gioco:\n\n");
        arrow_pos(1, pos);
        printf("Player vs Computer\n");
        arrow_pos(2, pos);
        printf("Player vs Player\n");

        key = getch();
        if (key == 80 && pos != 2)
            pos++;
        else if (key == 72 && pos != 1)
            pos--;
    }
    if (pos == 1)
        *modVsCPU = TRUE;
    else if (pos == 2)
        *modVsCPU = FALSE;
    else
        printf("ERRORE DI SELEZIONE");
}

* menu interattivo che setta la difficolta di gioco *
void menu_diff(int *difficolta){
    int pos = 1, key = 0;

    fflush(stdin);
    system("cls");

    while (key != 13) {

        system("cls");
        titolo();

        printf("Seleziona contro chi vuoi giocare:\n\n");
        arrow_pos(1, pos);
        printf("Wall-E\n");
        arrow_pos(2, pos);
        printf("VIKI\n");
        arrow_pos(3, pos);
        printf("HAL 9000\n");

        key = getch();
        if (key == 80 && pos != 3)
            pos++;
        else if (key == 72 && pos != 1)
            pos--;
    }
    if (pos == 1)
        *difficolta = 1;
    else if (pos == 2)
        *difficolta = 2;
    else if (pos == 3)
        *difficolta = 3;
}

* menu che setta il nome dei giocatori *
void menu_nome(bool_t *modVsCPU, partita_t *partita, char *player1, char *player2, int *difficolta){
    if (modVsCPU){
        printf("\nGiocatore, inserisci il tuo nome utente: ");
        scanf("%s", player1);
        getchar();

        partita->player1.nome=player1;
        if (*difficolta == 1)
            partita->player2.nome="Wall-E";
        else if (*difficolta == 2)
            partita->player2.nome="VIKI";
        else if (*difficolta == 3)
            partita->player2.nome="HAL 9000";
        else
            printf("ERRORE IN menu_nome!!");
    }
    else{
        printf("\nGiocatore uno, inserisci il tuo nome utente: ");
        scanf("%s", player1);
        getchar();

        partita->player1.nome = player1;

        printf("\nGiocatore due, inserisci il tuo nome utente: ");
        scanf("%s", player2);
        getchar();

        partita->player2.nome = player2;
    }
}
#endif */
