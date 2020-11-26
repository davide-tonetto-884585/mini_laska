/*lunghezza del lato della scacchiera*/
#define LATO_SCACCHIERA (7)
#define INPUT_SIZE (20)

typedef int bool;

/*enum che definisce di che colore possono essere le pedine*/
enum colore {
    BIANCO = 'b',
    NERO = 'n'
};

/*struttura che definisce una pedina indicandone il colore e una variabile che indica se la pedina è promossa o meno*/
typedef struct Pedina {
    enum colore colore;
    bool isPromossa;
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

/*Struct per gestione array dinamici di mosse*/
typedef struct VettoreDinamicoMosse {
    /* data */
    Mossa *mosse;
    int size; /*dimensione logica, celle effettivamente utilizzate*/
    int capacity;/*dimensione fisica, la massima dimensione utilizzabile*/
} VettoreDinamicoMosse;

/*struttura che definisce lo stato della partita indicandone il colore del turno corrente, la matrice rappresentante la scacchiera e una variabile che indica se la partita è finita o no*/
typedef struct Partita {
    enum colore turnoCorrente;
    Colonna scacchiera[LATO_SCACCHIERA][LATO_SCACCHIERA];
    bool isEnded;
    VettoreDinamicoMosse mossePartita;
} Partita;

/*funzione inizializzazione vettore dinamico*/
bool initVet(VettoreDinamicoMosse *vet, int capacity);

/*funzione per inserire un elemento in coda all'array dinamico*/
bool pushBack(VettoreDinamicoMosse *vet, Mossa mossa);

/*funzione che libera la memoria allocata per un vettore dinamico*/
void freeVet(VettoreDinamicoMosse *vet);

/*funzione che passata la matrice rappresentante la scacchiera ne inizializza gli elementi a inizio gioco
 * la funzione allocherà le pedine della scacchiera tramite malloc, perciò bisognerà liberare la memoria con la funzione freePedine
 * ritorna 1 (true) se la malloc va a buon fine, altrimenti 0 (false)*/
bool init_game(Colonna *scacchiera, int ROWS, int COLS);

/*funzione che stampa la scacchiera in base alla matrice delle pedine passata per argomento*/
void draw(Colonna *scacchiera, int lato);

/*funzione che controlla se ci sono pedine da promuovere e, in caso, le promuove*/
void controlloPromozione(Colonna *scacchiera, int ROWS, int COLS);

/*funzione che controlla se la partita è finita*/
/*TODO fare controllo vincitore*/
enum colore isWin();

/*funzione che controlla se la mossa indicata dall'utente è valida, ritorna 1 se la mossa è valida, altrimenti 0*/
bool controlloMossa(Colonna *scacchiera, int ROWS, int COLS, Mossa mossa, enum colore turnoCorrente);

/*funzione che restituisce tutte le mosse possibili che può effettuare una pedina, ritorna un vettore dinamico quindi andrà deallocato con la funzione freeVet()*/
VettoreDinamicoMosse
trovaMosseDisponibili(Colonna *scacchiera, int ROWS, int COLS, enum colore turnoCorrente);

/*funzione che muove la pedina indicata dalle coordinate iniziali alle coordinate finali e conquista se la mossa lo richiede*/
/*la funzione non effettua controlli sulla validità della mossa, per fare ciò utilizzare "controlloMossa"*/
void muoviPedina(Colonna *scacchiera, int COLS, Mossa mossa);

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
bool ShiftA(Pedina **v, int n, int p, int v_size);

/*funzione per stampare una mossa*/
void stampaMossa(Mossa mossa);

/*funzione che richiede un parametro in input e controlla che sia di tipo int valido*/
bool inputInt(int *var);
