
#include "gooselib_allocator.h"
#include <stdlib.h>
#include <string.h>


gooselib_allocator_t gooselib_allocator_create(void *(*malloc)(void *, size_t),
                                               void (*free)(void *, void *),
                                               void *allocator_context) {
	gooselib_allocator_t a;
	a.allocations = NULL;
	a.length = 0;
	a.malloc = malloc;
	a.free = free;
	a.allocator_context = allocator_context;
	return a;
}

void *gooselib_alloc(gooselib_allocator_t *a, size_t size) {
	/* Allocate. */
	void *memory = a->malloc(a->allocator_context, size);
	if (memory == NULL) {
		return NULL;
	}
	/* Log this allocation. */
	{
		void **allocations = a->malloc(a->allocator_context, (1 + a->length) * sizeof(void *));
		if (allocations == NULL) {
			/* Check if `free` exists because we might be using a tracing garbage collector. */
			if (a->free != NULL) {
				(void) a->free(a->allocator_context, memory);  /* Because `memory`'s malloc was successful. */
			}
			return NULL;
		}
		(void) memcpy(allocations, a->allocations, a->length);
		allocations[a->length] = memory;
		/* Check if `free` exists because we might be using a tracing garbage collector. */
		if (a->free != NULL) {
			(void) a->free(a->allocator_context, a->allocations);
		}
		a->allocations = allocations;
	}
	return memory;
}

void gooselib_free(gooselib_allocator_t *a) {
	/* Check if `free` exists because we might be using a tracing garbage collector. */
	if (a->free != NULL) {
		/* Free public allocations. */
		for (size_t index = 0; index < a->length; index++) {
			(void) a->free(a->allocator_context, a->allocations[index]);
		}
		/* Free allocation log. */
		(void) a->free(a->allocator_context, a->allocations);
	}
	a->allocations = NULL;
	a->length = 0;
}
