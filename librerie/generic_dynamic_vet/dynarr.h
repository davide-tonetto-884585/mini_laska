/** @file dynarr.h
 *  @brief libreria che implementa array dinamici generici tramite utilizzo del macroprocessore
 */

#ifndef DYNARR
#define DYNARR

#include <stdlib.h>
#include <string.h>

#define DYN_ARR_DEFAULT_CAPACITY (2)

/**
 * @brief enum che implementa il tipo bool
 *
 * tramite questo enum si vuole implementare il tipo bool che assegna 0 a FALSE e 1 a TRUE
 */
enum bool {FALSE, TRUE};
typedef enum bool bool_t;

/** struttura contenente le informazioni dell'array dinamico:
 *
 * size - numero di elementi effettivamente contenuti
 * elem_size - grandezza in bite del tipo degli elementi dell'array dinamico
 * capacity - numero di elementi massimi contenuti nell'array
 * */
struct dyn_arr_info {
    size_t size;
    size_t elem_size;
    size_t capacity;
};
typedef struct dyn_arr_info dyn_arr_info_t;

/** inizialzza le informazioni dell'array dinamico e ne alloca lo spazio
 *
 * @param info - puntatore allo struct info dell'array
 * @param elem_size - grandezza in bite di un elemento dell'array dinamico
 * @param capacity - numero di elementi da allocare inizialmente
 * @return void* - void pointer al primo elemento della memoria allocata tramite la malloc
 * */
void *_dyn_arr_init(dyn_arr_info_t *info, size_t elem_size, size_t capacity);

/** inserisce un elemento in coda all'array dinamico (se serve alloca ulteriore spazio)
 *
 * @param data - void pointer al primo elemento dell'array
 * @param info - informazioni dell'array dinamico
 * @param elem - void pointer all'elemento da aggiungere
 * @return void
 * */
void *_dyn_arr_push(void *data, dyn_arr_info_t *info, void *elem);

/** rimuove l'ultimo elemento dell'array dinamico (se serve alloca ulteriore spazio)
 *
 * @param data - void pointer al primo elemento dell'array
 * @param info - informazioni dell'array dinamico
 * @param futureRemovedValue - void pointer alla variabile passata come argomento su cui verrà copiato l'elemento rimosso
 * @return bool_t - false se l'array non ha elementi altrtimenti true
 * */
bool_t _dyn_arr_pop(void *data, dyn_arr_info_t *info, void *futureRemovedValue);

/** dealloca la memoria dell'array dinamico e ne setta le info ai valori di default
 *
 * @param data - void pointer al primo elemento dell'array
 * @param info - informazioni dell'array dinamic
 * @return void
 * */
void _dyn_arr_destroy(void *data, dyn_arr_info_t *info);

/** controlla che la posizione passata sia valida e ne copia l'elemento in quella posizione sul parametro element
 *
 * @param data - void pointer al primo elemento dell'array
 * @param info - informazioni dell'array dinamic
 * @param positiom - posizione che si vuole ottenre
 * @param element - puntatore alla variabile su cui verrà copiato l'elemento
 * @return bool_t - FALSE se la posizione non esiste non esiste
 * */
bool_t _dyn_arr_get_elem_secure(void *data, dyn_arr_info_t *info, size_t position, void *element);

/** rimuove l'elemento nella posizione passata
 *
 * @param data - void pointer al primo elemento dell'array
 * @param info - informazioni dell'array dinamic
 * @param position - posizione in cui rimuovere l'elemento
 * @return bool_t - true se l'elemento è stato rimosso
 * */
bool_t _dyn_arr_remove_elem(void *data, dyn_arr_info_t *info, size_t position);

/** ritorna true se trova almeno una ricorrenza di element nell'array
 *
 * @param data - void pointer al primo elemento dell'array
 * @param size - grandezza dell'arrayy dinamic
 * @param element - elemento da ricercare
 * @param equals - puntatore a funzione definita dall'utente che ritorna true se due elementi dell'array sono uguali
 * @return bool_t - true se l'elemento è stato trovato
 * */
bool_t _dyn_arr_contains_elem(void *data, size_t dataSize, dyn_arr_info_t *info, void *element,
                            bool_t (*equals)(const void *, const void *));

/** ritorna la posizione della prima ricorrenza di element nell'array oppure -1 se non lo trova
 *
 * @param data - void pointer al primo elemento dell'array
 * @param dataSize - grandezza dell'array dinamic
 * @param cont - contatore per ricorsione (passare 0)
 * @param element - elemento da ricercare
 * @param equals - puntatore a funzione definita dall'utente che ritorna true se due elementi dell'array sono uguali
 * @return int - posizione della prima ricorrenza dell'elemento o -1 se non lo trova
 * */
int _dyn_arr_index_of_elem(void *data, size_t dataSize, int cont, dyn_arr_info_t *info, void *element,
                              bool_t (*equals)(const void *, const void *));

/** definisce uno struct contenente le informazioni dell'array dinamico e l'array stesso del tipo passato
 *
 * @param type - definisce il tipo degli elementi dell'array dinamico
 * @return void
 * */
#define DYN_ARR_CREATE(type) typedef struct dyn_arr_##type { dyn_arr_info_t info; type *data; } dyn_arr_##type

/** inizialzza l'array dinamico passato per argomento con una capacity di default
 *
 * @param array - array dinamico da inizializzare
 * @return void
 * */
#define DYN_ARR_INIT_DEFAULT(array) array.data = _dyn_arr_init(&array.info, sizeof(*array.data), DYN_ARR_DEFAULT_CAPACITY)

