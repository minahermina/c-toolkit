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
#define STR_ARG(str) (int)str.length, str.chars

#define debug_string(str) {  \
    printf("Length: %4zu\n", (str)->length);\
    printf("Capacity: %0zu\n", (str)->capacity);\
    printf("Address: %5p\n", (str)->chars);\
} \

typedef struct {
    char *chars;
    size_t length;
    size_t capacity;
} String;


void str_init(String *string, const char *init_str);
void str_insert_cstr_at(String *string, size_t pos, const char *init_str);
void str_append_cstr(String *string, const char *init_str);

