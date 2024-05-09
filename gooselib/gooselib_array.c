#include "gooselib_array.h"
#include "gooselib_types.h"
#include <string.h>
#include <stdbool.h>

gooselib_array_t gooselib_array_create(size_t element_size,
                                       gooselib_malloc_t malloc,
                                       gooselib_free_t free,
                                       void *allocator_context) {
	gooselib_array_t array;
	array.malloc = malloc;
	array.free = free;
	array.allocator_context = allocator_context;
	array.element_size = element_size;
	array.elements_length = 0;
	array.elements_memorySize = 0;
	array.elements = NULL;
	return array;
}

void gooselib_array_quit(gooselib_array_t *array) {
	if (array->elements != NULL && array->free != NULL) {
		(void) array->free(array->allocator_context, array->elements);
	}
	array->elements_length = 0;
	array->element_size = 0;
	array->elements_memorySize = 0;
}

int gooselib_array_pushElement(gooselib_array_t *array, void *element) {
	int e = GOOSELIB_OK;

	gooselib_array_t localArray = *array;
	size_t newLengthSize = (localArray.elements_length + 1) * localArray.element_size;

	/* Add space for a new element. */
	if (newLengthSize > localArray.elements_memorySize) {
		void *block = localArray.malloc(localArray.allocator_context, 2 * newLengthSize);
		if (block == NULL) {
			e = GOOSELIB_ALLOCFAILED;
			goto cleanup;
		}
		(void) memcpy(block, localArray.elements, localArray.elements_length * localArray.element_size);
		if (localArray.free != NULL) {
			(void) localArray.free(localArray.allocator_context, localArray.elements);
		}
		localArray.elements = block;
		localArray.elements_memorySize = 2 * newLengthSize;
	}

	if (element == NULL) {
		/* Create an empty element. */
		(void) memset(((unsigned char *) localArray.elements
		               + (ptrdiff_t) (localArray.element_size * localArray.elements_length)),
		              0,
		              localArray.element_size);
	} else {
		/* Copy given element into array. */
		(void) memcpy(((unsigned char *) localArray.elements
		               + (ptrdiff_t) (localArray.element_size * localArray.elements_length)),
		              element,
		              localArray.element_size);
	}

	localArray.elements_length++;

 cleanup:
	*array = localArray;
	return e;
}

int gooselib_array_pushElements(gooselib_array_t *array, const void *elements, size_t elements_length) {
	int e = GOOSELIB_OK;
	
	bool wasNull = (elements == NULL);
	
	if (elements_length == 0) goto cleanup;
	
	// Add space for new elements.
	while ((array->elements_length + elements_length) * array->element_size > array->elements_memorySize) {
		void *buffer = array->malloc(array->allocator_context,
		                             2 * (array->elements_length + elements_length) * array->element_size);
		if (buffer == NULL) {
			e = GOOSELIB_ALLOCFAILED;
			goto cleanup;
		}
		(void) memcpy(buffer, array->elements, array->elements_length * array->element_size);
		if (array->free != NULL) {
			(void) array->free(array->allocator_context, array->elements);
		}
		array->elements = buffer;
		array->elements_memorySize = 2 * (array->elements_length + elements_length) * array->element_size;
	}
	
	if (wasNull) {
		(void) memset(&((unsigned char *) array->elements)[array->element_size * array->elements_length],
		              0,
		              elements_length * array->element_size);
	}
	else {
		// Copy given element into array.
		(void) memcpy(&((unsigned char *) array->elements)[array->element_size * array->elements_length],
		              elements,
		              elements_length * array->element_size);
	}
	
	array->elements_length += elements_length;
	
 cleanup: return e;
}

// int gooselib_array_popElement(gooselib_array_t *array, void **element, ptrdiff_t index) {
// 	int e = GOOSELIB_OK;
	
// 	*element = (unsigned char *) array->elements + index;
	
// 	l_cleanup:
	
// 	return e;
// }

int gooselib_array_popElement(gooselib_array_t *array, void *element) {
	if (array->elements_length > 0) {
		if (element != NULL) {
			(void) memcpy(element,
			              (char*)array->elements + (array->elements_length - 1) * array->element_size,
			              array->element_size);
		}
		--array->elements_length;
	}
	else {
		return GOOSELIB_ERROR;
	}
	return GOOSELIB_OK;
}

int gooselib_array_popElements(gooselib_array_t *array, void *elements, size_t count) {
	int e = GOOSELIB_OK;
	if (count < 0) {
		e = GOOSELIB_ERROR;
		goto cleanup;
	}
	if (count > 0) {
		if (array->elements_length >= count) {
			if (elements != NULL) {
				(void) memcpy(elements,
				              (char*)array->elements + (array->elements_length - count) * array->element_size,
				              count * array->element_size);
			}
			array->elements_length -= count;
		}
		else {
			e = GOOSELIB_ERROR;
			goto cleanup;
		}
	}
 cleanup: return e;
}

