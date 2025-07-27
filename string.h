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

#include <stdio.h>

#define STR_INIT_CAPACITY 63
#define STR_FMT   "%.*s"
#define STR_ARG(str) (int)str.size, str.chars

#define STR_FOREACH(str, ch) \
    for (size_t _i = 0; _i < (str).size && ((ch) = (str).chars[_i], 1); ++_i)

#define debug_string(str) {  \
    printf("size: %4zu\n", (str)->size);\
    printf("Capacity: %0zu\n", (str)->capacity);\
    printf("Address: %5p\n", (str)->chars);\
} \

typedef struct {
    char *chars;
    size_t size;
    size_t capacity;
} String;


void str_init(String *string, const char *init_str); //
void str_insert_cstr_at(String *string, size_t pos, const char *init_str); //
void str_set_cstr(String *string, const char *cstr); //
void str_append_cstr(String *string, const char *init_str); //
void str_insert_at(String *string, size_t pos, String *src);
void str_set_at(String *string, size_t index, const char ch); //
void str_append(String *dest, String *src); //
void str_copy(String *dest, const String *src); //
void str_substr(String *dest, const String *src, size_t pos, size_t length); //
void str_reverse(const String *string); //
void str_lower(String *string); //
void str_upper(String *string); //
char str_at(const String *string, size_t index); //
int str_compare(const String *string1, const String *string2); //
int str_icompare(const String *string1, const String *string2); //
void str_free(String *string); //

