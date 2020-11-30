#include <stdlib.h>
#include "dynamic_vet.h"

bool initVet(VettoreDinamicoMosse *vet, size_t capacity) {
    vet->mosse = (Mossa *) malloc(capacity * sizeof(Mossa));

    if (vet->mosse == NULL)
        return 0;

    vet->size = 0;
    vet->capacity = capacity;

    return 1;
}

bool pushBack(VettoreDinamicoMosse *vet, Mossa mossa) {
    if (vet->size == vet->capacity) {
        /*extend vector*/
        Mossa *new_data = (Mossa *) realloc(vet->mosse, vet->capacity * 2 * sizeof(Mossa));
        if (!new_data) {
            free(vet->mosse);
            return 0;
        }
        vet->mosse = new_data;
        vet->capacity *= 2;
    }

    vet->mosse[vet->size] = mossa;
    vet->size++;

    return 1;
}

void freeVet(VettoreDinamicoMosse *vet) {
    free(vet->mosse);
    vet->mosse = NULL;
    vet->size = vet->capacity = 0;
}