/*
  Fetches element on the top of the stack.
  Returns bufferUnderflow if the stack is empty.
*/
int gooselib_array_getTop(gooselib_array_t *array, void *element) {
	int e = GOOSELIB_OK;
	
	if (array->elements_length > 0) {
		(void) memcpy(element,
		              (char*)array->elements + (array->elements_length - 1) * array->element_size,
		              array->element_size);
	}
	else {
		e = GOOSELIB_ERROR;
		goto cleanup;
	}
	
 cleanup: return e;
}

/*
  Fetches element on the top of the stack.
  Returns bufferUnderflow if the stack is empty.
*/
int gooselib_array_setTop(gooselib_array_t *array, void *element) {
	int e = GOOSELIB_OK;
	
	if (array->elements_length > 0) {
		(void) memcpy((char*)array->elements + (array->elements_length - 1) * array->element_size,
		              element,
		              array->element_size);
	}
	else {
		e = GOOSELIB_ERROR;
		goto cleanup;
	}
	
 cleanup: return e;
}

int gooselib_array_get(gooselib_array_t *array, void *element, ptrdiff_t index) {
	if ((index >= 0) && ((size_t) index < array->elements_length)) {
		(void) memcpy(element, (uint8_t *) array->elements + index * array->element_size, array->element_size);
		return GOOSELIB_OK;
	}
	return GOOSELIB_ERROR;
}

int gooselib_array_set(gooselib_array_t *array, const void *element, ptrdiff_t index) {
	if ((index >= 0) && ((size_t) index < array->elements_length)) {
		(void) memcpy( (char*)array->elements + index * array->element_size, element, array->element_size);
		return GOOSELIB_OK;
	}
	return GOOSELIB_ERROR;
}

/*
  Keeps arrayDestination's allocator the same. After all, it's easy enough to copy the allocator yourself if needed.
*/
int gooselib_array_copy(gooselib_array_t *arrayDestination, gooselib_array_t arraySource) {
	int e = GOOSELIB_OK;

	if (arrayDestination->free != NULL) {
		(void) arrayDestination->free(arrayDestination->allocator_context, arrayDestination->elements);
	}
	arrayDestination->elements = arrayDestination->malloc(arrayDestination->allocator_context,
	                                                      arraySource.elements_memorySize);
	if (arrayDestination->elements == NULL) {
		e = GOOSELIB_ALLOCFAILED;
		goto cleanup;
	}
	
	arrayDestination->elements_length = arraySource.elements_length;
	// arrayDestination->memoryAllocation = NULL;
	arrayDestination->element_size = arraySource.element_size;
	arrayDestination->elements_memorySize = arraySource.elements_memorySize;
	
	(void) memcpy(arrayDestination->elements,
	              arraySource.elements,
	              arraySource.elements_length * arraySource.element_size);
	
	*arrayDestination = arraySource;
	
 cleanup: return e;
}

// Just `array_push` but for arrays.
int gooselib_array_append(gooselib_array_t *arrayDestination, gooselib_array_t *arraySource) {
	int e = GOOSELIB_OK;

	// Add space for new elements.
	while ((arrayDestination->elements_length + arraySource->elements_length) * arrayDestination->element_size
	       > arrayDestination->elements_memorySize) {
		void *buffer = arrayDestination->malloc(arrayDestination->allocator_context,
		                                        (2
		                                         * (arrayDestination->elements_length + arraySource->elements_length)
		                                         * arrayDestination->element_size));
		if (buffer == NULL) {
			e = GOOSELIB_ALLOCFAILED;
		}
		(void) memcpy(buffer,
		              arrayDestination->elements,
		              arrayDestination->elements_length * arrayDestination->element_size);
		if (arrayDestination->free != NULL) {
			(void) arrayDestination->free(arrayDestination->allocator_context, arrayDestination->elements);
		}
		arrayDestination->elements = buffer;
		arrayDestination->elements_memorySize = (2
		                                         * (arrayDestination->elements_length + arraySource->elements_length)
		                                         * arrayDestination->element_size);
	}
	
	// Copy given element into array.
	(void) memcpy(&((unsigned char *) arrayDestination->elements)[arrayDestination->element_size
	                                                              * arrayDestination->elements_length],
	              arraySource->elements,
	              arraySource->elements_length * arrayDestination->element_size);
	
	arrayDestination->elements_length += arraySource->elements_length;

 cleanup: return e;
}

void gooselib_array_clear(gooselib_array_t *array) {
	if (array->elements_length == 0) return;
	array->elements_length = 0;
	array->elements_memorySize = 0;
	if (array->free != NULL) {
		(void) array->free(array->allocator_context, array->elements);
	}
}
