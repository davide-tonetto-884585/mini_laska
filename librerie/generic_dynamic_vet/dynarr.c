#include "dynarr.h"

void *_dyn_arr_init(dyn_arr_info_t *info, size_t elem_size, size_t capacity) {
    void *temp;

    info->size = 0;
    info->elem_size = elem_size;
    info->capacity = capacity;

    temp = malloc(info->capacity * info->elem_size);
    if (temp == NULL)
        exit(EXIT_FAILURE);

    return temp;
}

void *_dyn_arr_push(void *data, dyn_arr_info_t *info, void *elem) {
    void *temp = data;
    if (info->size == info->capacity) {
        temp = realloc(data, info->capacity * 2 * info->elem_size);
        if (temp == NULL)
            exit(EXIT_FAILURE);

        info->capacity *= 2;
    }

    /* L'aritmetica per i void pointer non è permessa in c (a meno che non si usi un'estensione di gnu gcc) quindi casto a char*
     * che ha la stessa rappresentazione e allineamento del void* */
    memcpy((char *) temp + info->size * info->elem_size, elem, info->elem_size);
    info->size++;

    return temp;
}

bool_t _dyn_arr_pop(void *data, dyn_arr_info_t *info, void *futureRemovedValue) {
    if (info->size == 0) {
        return FALSE;
    }

    /* L'aritmetica per i void pointer non è permessa in c (si se si usa un'estensione di gnu gcc) quindi casto a char*
     * che ha la stessa rappresentazione e allineamento del void* */
    memcpy(futureRemovedValue, (char *) data + (info->size - 1) * info->elem_size, info->elem_size);
    info->size--;
    return TRUE;
}

void _dyn_arr_destroy(void *data, dyn_arr_info_t *info) {
    info->size = info->elem_size = info->capacity = 0;
    free(data);
}

bool_t _dyn_arr_get_elem_secure(void *data, dyn_arr_info_t *info, size_t position, void *element) {
    if (position >= info->size)
        return FALSE;

    memcpy(element, (char *) data + position * info->elem_size, info->elem_size);
    return TRUE;
}

bool_t _dyn_arr_remove_elem(void *data, dyn_arr_info_t *info, size_t position) {
    if (position >= info->size)
        return FALSE;

    if (position != info->size - 1)
        memcpy((char *) data + position * info->elem_size,
               (char *) data + (position + 1) * info->elem_size,
               (info->size - position - 1) * info->elem_size);

    info->size--;
    return TRUE;
}

bool_t _dyn_arr_contains_elem(void *data, size_t dataSize, dyn_arr_info_t *info, void *element,
                              bool_t (*equals)(const void *, const void *)) {
    if (dataSize == 0)
        return FALSE;
    else if (equals(data, element))
        return TRUE;
    else
        return _dyn_arr_contains_elem((char *) data + info->elem_size, dataSize - 1, info, element, equals);
}

int _dyn_arr_index_of_elem(void *data, size_t dataSize, int cont, dyn_arr_info_t *info, void *element,
                           bool_t (*equals)(const void *, const void *)) {
    if (dataSize == 0)
        return -1;
    else if (equals(data, element))
        return cont;
    else
        return _dyn_arr_index_of_elem((char *) data + info->elem_size, dataSize - 1, cont + 1, info, element, equals);
}
