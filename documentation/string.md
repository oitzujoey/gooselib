# Strings

## C strings

Normal null terminated strings.

## `gooselib_string_t`

A string with the length attached to it.

## `gooselib_dstring_t`

A dynamically allocated string. Dstrings are intended to be immutable so I suggest you don't mutate them. They are always null terminated, so their `bytes` field can be used as a constant C string.
