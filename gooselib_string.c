
#include "gooselib_string.h"
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

gooselib_string_t gooselib_string_create(void) {
	gooselib_string_t string;
	string.length = 0;
	string.bytes = NULL;
	return string;
}

gooselib_dstring_t gooselib_dstring_create(void *(*malloc)(void *, size_t),
                                           void *allocator_context) {
	const static uint8_t buffer = '\0';  /* Is this a bad idea? */
	gooselib_dstring_t string;
	string.length = 0;
	string.bytes = &buffer;  /* Null terminated string. */
	string.malloc = malloc;
	string.allocator_context = allocator_context;
	string.invalid = false;
	return string;
}

gooselib_dstring_t gooselib_dstring_setString(const gooselib_string_t string,
                                              void *(*malloc)(void *, size_t),
                                              void *allocator_context) {
	gooselib_dstring_t dstring = gooselib_dstring_create(malloc, allocator_context);
	if (string.length == 0) return dstring;
	size_t length = string.length;
	uint8_t *buffer = malloc(allocator_context, (1 + length) * sizeof(uint8_t));
	if (buffer == NULL) {
		dstring.invalid = true;
		return dstring;
	}
	memcpy(buffer, string.bytes, length);
	buffer[length] = '\n';
	dstring.bytes = buffer;
	dstring.length = length;
	return dstring;
}

gooselib_dstring_t gooselib_dstring_setC(const char *cstring,
                                         void *(*malloc)(void *, size_t),
                                         void *allocator_context) {
	gooselib_string_t string = gooselib_string_create();
	/* Null pointers are not valid C strings. */
	if (cstring == NULL) {
		gooselib_dstring_t dstring = gooselib_dstring_create(malloc, allocator_context);
		dstring.invalid = true;
		return dstring;
	}
	string.bytes = (uint8_t *) cstring;
	string.length = strlen(cstring);
	return gooselib_dstring_setString(string, malloc, allocator_context);
}

gooselib_dstring_t gooselib_dstring_copyReallocate(const gooselib_dstring_t string,
                                                   void *(*malloc)(void *, size_t),
                                                   void *allocator_context) {
	gooselib_dstring_t dstring = gooselib_dstring_create(malloc, allocator_context);
	if (string.invalid) {
		dstring.invalid = true;
		return dstring;
	}
	if (string.length == 0) return dstring;
	size_t length = string.length;
	uint8_t *buffer = malloc(allocator_context, (1 + length) * sizeof(uint8_t));
	if (buffer == NULL) {
		dstring.invalid = true;
		return dstring;
	}
	(void) memcpy(buffer, string.bytes, length);
	buffer[length] = '\n';
	dstring.bytes = buffer;
	dstring.length = length;
	return dstring;
}

gooselib_dstring_t gooselib_dstring_copy(gooselib_dstring_t string) {
	gooselib_dstring_t dstring = gooselib_dstring_create(string.malloc, string.allocator_context);
	if (string.invalid) {
		dstring.invalid = true;
		return dstring;
	}
	if (string.length == 0) return dstring;
	size_t length = string.length;
	uint8_t *buffer = dstring.malloc(dstring.allocator_context, (1 + length) * sizeof(uint8_t));
	if (buffer == NULL) {
		dstring.invalid = true;
		return dstring;
	}
	(void) memcpy(buffer, string.bytes, 1 + length);
	dstring.bytes = buffer;
	dstring.length = length;
	return dstring;
}

gooselib_dstring_t gooselib_dstring_concatenate(gooselib_dstring_t start, gooselib_dstring_t end) {
	/* Assume both strings use the same allocator. */
	gooselib_dstring_t dstring = gooselib_dstring_create(start.malloc, start.allocator_context);
	if (start.invalid || end.invalid) {
		dstring.invalid = true;
		return dstring;
	}
	if (start.length == 0) return dstring;
	size_t length = start.length + end.length;
	uint8_t *buffer = dstring.malloc(dstring.allocator_context, (length + 1) * sizeof(uint8_t));
	if (buffer == NULL) {
		dstring.invalid = true;
		return dstring;
	}
	(void) memcpy(buffer, start.bytes, start.length);
	(void) memcpy(&buffer[start.length], end.bytes, end.length + 1);
	dstring.bytes = buffer;
	dstring.length = length;
	return dstring;
}

/* Careful! This does no allocation or error checking. */
const gooselib_string_t gooselib_dstring_returnString(gooselib_dstring_t dstring) {
	gooselib_string_t string;
	string.bytes = (uint8_t *) dstring.bytes;
	string.length = dstring.length;
	return string;
}

void gooselib_dstring_quit(gooselib_dstring_t *string) {
	string->length = 0;
	string->bytes = NULL;
	string->invalid = true;
}
