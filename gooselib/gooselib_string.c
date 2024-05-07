
#include "gooselib_string.h"
#include "gooselib_array.h"
#include "gooselib_types.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>


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
	uint8_t *buffer = malloc(allocator_context, (length + 1) * sizeof(uint8_t));
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

gooselib_dstring_t gooselib_dstring_copyReallocateS(const gooselib_string_t string,
                                                    void *(*malloc)(void *, size_t),
                                                    void *allocator_context) {
	gooselib_dstring_t dstring = gooselib_dstring_create(malloc, allocator_context);
	if (string.length == 0) return dstring;
	size_t length = string.length;
	uint8_t *buffer = malloc(allocator_context, (length + 1) * sizeof(uint8_t));
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

gooselib_dstring_t gooselib_dstring_copyReallocateC(const char *cstring,
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
	return gooselib_dstring_copyReallocateS(string, malloc, allocator_context);
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
	size_t length = start.length + end.length;
	if (length == 0) return dstring;
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

gooselib_dstring_t gooselib_dstring_concatenateS(gooselib_dstring_t start, gooselib_string_t end) {
	/* Assume both strings use the same allocator. */
	gooselib_dstring_t dstring = gooselib_dstring_create(start.malloc, start.allocator_context);
	if (start.invalid) {
		dstring.invalid = true;
		return dstring;
	}
	size_t length = start.length + end.length;
	if (length == 0) return dstring;
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

gooselib_dstring_t gooselib_dstring_concatenateC(gooselib_dstring_t start, const char *end) {
	/* Assume both strings use the same allocator. */
	gooselib_string_t end_string = gooselib_string_create();
	/* Null pointers are not valid C strings. */
	if (end == NULL) {
		gooselib_dstring_t dstring = gooselib_dstring_create(start.malloc, start.allocator_context);
		dstring.invalid = true;
		return dstring;
	}
	end_string.length = strlen(end);
	end_string.bytes = (uint8_t *) end;
	return gooselib_dstring_concatenateS(start, end_string);
}

gooselib_dstring_t gooselib_dstring_join(gooselib_dstring_t dstring, gooselib_array_t string_array) {
	gooselib_dstring_t result = gooselib_dstring_create(dstring.malloc, dstring.allocator_context);
	for (size_t index = 0; index < string_array.elements_length; index++) {
		if (index != 0) {
			result = gooselib_dstring_concatenate(result, dstring);
		}
		gooselib_dstring_t element;
		int e = gooselib_array_get(&string_array, &element, index);
		if (e) {
			result.invalid = true;
			return result;
		}
		result = gooselib_dstring_concatenate(result, element);
	}
	return result;
}

gooselib_dstring_t gooselib_dstring_reverse(gooselib_dstring_t ds) {
	gooselib_dstring_t reversed = gooselib_dstring_create(ds.malloc, ds.allocator_context);
	uint8_t *buffer = reversed.malloc(reversed.allocator_context, (ds.length + 1) * sizeof(uint8_t));
	if (buffer == NULL) {
		reversed.invalid = true;
		return reversed;
	}
	/* Probably an easy optimization here. */
	for (size_t index = 0; index < ds.length; index++) {
		buffer[ds.length - index - 1] = ds.bytes[index];
	}
	buffer[ds.length] = '\0';
	reversed.bytes = buffer;
	reversed.length = ds.length;
	
	return reversed;
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
