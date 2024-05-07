#ifndef GOOSELIB_ARRAY_H
#define GOOSELIB_ARRAY_H

#include "gooselib_types.h"


gooselib_array_t gooselib_array_create(size_t element_size,
                                       gooselib_malloc_t malloc,
                                       gooselib_free_t free,
                                       void *allocator_context);
int gooselib_array_quit(gooselib_array_t *array);
int gooselib_array_pushElement(gooselib_array_t *array, void *element);
int gooselib_array_pushElements(gooselib_array_t *array, const void *elements, size_t elements_length);
int gooselib_array_popElement(gooselib_array_t *array, void *element);
int gooselib_array_popElements(gooselib_array_t *array, void *elements, size_t count);
int gooselib_array_getTop(gooselib_array_t *array, void *element);
int gooselib_array_setTop(gooselib_array_t *array, void *element);
int gooselib_array_get(gooselib_array_t *array, void *element, ptrdiff_t index);
int gooselib_array_set(gooselib_array_t *array, const void *element, ptrdiff_t index);
void gooselib_array_clear(gooselib_array_t *array);

#endif // GOOSELIB_ARRAY_H
