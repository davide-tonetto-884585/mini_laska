#include <stdlib.h>
#include "dynamic_vet.h"

bool_t initVet(vettore_dinamico_mossa_t *vet, size_t capacity) {
    vet->mosse = (mossa_t *) malloc(capacity * sizeof(mossa_t));

    if (vet->mosse == NULL)
        return FALSE;

    vet->size = 0;
    vet->capacity = capacity;

    return TRUE;
}

bool_t pushBack(vettore_dinamico_mossa_t *vet, mossa_t mossa) {
    if (vet->size == vet->capacity) {
        /*extend vector*/
        mossa_t *new_data = (mossa_t *) realloc(vet->mosse, vet->capacity * 2 * sizeof(mossa));
        if (!new_data) {
            free(vet->mosse);
            return FALSE;
        }
        vet->mosse = new_data;
        vet->capacity *= 2;
    }

    vet->mosse[vet->size] = mossa;
    vet->size++;

    return TRUE;
}

void freeVet(vettore_dinamico_mossa_t *vet) {
    free(vet->mosse);
    vet->mosse = NULL;
    vet->size = vet->capacity = 0;
}
