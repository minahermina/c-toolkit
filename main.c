#include <stdio.h>
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
    arena_init(&arena, 1024 * 1024);

    str_set_cstr(&str, "Mina", .arena=&arena);
    // printf("str.arr = %p\n", str.arr);
    str_append_cstr(&str, "Mina", .arena=&arena);
    // printf("str.arr = %p\n", str.arr);
    str_append_cstr(&str, "Mina", .arena=&arena);
    // printf("str.arr = %p\n", str.arr);
    // str_append_cstr(&str, "Mina", .arena=&arena);
    // str_append_cstr(&str, "Mina", .arena=&arena);
    // str_append_cstr(&str, "Mina", .arena=&arena);
    // str_append_cstr_n(&str1, "Mina", strlen("Mina"), .arena=&arena);
    // read_file_to_string(&str, "/home/mina/.bashrc", .arena=&arena);

    printf(STR_FMT"\n", STR_ARG(str));
    debug_string(&str);
    arena_dump(&arena);
    arena_destroy(&arena);

    return 0;
}
