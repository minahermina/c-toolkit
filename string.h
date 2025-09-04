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

#ifndef STRING_LIB
#define STRING_LIB

#include "arena.h"

#define STR_INIT_CAPACITY 63
#define STR_FMT   "%.*s"
#define STR_ARG(str) (int)str.size, str.arr

#define STR_FOREACH(str, ch) \
    for (size_t _i = 0; _i < (str).size && ((ch) = (str).arr[_i], 1); ++_i)


typedef struct {
    Arena *arena;
} Args;

typedef struct {
    char *arr;
    size_t size;
    size_t capacity;
} String;


void _str_insert_cstr_at(String *string, const char *cstr, size_t pos, Args args);
#define str_insert_cstr_at(string, cstr, pos, ...) \
    _str_insert_cstr_at(string, cstr, pos, (Args){__VA_ARGS__})

void _str_insert_cstr_n_at(String *string, const char *cstr, size_t n, size_t pos, Args args);
#define str_insert_cstr_n_at(string, cstr, n, pos, ...) \
    _str_insert_cstr_n_at(string, cstr, n, pos, (Args){__VA_ARGS__})

void _str_append_cstr(String *string, const char *cstr, Args args);
#define str_append_cstr(string, cstr, ...) \
    _str_append_cstr(string, cstr, (Args){__VA_ARGS__})

void _str_append_cstr_n(String *string, const char *cstr, size_t n, Args args);
#define str_append_cstr_n(string, cstr, n, ...) \
    _str_append_cstr_n(string, cstr, n, (Args){__VA_ARGS__})

void str_insert_at(String *string, size_t pos, String *src);
void str_set_at(String *string, size_t index, const char ch);

void _str_set(String *dest, const String *src, Args args);

#define str_set(dest, src, ...) \
    _str_set(dest, src, (Args){__VA_ARGS__})

void _str_set_cstr(String *dest, const char *src, Args args);
#define str_set_cstr(dest, src, ...) \
    _str_set_cstr(dest, src, (Args){__VA_ARGS__})

void _str_append(String *dest, const String *src, Args args);
#define str_append(dest, src, ...) \
    _str_append(dest, src, (Args){__VA_ARGS__})

void _str_substr(String *dest, const String *src, size_t pos, size_t length, Args args);
#define str_substr(dest, src, pos, length, ...) \
    _str_substr(dest, src, pos, length, (Args){__VA_ARGS__})

int str_find_cstr(const String *string, const char *cstr);

int str_find(const String *string1, const String *string2);
int str_find_ch(const String *string, const char ch);
void str_reverse(const String *string);
void str_lower(String *string);
void str_upper(String *string);
char str_at(const String *string, size_t index);
int str_compare(const String *string1, const String *string2);
int str_icompare(const String *string1, const String *string2);

void str_trim(String *string);
void str_trim_left(String *string);
void str_trim_right(String *string);
/* 
void str_remove(String *string1, const String *string2);
void str_remove_cstr(String *string, const char *cstr);
void str_replace(String *string1, const String *string2, Args args); 
void str_replace_cstr(String *string, const char *cstr, Args args); 
void str_clear(String *string); 
*/
void str_free(String *string);

int _str_from_file(String *string, const char *filename, Args args);
#define str_from_file(string, filename, ...) \
    _str_from_file(string, filename, (Args){__VA_ARGS__})

#endif
