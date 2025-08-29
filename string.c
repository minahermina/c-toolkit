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

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include "string.h"

#define debug_string(str) {  \
    printf("size: %4zu\n", (str)->size);\
    printf("Capacity: %0zu\n", (str)->capacity);\
    printf("Address: %5p\n", (str)->arr);\
} \

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
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

static void *
_realloc(void *ptr, size_t oldsize, size_t newsize, Arena *arena)
{
    if (arena != NULL){
        return arena_realloc(arena, ptr, oldsize, newsize);
    } 
    else {
        return realloc(ptr, newsize);
    }
}

static void 
str_realloc(String *string, size_t size, Arena *arena)
{
    MUST(string != NULL, "string is NULL in str_realloc");
    string->arr = _realloc(string->arr, string->size, size, arena);
}

static void
str_resize(String *string, size_t len, Arena *arena)
{
    MUST(string != NULL, "string is NULL in str_resize");

    /* No expanding is required. */
    if(len <= string->capacity - string->size){
        return;
    }

    string->capacity = nearest_pow(string->size + len + 1);
    /* Overflow happens */
    if(string->capacity == 0){
        string->capacity = string->size + len + 1;
    }

    str_realloc(string, string->capacity, arena);
    string->size = len;
    MUST(string->arr != NULL, "Error Allocating memory in str_resize");
}

void
_str_insert_cstr_at(String *string, const char *cstr, size_t pos, Args args)
{
    MUST(string != NULL,      "string is NULL in str_insert_cstr_at");
    MUST(cstr != NULL,        "cstr is NULL in str_insert_cstr_at");
    MUST(pos <= string->size, "pos out of bounds in str_insert_cstr_at");
    _str_insert_cstr_n_at(string, cstr, strlen(cstr), pos, args);
}


void
_str_insert_cstr_n_at(String *string, const char *cstr, size_t n, size_t pos, Args args)
{
    size_t insert_len;
    MUST(string != NULL,                  "string is NULL in str_insert_cstr_at");
    MUST(cstr != NULL,                    "cstr is NULL in str_insert_cstr_at");
    MUST(pos <= string->size,             "pos out of bounds in str_insert_cstr_at");
    MUST((n <= strlen(cstr)) && (n >= 0), "0 <= n <= strlen(cstr) is not satisfied in str_insert_cstr_at");

    if(n == 0){
        return;
    }
    insert_len = n;

    str_resize(string, string->size + insert_len, args.arena);

    // Move existing content to make room (if not inserting at end)
    if (pos < string->size) {
        memmove(string->arr + pos + insert_len,
                string->arr + pos,
                string->size - pos);
    }

    if (insert_len == 1) {
        string->arr[pos] = *cstr;
    } else {
        memcpy(string->arr + pos, cstr, insert_len);
    }

    string->arr[string->size] = '\0';

}

void
_str_append_cstr(String *string, const char *cstr, Args args)
{
    MUST(string != NULL, "string is NULL in str_append_cstr");
    MUST(cstr   != NULL, "cstr is NULL in str_append_cstr");
    _str_insert_cstr_at(string, cstr, string->size, args);
}

void
_str_append_cstr_n(String *string, const char *cstr, size_t n, Args args)
{

    MUST(string != NULL, "string is NULL in str_append_cstr");
    MUST(cstr   != NULL, "cstr is NULL in str_append_cstr");
    _str_insert_cstr_n_at(string, cstr, n, string->size, args);
}

void
str_insert_at(String *string, size_t pos, String *src)
{

}


void
str_set_at(String *string, size_t index, const char ch)
{
    MUST(string != NULL,        "string is NULL in str_set_at");
    MUST(string->arr != NULL,   "string->arr is NULL in str_set_at");
    MUST(index <= string->size, "index is out of bound in str_set_at");
    string->arr[index] = ch;
}

void
_str_append(String *dest, const String *src, Args args)
{
    char *ptr = NULL;
    MUST(dest != NULL,      "dest is NULL in str_append");
    MUST(dest->arr != NULL, "dest->arr is NULL in str_append");
    MUST(src != NULL,       "src is NULL in str_append");
    MUST(src->arr != NULL,  "src->arr is NULL in str_append");

    if(dest == src){
        ptr = _realloc(ptr, dest->size, src->size, args.arena);
        memcpy(ptr, src->arr, src->size);
    }
    else{
        ptr = src->arr;
    }
    _str_append_cstr(dest, ptr, args);
    if(dest == src && args.arena == NULL) 
        free(ptr);

    // printf("%p\n", ptr);
}

