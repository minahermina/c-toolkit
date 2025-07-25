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
#include <ctype.h>
#include <assert.h>

#define debug_string(str) {  \
    printf("Length: %4zu\n", (str)->length);\
    printf("Capacity: %0zu\n", (str)->capacity);\
    printf("Address: %5p\n", (str)->chars);\
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
    str_expand(string, MAX(len, STR_INIT_CAPACITY));

    str_insert_cstr_at(string, 0, init_str);
    debug_string(string);
}

void
str_insert_at(String *string, size_t pos, String *src)
{

}

void
str_append(String *dest, String *src)
{


}

void
str_copy(String *dest, const String *src)
{
    MUST(src != NULL, "src is NULL in str_copy");
    MUST(dest != NULL, "dest is NULL in str_copy");

    dest->length = src->length;
    dest->capacity = src->capacity;

    dest->chars = realloc(dest->chars, dest->capacity);

    MUST(dest->chars != NULL, "Error Allocating memory");

    memcpy(dest->chars, src->chars, dest->length);
}


void
str_substr(String *dest, const String *src, size_t pos, size_t length)
{
    size_t max_length;
    MUST(dest != NULL, "src is NULL in str_substr");
    MUST(src  != NULL, "dest is NULL in str_substr");
    MUST(pos < src->length, "pos out of bound in str_substr");

    // Calculate actual length to extract
    max_length = src->length - pos;

    length = MIN(length, max_length);

    str_expand(dest, MAX(STR_INIT_CAPACITY, length));
    dest->length = length;

    memcpy(dest->chars, src->chars + pos, length);
    dest->chars[dest->length] = '\0';
}


void
str_lower(String *string)
{
    char *p;
    size_t i;
    MUST(string        != NULL, "string is NULL in str_lower");
    MUST(string->chars != NULL, "string->chars  is NULL in str_lower");

    for(i = 0; i < string->length; ++i){
        string->chars[i] =  tolower(string->chars[i]);
    }
}

void
str_upper(String *string)
{
    char *p;
    size_t i;
    MUST(string        != NULL, "string is NULL in str_lower");
    MUST(string->chars != NULL, "string->chars  is NULL in str_lower");

    for(i = 0; i < string->length; ++i){
        string->chars[i] =  toupper(string->chars[i]);
    }
}

char
str_at(const String *string, size_t index)
{
    MUST(string        != NULL,  "string is NULL in str_at");
    MUST(string->chars != NULL,  "string->chars  is NULL in str_at");
    MUST(index < string->length, "index out of bounds in str_at");

    return string->chars[index];
}

int
str_compare(const String *string1, const String *string2)
{
    char *p, *q;
    MUST(string1 != NULL, "string1 is NULL in str_compare");
    MUST(string2 != NULL, "string2 is NULL in str_compare");
    MUST(string1->chars != NULL, "string1->chars  is NULL in str_compare");
    MUST(string2->chars != NULL, "string2->chars is NULL in str_compare");

    p = string1->chars;
    q = string2->chars;
    while(*p == *q && *p){
        p++;
        q++;
    }

    return p[0] - q[0];
}


void
str_free(String *string)
{
    MUST(string != NULL, "string is NULL in str_free");
    free(string->chars);

    string->length = 0;
    string->capacity = 0;
    string->chars = NULL;
}
