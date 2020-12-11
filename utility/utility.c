#include "utility.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void switchTurno(enum colore *turnoCorrente) {
    if (*turnoCorrente == BIANCO)
        *turnoCorrente = NERO;
    else
        *turnoCorrente = BIANCO;
}

size_t atPosition(size_t row, size_t col, size_t width) {
    return row * width + col;
}

size_t atItermediatePosition(mossa_t mossaConquista, size_t width) {
    int colonnaPedinaInt = mossaConquista.posizionePedina.colonna - 97;
    int colonnaFinaleInt = mossaConquista.posizioneFinale.colonna - 97;

    return ((mossaConquista.posizionePedina.riga + mossaConquista.posizioneFinale.riga) / 2) * width +
           ((colonnaPedinaInt + colonnaFinaleInt) / 2);
}


bool_t ShiftADX(pedina_t **v, int n, int p, int v_size) {
    int i;
    if (n < v_size) {
        for (i = n; i > p; i--)
            v[i] = v[i - 1];
        /*la cella di indice p ? libera e pu? essere usata all'occorrenza*/
        return TRUE;
    } else
        return FALSE;
}

void stampaMossa(mossa_t mossa) {
    printf("%c%d - %c%d", mossa.posizionePedina.colonna, mossa.posizionePedina.riga, mossa.posizioneFinale.colonna, mossa.posizioneFinale.riga);
}

bool_t equalsMossa(mossa_t m1, mossa_t m2) {
    return m1.posizionePedina.riga == m2.posizionePedina.riga && m1.posizionePedina.colonna == m2.posizionePedina.colonna &&
           m1.posizioneFinale.riga == m2.posizioneFinale.riga && m1.posizioneFinale.colonna == m2.posizioneFinale.colonna;
}

bool_t inputInt(int *var) {
    char *ptr;
    char str[INPUT_SIZE];

    fgets(str, INPUT_SIZE, stdin);
    *var = (int) strtol(str, &ptr, 10);

    if (*var == INT_MAX || *var == INT_MIN)
        return FALSE;
    else if (*var)
        return TRUE;

    return FALSE;
}

