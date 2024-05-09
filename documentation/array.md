# Dynamic arrays

## Theory

`gooselib_array_t` is the dynamic array type. Elements can be pushed and popped from the end, and arbitrary elements can be read and written in roughly linear time. `malloc` (or whatever allocator you passed to the array) is _not _ called every time you push an element. Extra space is allocated in the expectation that more elements will be pushed. After you run out of that space, the allocator is called and the amount of memory used by the array is doubled. Calls to the allocator become less frequent as you push more elements into the array.

## Usage

This code is untested.

In this case we will use the stdlib allocator. Gooselib expects a different type signature for allocators so we need to write wrappers for them.

```c
void *callback_malloc(void *context, size_t size) {
	(void) context;
	return malloc(size);
}

void callback_free(void *context, void *memory) {
	(void) context;
	free(memory);
}
```

Create an array. Pass the size of the data type you wish to store, then the allocator, the deallocator, and the allocator context. Since we are using `malloc`, the context doesn't exist and so we pass `NULL`.

```c
gooselib_array_t array = gooselib_array_create(sizeof(int), callback_malloc, callback_free, NULL);
```

Push some integers in it.

```c
for (int i = 0; i < 10; i++) {
	int e = gooselib_array_pushElement(&array, &i);
	if (e) {
		printf("Push failed. (%i)\n", e);
		return 1;
	}
}
```

Print the fifth element.

```c
int integer;
int e = gooselib_array_get(&array, &integer, 4);
if (e) {
	printf("Get failed. (%i)\n", e);
	return 1;
}
printf("integer: %i\n", integer);
```

Free the array's memory.

```c
(void) gooselib_array_quit(&array);
```

## DuckLib

This was ported from DuckLib.

The allocation strategy was removed. The memory size now always doubles when the array reaches the end of its allocated memory.
