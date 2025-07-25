/*
    Copyright (C) 2025  Mina Albert Saeed <mina.albert.saeed@gmail.com>

    A simple string implementation for C with dynamic memory management.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "string.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define debug_string(str) {  \
    printf("Length: %4zu\n", (str)->length);\
    printf("Capacity: %0zu\n", (str)->capacity);\
    printf("Address: %5p\n", (str)->chars);\
} \

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MUST(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Error: %s\n", (message)); \
            assert(condition); \
        } \
    } while (0)


static size_t
nearest_pow(size_t num)
{
    num--;

    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    #if SIZE_MAX > 0xFFFFFFFF
        num |= num >> 32;  /* 64-bit systems */
    #endif

    return num + 1;
}

static void
str_expand(String *string, size_t len)
{
    MUST(string != NULL, "string is NULL in str_expand");

    /* No expanding is required. */
    if(len <= string->capacity - string->length){
        return;
    }

    string->capacity = nearest_pow(string->length + len + 1);
    /* Overflow happens */
    if(string->capacity == 0){
        string->capacity = string->length + len + 1;
    }

    string->chars = realloc(string->chars, string->capacity);
    MUST(string->chars != NULL, "Error Allocating memory");
}

void
str_insert_cstr_at(String *string, size_t pos, const char *cstr)
{
    size_t insert_len;
    MUST(string != NULL,        "string is NULL in str_insert_cstr_at");
    MUST(cstr != NULL,          "cstr is NULL in str_init");
    MUST(pos <= string->length, "pos out of bounds in str_insert_cstr_at");

    insert_len = strlen(cstr);

    str_expand(string, insert_len);

    // Move existing content to make room (if not inserting at end)
    if (pos < string->length) {
        memmove(string->chars + pos + insert_len,
                string->chars + pos,
                string->length - pos);
    }

    if (insert_len == 1) {
        string->chars[pos] = *cstr;
    } else {
        memcpy(string->chars + pos, cstr, insert_len);
    }

    string->length += insert_len;
    string->chars[string->length] = '\0';

}

void
str_append_cstr(String *string, const char *cstr)
{
    MUST(string != NULL,        "string is NULL in str_insert_cstr_at");
    MUST(cstr != NULL,          "cstr is NULL in str_init");
    str_insert_cstr_at(string, string->length, cstr);
}

void
str_init(String *string, const char *init_str)
{
    size_t len;
    MUST(string   != NULL, "string is NULL in str_init");
    MUST(init_str != NULL, "init_str is NULL in str_init");

    string->length = 0;
    string->capacity = 0;

    len = strlen(init_str); 
    printf("init_str: %zu\n", len);
    str_expand(string, MAX(len, STR_INIT_CAPACITY));

    str_insert_cstr_at(string, 0, init_str);
    debug_string(string);
}
