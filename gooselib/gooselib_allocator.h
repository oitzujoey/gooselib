#ifndef GOOSELIB_ALLOCATOR_H
#define GOOSELIB_ALLOCATOR_H

#include "gooselib_types.h"

gooselib_allocator_t gooselib_allocator_create(gooselib_malloc_t malloc, gooselib_free_t free, void *allocator_context);

void *gooselib_alloc(gooselib_allocator_t *a, size_t size);
#define gooselib_alloc_type(a, length, type) gooselib_alloc(a, (length) * sizeof(type))

void gooselib_free(gooselib_allocator_t *a);
#define gooselib_quit(a) gooselib_free(a)

#endif /* GOOSELIB_ALLOCATOR_H */
