#ifndef GOOSELIB_STRING_H
#define GOOSELIB_STRING_H

#include <stdint.h>
#include <stddef.h>

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

#endif /* GOOSELIB_STRING_H */
