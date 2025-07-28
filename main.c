#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "string.h"
#include "arena.h"

#define debug_string(str) {  \
    printf("size: %4zu\n", (str)->size);\
    printf("Capacity: %0zu\n", (str)->capacity);\
    printf("Address: %5p\n", (str)->arr);\
} \

int
main()
{
    Arena arena = {0};
    String str = {0};
    arena_init(&arena, ARENA_REGION_DEFAULT_CAPACITY);
    arena_dump(&arena);

    str_init(&str, "MINA", &arena);
    printf("str is: " STR_FMT "\n", STR_ARG(str));
    str_append_cstr(&str, "MINA Albert Saeed", &arena);
    str_append_cstr(&str, "MINA Albert Saeed", &arena);
    str_append_cstr(&str, "MINA Albert Saeed", &arena);
    str_append_cstr(&str, "MINA Albert Saeed", &arena);

    arena_destroy(&str);

    return 0;
}
