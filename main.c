#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include "string.h"
#include "arena.h"

#define debug_string(str) {  \
    printf("size: %4zu\n", (str)->size);\
    printf("Capacity: %0zu\n", (str)->capacity);\
    printf("Address: %5p\n", (str)->arr);\
} \

#define read_file_to_string(string, filename, ...) \
    _read_file_to_string(string, filename, (Args){__VA_ARGS__})
int
_read_file_to_string(String *string, const char *filename, Args args)
{
    int fd;
    ssize_t read_bytes;
    #define SIZE (4096)
    char buf[SIZE] = {0};

    string->size = 0;
    if (string == NULL) {
        fprintf(stderr, "Invalid string pointer for file %s, %s\n", filename, strerror(errno));
        return -1;
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error opening the file %s, %s\n", filename, strerror(errno));
        return -1;
    }

    while ((read_bytes = read(fd, buf, SIZE)) > 0) {
        str_append_cstr_n(string, buf, read_bytes, .arena=args.arena);
    }

    if (read_bytes < 0) {
        fprintf(stderr, "Error reading from the file %s\n", filename);
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

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