/** inizialzza l'array dinamico passato per argomento ne setta la capacità
 *
 * @param array - array dinamico da inizializzare
 * @param capacity - numero di elementi da allocare inizialmente per l'array dinamico
 * @return void
 * */
#define DYN_ARR_INIT(array, capacity) array.data = _dyn_arr_init(&array.info, sizeof(*array.data), capacity)

/** permette di inserire un elemento in coda all'array dinamico
 *
 * @param array - array su cui aggiungere l'elemento
 * @param val - elemento da inserire (deve essere del tipo definito alla creazione dell'array dinamico)
 * @return void
 * */
#define DYN_ARR_PUSH(array, val) array.data = _dyn_arr_push(array.data, &array.info, &val)

/* In GNU C, but not GNU C++, you may also declare the type of a variable as __auto_type. In that case,
 * the declaration must declare only one variable, whose declarator must just be an identifier,
 * the declaration must be initialized, and the type of the variable is determined by the initializer;
 * the name of the variable is not in scope until
 *
 *  #define DYN_ARR_PUSH(array, val) do{ \
 *      __auto_type temp = val; \
 *      _dyn_arr_push(array.data, &array.info, &temp) \
 *  }while(0)
 *
 * su DYN_ARR_PUSH per permettere all'utente di passare equazioni e non solo variabili
 * */

/** rimuove l'ultimo elemento dell'array dinamico e lo copia nella variabile passata come parametro
 *
 * @param array - array da cui rimuovere l'elemento
 * @param futureRemovedValue - variabile su cui verrà copiato l'elemento rimosso
 * @return bool_t - false se l'array non ha elementi da rimuovere
 * */
#define DYN_ARR_POP(array, futureRemovedValue) _dyn_arr_pop(array.data, &array.info, &futureRemovedValue)

/** dealloca la memoria allocata per l'array dinamico e ne reimposta le info di default (l'array dinamico potrà essere reinizializzato)
 *
 * @param array - array da deallocare
 * @return void
 * */
#define DYN_ARR_DESTROY(array) _dyn_arr_destroy(array.data, &array.info)

/** restituisce il numero di elementi contenuti nell'array
 *
 * @param array - array dinamico di cui si vuole sapere il numero di elementi
 * @return size_t - numero di elementi contenuti nell'array dinamico
 * */
#define DYN_ARR_GET_SIZE(array) array.info.size

/** restituisce la capacità attuale dell'array
 *
 * @param array - array dinamico di cui si vuole sapere la capacità
 * @return size_t - capacità dell'array dinamico
 * */
#define DYN_ARR_GET_CAPACITY(array) array.info.capacity

/** copia l'elemento nella posizione passata nella variabile element
 *
 * @param array - array da cui si vuole reperire l'elemento
 * @param position - posizione dell'elemento da ottenere
 * @param element - posizione dell'elemento da ottenere
 * @return bool_t - true se ha reperito l'elemento, false se la posizione non è valida
 * */
#define DYN_ARR_GET_ELEM_SECURE(array, position, element) _dyn_arr_get_elem_secure(array.data, &array.info, position, &element)

/** restituisce l'elemento nella posizione passata (ATTENZIONE: non controlla che la posizione sia valida)
 *
 * @param array - array da cui si vuole reperire l'elemento
 * @param position - posizione dell'elemento da ottenere
 * @return elemento del tipo dell'array dinamico
 * */
#define DYN_ARR_GET_ELEM(array, position) array.data[position]

/** elimina l'elemento nella posizione indicata
 *
 * @param array - array da cui si vuole rimuovere l'elemento
 * @param position - posizione dell'elemento da rimuovere
 * @return bool_t - true se l'elemento è stato rimosso correttamente
 * */
#define DYN_ARR_REMOVE_ELEM(array, position) _dyn_arr_remove_elem(array.data, &array.info, position)

/** ritorna true se trova almeno una ricorrenza di element nell'array
 *
 * @param array - array su cui cercare l'elemento
 * @param element - elemento da ricercare
 * @param pointerToEqualsFunction - puntatore a funzione definita dall'utente che ritorna true se due elementi dell'array sono uguali
 * @return bool_t - true se l'elemento è stato trovato
 * */
#define DYN_ARR_CONTAINS_ELEM(array, element, pointerToEqualsFunction) _dyn_arr_contains_elem(array.data, array.info.size, &array.info, &element, pointerToEqualsFunction)

/** ritorna la posizione della prima ricorrenza di element nell'array o -1 se non trova nulla
 *
 * @param array - array su cui cercare l'elemento
 * @param element - elemento da ricercare
 * @param pointerToEqualsFunction - puntatore a funzione definita dall'utente che ritorna true se due elementi dell'array sono uguali
 * @return int - posizione della prima ricorrenza di element nell'array o -1 se non trova nulla
 * */
#define DYN_ARR_INDEX_OF_ELEM(array, element, pointerToEqualsFunction) _dyn_arr_index_of_elem(array.data, array.info.size, 0, &array.info, &element, pointerToEqualsFunction)

/** elimina la prima ricorrenza di element dall'array
 *
 * @param array - array su cui cercare l'elemento
 * @param element - elemento da ricercare
 * @param pointerToEqualsFunction - puntatore a funzione definita dall'utente che ritorna true se due elementi dell'array sono uguali
 * @return bool_t - true se l'elemento è stato rimosso correttamente
 * */
#define DYN_aRR_REMOVE_SPECIFIC_ELEM(array, element, pointerToEqualsFunction) DYN_ARR_REMOVE_ELEM(array, \
_dyn_arr_index_of_elem(array.data, array.info.size, 0, &array.info, &element, pointerToEqualsFunction))

#endif /* DYNARR */
