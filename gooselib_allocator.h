#ifndef GOOSELIB_ALLOCATOR_H
#define GOOSELIB_ALLOCATOR_H

#include <stddef.h>

typedef struct {
	void **allocations;
	/* Easy optimization: Double memory each time instead of incrementing. */
	size_t length;
	void *(*malloc)(void *, size_t);
	void (*free)(void *, void *);
	void *allocator_context;
} gooselib_allocator_t;

void gooselib_allocator_init(gooselib_allocator_t *a,
                             void *(*malloc)(void *, size_t),
                             void (*free)(void *, void *),
                             void *allocator_context);

void *gooselib_alloc(gooselib_allocator_t *a, size_t size);
#define gooselib_alloc_type(a, length, type) gooselib_alloc(a, (length) * sizeof(type))

void gooselib_free(gooselib_allocator_t *a);
#define gooselib_quit(a) gooselib_free(a)

#endif /* GOOSELIB_ALLOCATOR_H */
