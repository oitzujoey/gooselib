
#include "gooselib_allocator.h"
#include <stdlib.h>
#include <string.h>


void gooselib_allocator_init(gooselib_allocator_t *a,
                             void *(*malloc)(size_t),
                             void (*free)(void *),
                             void *allocator_context) {
	a->allocations = NULL;
	a->length = 0;
	a->malloc = malloc;
	a->free = free;
	a->allocator_context = allocator_context;
}

void *gooselib_alloc(gooselib_allocator_t *a, size_t size) {
	/* Allocate. */
	void *memory = a->malloc(size);
	if (memory == NULL) {
		return NULL;
	}
	/* Bookkeeping. */
	{
		void **allocations = a->malloc((1 + a->length) * sizeof(void *));
		if (allocations == NULL) {
			(void) a->free(memory);  /* Because `memory`'s malloc was successful. */
			return NULL;
		}
		(void) memcpy(allocations, a->allocations, a->length);
		allocations[a->length] = memory;
		(void) a->free(a->allocations);
		a->allocations = allocations;
	}
	return memory;
}

void gooselib_free(gooselib_allocator_t *a) {
	for (size_t index = 0; index < a->length; index++) {
		(void) a->free(a->allocations[index]);
	}
	(void) a->free(a->allocations);
	a->allocations = NULL;
	a->length = 0;
}
