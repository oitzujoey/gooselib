#ifndef GOOSELIB_STRING_H
#define GOOSELIB_STRING_H

#include "gooselib_types.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


gooselib_string_t gooselib_string_create(void);
gooselib_dstring_t gooselib_dstring_create(void *(*malloc)(void *, size_t), void *allocator_context);
gooselib_dstring_t gooselib_dstring_copyReallocateC(const char *cstring,
                                                    void *(*malloc)(void *, size_t),
                                                    void *allocator_context);
gooselib_dstring_t gooselib_dstring_concatenate(gooselib_dstring_t start, gooselib_dstring_t end);
gooselib_dstring_t gooselib_dstring_concatenate(gooselib_dstring_t start, gooselib_dstring_t end);
gooselib_dstring_t gooselib_dstring_concatenateC(gooselib_dstring_t start, const char *end);
gooselib_dstring_t gooselib_dstring_join(gooselib_dstring_t dstring, gooselib_array_t string_array);
gooselib_dstring_t gooselib_dstring_reverse(gooselib_dstring_t ds);

#endif /* GOOSELIB_STRING_H */
