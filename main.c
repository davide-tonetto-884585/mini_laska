#include <stdio.h>
#include"prototipi.h"

int main() {
    /*inizializzo una mossa che poi riempirò con la mossa inserita dall'utente*/
    Mossa mossa;

    /*variabile che indica se la mossa inserita è valida o no*/
    int isMossaValida = 0;

    /*inizializzo la partita*/
    Partita partita;

    /*inizializzo il turno iniziale al BIANCO*/
    partita.turnoCorrente = BIANCO;

    /*inizializzo isEnded a 0 cosi da indicare che la partita non è finita*/
    partita.isEnded = 0;

    /*inizializzo la matrice scacchiera per far partire la partita e controllo che tutto sia andato a buon fine*/
    if(!init_game(&(partita.scacchiera[0][0]), LATO_SCACCHIERA, LATO_SCACCHIERA)) {
        freePedine(&(partita.scacchiera[0][0]), LATO_SCACCHIERA, LATO_SCACCHIERA);
        printf("Errore inizializzazione partita.");
        return 1;
    }

    /*ciclo che continua fino alla fine della partita*/
    while (!partita.isEnded) {
        /*stampo l'attuale situazione della partita*/
        printf("\n");
        draw(&(partita.scacchiera[0][0]), LATO_SCACCHIERA);

        /*risetto la variabile isMossaValida a zero per poter entrare nel ciclo che richiede la mossa*/
        isMossaValida = 0;

        /*finchè l'utente non effettua una mossa valida richiedo le coordinate della mossa*/
        while (!isMossaValida) {
            /*Stampo il colore cui spetta il turno*/
            printf("Turno: %s;\n", partita.turnoCorrente == BIANCO ? "Bianco" : "Nero");

            /*richiedo la posizione della pedina che si desidera muovere*/
            printf("Selezionare la cella della pedina che si vuole muovere:\ncolonna [a-g]: ");
            scanf(" %c", &mossa.posizionePedina.colonna);
            printf("riga [1-7]: ");
            scanf("%d", &mossa.posizionePedina.riga);

            /*richiedo la posizione in cui si vuole muoere la pedina selezionata*/
            printf("Selezionare la cella su cui muovere la pedina:\ncolonna [a-g]: ");
            scanf(" %c", &mossa.posizioneFinale.colonna);
            printf("riga [1-7]: ");
            scanf("%d", &mossa.posizioneFinale.riga);

            /*se la funzione controlloMossa ritorna 1 allora la mossa è valida ed esco dal ciclo altrimenti richiedo nuovamente la mossa*/
            if (controlloMossa(&(partita.scacchiera[0][0]), LATO_SCACCHIERA, LATO_SCACCHIERA, mossa, partita.turnoCorrente)) {
                /*effettuo la mossa*/
                muoviPedina(&(partita.scacchiera[0][0]), LATO_SCACCHIERA, mossa);

                /*indico che la mossa è andata a buon fine*/
                isMossaValida = 1;
            } else
                printf("\nMossa non valida! Inserire nuovamente una mossa valida.\n");
        }

        /*Una volta effettuata la mossa passo al turno successivo*/
        partita.turnoCorrente = switchTurno(partita.turnoCorrente);
    }

    /*Libero la memoria allocata*/
    freePedine(&(partita.scacchiera[0][0]), LATO_SCACCHIERA, LATO_SCACCHIERA);

    /* system("clear"); -per pulire il terminale ogni turno */

    return 0;
}
