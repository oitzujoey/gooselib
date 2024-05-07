# Allocators

This is documentation for the allocation conventions and the allocation systems.

## Conventions

A general purpose allocator requires an allocation function (`malloc`), a deallocation function (`free`), and a context. The standard library's allocator has a global context, which is why you don't have to specify it when using that allocator. Custom allocators commonly require a context. This allows there to be multiple copies of the allocator.

`gooselib_malloc_t` is the allocation function. `gooselib_free_t` is the deallocation function. The type of the allocator context is `void *`.


## Arena allocator
