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

int
read_file_to_string(String *string, const char *filename)
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
        str_append_cstr_n(string, buf, read_bytes);
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
    String str1 = {0};
    String str2 = {0};
    arena_init(&arena, ARENA_REGION_DEFAULT_CAPACITY);

    str_set_cstr(&str1, "Mina", .arena=&arena);
    str_set_cstr(&str2, "Albert", .arena=&arena);
    // str_substr(&str1, &str2, 0, 2, .arena=&arena);
    // str.size = 0;
    // read_file_to_string(&str, "/home/mina/.config/nvim/lua/mappings.lua");

    printf(STR_FMT"\n", STR_ARG(str1));
    // printf(STR_FMT"\n", STR_ARG(str2));
    // printf("-----------------");
    // arena_dump(&arena);
    debug_string(&str1);
    debug_string(&str2);
    // printf("%ld", (str2.arr - str1.arr));
    // str_append_cstr(&str, ";Age: 22");
    // str_append_cstr(&str, " test string", .arena = &arena);
    // str_append_cstr(&str, "MINA Albert Saeed");
    // str_append_cstr(&str, "MINA Albert Saeed");
    // str_append_cstr(&str, "MINA Albert Saeed");

    // printf("str is: " STR_FMT "\n", STR_ARG(str));
    // arena_destroy(&arena);

    return 0;
}
