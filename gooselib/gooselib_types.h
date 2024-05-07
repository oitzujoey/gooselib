#ifndef GOOSELIB_TYPES_H
#define GOOSELIB_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


/* Simplified errors from DuckLib. Don't return a simple ambiguous error code like this unless you have no choice. */
#define GOOSELIB_OK 0
#define GOOSELIB_ERROR 1
#define GOOSELIB_NULL 2
#define GOOSELIB_ALLOCFAILED 3
#define GOOSELIB_CANTHAPPEN 4


/* allocator */

typedef void *(*gooselib_malloc_t)(void *, size_t);
typedef void (*gooselib_free_t)(void *, void *);

typedef struct {
	void **allocations;
	/* Easy optimization: Double memory each time instead of incrementing. */
	size_t length;
	gooselib_malloc_t malloc;
	gooselib_free_t free;
	void *allocator_context;
} gooselib_allocator_t;


/* string */

/* Dumb string. */
typedef struct {
	size_t length;
	uint8_t *bytes;
} gooselib_string_t;

/* String that manages memory for you. These strings are immutable. `bytes` is null-terminated. `free` is never
   called. You must use some sort of garbage collection such as the gooselib arena allocator. Strings are only valid
   when `invalid` isn't set. Avoid operating on multiple strings that don't all share the same allocator. If you need to
   do that, then use `gooselib_dstring_copyReallocate` to create a copy that uses the same allocator as all the other
   strings. */
typedef struct {
	size_t length;
	const uint8_t *bytes;
	void *(*malloc)(void *, size_t);
	void *allocator_context;
	bool invalid;
} gooselib_dstring_t;


/* array */

typedef struct {
	void *elements;
	size_t element_size;
	size_t elements_length;
	size_t elements_memorySize;
	gooselib_malloc_t malloc;
	gooselib_free_t free;
	void *allocator_context;
} gooselib_array_t;


#endif /* GOOSELIB_TYPES_H */
