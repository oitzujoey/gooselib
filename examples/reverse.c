#include "../gooselib/gooselib_string.h"
#include "../gooselib/gooselib_array.h"
#include "../gooselib/gooselib_allocator.h"
#include <stdlib.h>
#include <stdio.h>

void *callback_malloc(void *context, size_t size) {
	(void) context;
	return malloc(size);
}

void callback_free(void *context, void *memory) {
	(void) context;
	free(memory);
}

int main(int argc, char *argv[]) {
	int e = 0;
	// Create arena allocator.
	gooselib_allocator_t a = gooselib_allocator_create(callback_malloc, callback_free, NULL);
	do {
		// Create dynamic array. Normally that null argument would be the `free` function, but since we're using an
		// arena there's no point. Passing the arena's free function in would free ALL memory, not just the block the
		// array intended to free.
		gooselib_array_t args = gooselib_array_create(sizeof(gooselib_dstring_t),
		                                              (gooselib_malloc_t) gooselib_alloc,
		                                              NULL,
		                                              &a);
		for (int index = 1; index < argc; index++) {
			// Create a dynamically allocated string. Dstrings leak memory by design.
			gooselib_dstring_t arg = gooselib_dstring_copyReallocateC(argv[index],
			                                                          (gooselib_malloc_t) gooselib_alloc,
			                                                          &a);
			e = gooselib_array_pushElement(&args, &arg);
			if (e) {
				printf("Push failed. (%i)\n", e);
				e = 1;
				break;
			}
		}
		// Create a dstring that contains a space. Ugly isn't it?
		gooselib_dstring_t space = gooselib_dstring_copyReallocateC(" ", (gooselib_malloc_t) gooselib_alloc, &a);
		// `gooselib_dstring_join` expects an array of dstrings. Bad things happen if the array does not contain
		// dstrings.
		// Dynamic string functions compose. Errors only need to be checked for before accessing the `bytes` and
		// `length` fields.
		gooselib_dstring_t result = gooselib_dstring_reverse(gooselib_dstring_join(space, args));
		// *Always* check if a string is valid before accessing its members.
		if (result.invalid) {
			puts("Invalid string.");
			e = 1;
			break;
		}
		// Dstrings are null terminated.
		puts((const char *) result.bytes);
	} while (0);
	// Free all the memory.
	(void) gooselib_free(&a);
	return e;
}
