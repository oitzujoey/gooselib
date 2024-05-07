#include "../gooselib/gooselib_string.h"
#include "../gooselib/gooselib_array.h"
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
	gooselib_array_t args = gooselib_array_create(sizeof(gooselib_dstring_t), callback_malloc, callback_free, NULL);
	for (int index = 1; index < argc; index++) {
		gooselib_dstring_t arg = gooselib_dstring_copyReallocateC(argv[index], callback_malloc, NULL);
		int e = gooselib_array_pushElement(&args, &arg);
		if (e) {
			printf("Push failed. (%i)\n", e);
			return 1;
		}
	}
	// decrease length by 1
	gooselib_dstring_t space = gooselib_dstring_copyReallocateC(" ", callback_malloc, NULL);
	gooselib_dstring_t result = gooselib_dstring_reverse(gooselib_dstring_join(space, args));
	puts((const char *) result.bytes);
}
