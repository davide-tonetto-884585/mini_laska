/*lunghezza del lato della scacchiera*/
#define LATO_SCACCHIERA (7) 

/*enum che definisce di che colore possono essere le pedine*/
enum colore {
    BIANCO = 'B',
    NERO = 'N'
};

/*struttura che definisce una pedina indicandone il colore e una variabile che indica se la pedina è promossa o meno*/
typedef struct Pedina {
    enum colore colore;
    int isPromossa;
} Pedina;

/*struttura che definisce una colonna costituita da massimo tre pedine, se l'altezza della colonna è zero la cella della scacchiera sarà vuota, se è uno questa sarà
 * costituita da un'unica pedina*/
typedef struct Colonna {
    Pedina *pedine[3];
    int altezza;
} Colonna;

/*struttura che definisce una posizione sulla scacchiera*/
typedef struct Posizione {
    int riga;
    char colonna;
} Posizione;

/*struttura che definisce le coordinate di una mossa*/
typedef struct Mossa {
    Posizione posizionePedina;
    Posizione posizioneFinale;
} Mossa;

/*struttura che definisce lo stato della partita indicandone il colore del turno corrente, la matrice rappresentante la scacchiera e una variabile che indica se la partita è finita o no*/
typedef struct Partita {
    enum colore turnoCorrente;
    Colonna scacchiera[LATO_SCACCHIERA][LATO_SCACCHIERA];
    int isEnded;
} Partita;

/*funzione che passata la matrice rappresentante la scacchiera ne inizializza gli elementi a inizio gioco
 * la funzione allocherà le pedine della scacchiera tramite malloc, perciò bisognerà liberare la memoria con la funzione freePedine
 * ritorna 1 (true) se la malloc va a buon fine, altrimenti 0 (false)*/
int init_game(Colonna *scacchiera, int ROWS, int COLS);

/*funzione che stampa la scacchiera in base alla matrice delle pedine passata per argomento*/
void draw(Colonna *scacchiera, int lato);

/*funzione che controlla se la partita è finita*/
/*TODO fare controllo vincitore*/
enum colore isWin();

/*funzione che controlla se la mossa indicata dall'utente è valida, ritorna 1 se la mossa è valida, altrimenti 0*/
int controlloMossa(Colonna *scacchiera, int ROWS, int COLS, Mossa mossa, enum colore turnoCorrente);

/*funzione che muove la pedina indicata dalle coordinate iniziali alle coordinate finali e conquista se la mossa lo richiede*/
/*la funzione non effettua controlli sulla validità della mossa, per fare ciò utilizzare "controlloMossa"*/
void muoviPedina(Colonna *scacchiera, int ROWS, Mossa mossa);

/*funzione che passato il turno attuale lo cambia*/
enum colore switchTurno(enum colore turnoCorrente);

/*funzione che restitruisce la posizione dell'elemento di una matrice creata come array monodimensionale*/
int atPosition(int row, int col, int width);

/*funzione che trova la posizione intermedia tra due celle (posizione della pedina da conquistare*/
int atItermediatePosition(int initialRow, int finalRow, int initialCol, int finalCol, int width);

/*funzione che si occupa di liberare la memoria allocata tramite malloc per le pedine della scacchiera ancora in gioco a fine partita*/
void freePedine(Colonna *scacchiera, int ROWS, int COLS);

/*funzione che shifta gli eleenti di un array*/
/*p: posizione da liberare; n: dimensione logica (celle effettivamente occupate)
v_size: dimensione fisica del vettore*/
int ShiftA(Pedina **v, int n, int p, int v_size);
