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
	gooselib_allocator_t a = gooselib_allocator_create(callback_malloc, callback_free, NULL);
	do {
		gooselib_array_t args = gooselib_array_create(sizeof(gooselib_dstring_t),
		                                              (gooselib_malloc_t) gooselib_alloc,
		                                              NULL,
		                                              &a);
		for (int index = 1; index < argc; index++) {
			gooselib_dstring_t arg = gooselib_dstring_copyReallocateC(argv[index], (gooselib_malloc_t) gooselib_alloc, &a);
			e = gooselib_array_pushElement(&args, &arg);
			if (e) {
				printf("Push failed. (%i)\n", e);
				e = 1;
				break;
			}
		}
		// decrease length by 1
		gooselib_dstring_t space = gooselib_dstring_copyReallocateC(" ", (gooselib_malloc_t) gooselib_alloc, &a);
		gooselib_dstring_t result = gooselib_dstring_reverse(gooselib_dstring_join(space, args));
		if (result.invalid) {
			puts("Invalid string.");
			e = 1;
			break;
		}
		puts((const char *) result.bytes);
	} while (0);
	(void) gooselib_free(&a);
	return e;
}
