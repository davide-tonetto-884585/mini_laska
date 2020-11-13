#include <stdio.h>
#include"prototipi.h"

int main() {
    /*variabile per ciclo for*/
    int i;

    /*variabile in cui salvo la mossa che l'utente vuole effettuare*/
    int numeroMossa;

    /*variabile che indica se la mossa inserita è valida o no*/
    int isMossaValida = 0;

    /*vettore dinamico mosse*/
    VettoreDinamicoMosse mosseDisponibili;

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

            /*ottengo le mosse disponibili*/
            mosseDisponibili = trovaMosseDisponibili(&(partita.scacchiera[0][0]), LATO_SCACCHIERA, LATO_SCACCHIERA, partita.turnoCorrente);

            for (i = 0; i < mosseDisponibili.size; ++i) {
                printf("%d) ", i + 1);
                stampaMossa(mosseDisponibili.mosse[i]);
                printf(" ");
            }
            printf("\nInserisci il numero corrispondente alla mossa che vuoi effettuare: ");
            scanf("%d", &numeroMossa);

            /*se la funzione controlloMossa ritorna 1 allora la mossa è valida ed esco dal ciclo altrimenti richiedo nuovamente la mossa*/
            if (numeroMossa <= mosseDisponibili.size && numeroMossa >= 1) {
                /*effettuo la mossa*/
                muoviPedina(&(partita.scacchiera[0][0]), LATO_SCACCHIERA, mosseDisponibili.mosse[numeroMossa - 1]);

                /*libero la memoria allocata per l'array mosseDisponibili*/
                freeVet(mosseDisponibili);

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