void 
_str_set(String *dest, const String *src, Args args)
{
    MUST(src != NULL,  "src is NULL in str_set");
    MUST(dest != NULL, "dest is NULL in str_set");

    str_resize(dest, src->size, args.arena);
    MUST(dest->arr != NULL, "Error Allocating memory");

    memcpy(dest->arr, src->arr, dest->size);
}

void
_str_set_cstr(String *dest, const char *src, Args args)
{
    size_t src_len;
    MUST(src != NULL,  "src is NULL in str_set_cstr");
    MUST(dest != NULL, "dest is NULL in str_set_cstr");

    src_len = strlen(src);
    str_resize(dest, src_len, args.arena);
    MUST(dest->arr != NULL, "Error Allocating memory");

    memcpy(dest->arr, src, dest->size);
}

void
_str_substr(String *dest, const String *src, size_t pos, size_t length, Args args)
{
    size_t max_length;
    MUST(dest != NULL, "src is NULL in str_substr");
    MUST(src  != NULL, "dest is NULL in str_substr");
    MUST(pos < src->size, "pos out of bound in str_substr");

    // Calculate actual length to extract
    max_length = src->size - pos;

    length = MIN(length, max_length);

    length = (dest->capacity > length ? length : STR_INIT_CAPACITY);
    str_resize(dest, length, args.arena);
    dest->size = length;

    memcpy(dest->arr, src->arr + pos, length);
    dest->arr[dest->size] = '\0';
}

void
str_reverse(const String *string)
{
    size_t i, j, size;
    char temp;
    MUST(string != NULL, "string  is NULL in str_reverse");
    MUST(string->arr != NULL, "string->arr  is NULL in str_reverse");

    size = string->size;
    for(i = 0, j = size-1; i < size/2; i++, j--){
        temp = string->arr[i];
        string->arr[i] = string->arr[j];
        string->arr[j] = temp;
    }
}

void
str_lower(String *string)
{
    size_t i;
    MUST(string      != NULL, "string is NULL in str_lower");
    MUST(string->arr != NULL, "string->arr  is NULL in str_lower");

    for(i = 0; i < string->size; ++i){
        string->arr[i] =  tolower(string->arr[i]);
    }
}

void
str_upper(String *string)
{
    size_t i;
    MUST(string        != NULL, "string is NULL in str_lower");
    MUST(string->arr != NULL, "string->arr  is NULL in str_lower");

    for(i = 0; i < string->size; ++i){
        string->arr[i] =  toupper(string->arr[i]);
    }
}

char
str_at(const String *string, size_t index)
{
    MUST(string        != NULL,  "string is NULL in str_at");
    MUST(string->arr != NULL,  "string->arr  is NULL in str_at");
    MUST(index < string->size, "index out of bounds in str_at");

    return string->arr[index];
}

int
str_compare(const String *string1, const String *string2)
{
    char *p, *q;
    MUST(string1 != NULL, "string1 is NULL in str_compare");
    MUST(string2 != NULL, "string2 is NULL in str_compare");
    MUST(string1->arr != NULL, "string1->arr  is NULL in str_compare");
    MUST(string2->arr != NULL, "string2->arr is NULL in str_compare");

    p = string1->arr;
    q = string2->arr;
    while(*p == *q && *p){
        p++;
        q++;
    }

    return p[0] - q[0];
}

int
str_icompare(const String *string1, const String *string2)
{
    char *p, *q;
    MUST(string1 != NULL, "string1 is NULL in str_compare");
    MUST(string2 != NULL, "string2 is NULL in str_compare");
    MUST(string1->arr != NULL, "string1->arr  is NULL in str_compare");
    MUST(string2->arr != NULL, "string2->arr is NULL in str_compare");

    p = string1->arr;
    q = string2->arr;
    while(tolower(*p) == tolower(*q) && *p){
        p++;
        q++;
    }

    return tolower(p[0]) - tolower(q[0]);
}


void
str_free(String *string)
{
    MUST(string != NULL, "string is NULL in str_free");
    free(string->arr);

    string->size = 0;
    string->capacity = 0;
    string->arr = NULL;
}
