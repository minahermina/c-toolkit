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

#include <stdio.h>
#include "arena.h"

#define STR_INIT_CAPACITY 63
#define STR_FMT   "%.*s"
#define STR_ARG(str) (int)str.size, str.arr

#define STR_FOREACH(str, ch) \
    for (size_t _i = 0; _i < (str).size && ((ch) = (str).arr[_i], 1); ++_i)

typedef struct {
    char *arr;
    size_t size;
    size_t capacity;
} String;

/* Init a String object with a C string  */
/**
 * str_init:
 * @string: a #String object
 * @init_str: (nullable): a null-terminated C string to initialize with, or %NULL
 *
 * Initializes a String object with the contents of a C string.
 * If @init_str is %NULL, the string will be initialized as empty.
 *
 */
void str_init(String *string, const char *init_str, Arena *arena);

/**
 * str_insert_cstr_at:
 * @string: a #String object
 * @pos: position to insert at (0-based index)
 * @init_str: a null-terminated C string to insert
 *
 * Inserts a C string at the specified position in the String object.
 * All characters at and after @pos will be shifted to the right.
 *
 */
void str_insert_cstr_at(String *string, size_t pos, const char *init_str, Arena *arena);

/**
 * str_set_cstr:
 * @string: a #String object
 * @cstr: a null-terminated C string
 *
 * Sets the contents of the String object to the given C string,
 * replacing any existing content.
 *
 */
void str_set_cstr(String *string, const char *cstr);

/**
 * str_append_cstr:
 * @string: a #String object
 * @init_str: a null-terminated C string to append
 *
 * Appends the contents of a C string to the end of the String object.
 *
 */
void str_append_cstr(String *string, const char *init_str, Arena *arena);

/**
 * str_insert_at:
 * @string: a #String object
 * @pos: position to insert at (0-based index)
 * @src: a #String object to insert
 *
 * Inserts the contents of @src at the specified position in @string.
 * All characters at and after @pos will be shifted to the right.
 *
 */
void str_insert_at(String *string, size_t pos, String *src);

/**
 * str_set_at:
 * @string: a #String object
 * @index: the position of the character to set (0-based)
 * @ch: the character to set
 *
 * Sets the character at the specified index to the given character.
 * The @index must be within the current bounds of the string.
 *
 */
void str_set_at(String *string, size_t index, const char ch);

/**
 * str_append:
 * @dest: destination #String object
 * @src: source #String object to append
 *
 * Appends the contents of @src to the end of @dest.
 *
 */
void str_append(String *dest, String *src);

/**
 * str_set:
 * @dest: destination #String object
 * @src: source #String object
 *
 * Sets the contents of @dest to be a copy of @src,
 * replacing any existing content in @dest.
 *
 */
void str_set(String *dest, const String *src);

/**
 * str_substr:
 * @dest: destination #String object
 * @src: source #String object
 * @pos: starting position in @src (0-based index)
 * @length: number of characters to extract
 *
 * Extracts a substring from @src and stores it in @dest.
 * The substring starts at @pos and contains @length characters.
 * If @pos + @length exceeds the length of @src, only the available
 * characters are extracted.
 *
 */
void str_substr(String *dest, const String *src, size_t pos, size_t length, Arena *arena);

/**
 * str_reverse:
 * @string: a #String object
 *
 * Reverses the contents of the string in-place.
 *
 */
void str_reverse(const String *string);

/**
 * str_lower:
 * @string: a #String object
 *
 * Converts all characters in the string to lowercase in-place.
 * Only ASCII characters are affected.
 *
 */
void str_lower(String *string);

/**
 * str_upper:
 * @string: a #String object
 *
 * Converts all characters in the string to uppercase in-place.
 * Only ASCII characters are affected.
 *
 */
void str_upper(String *string);

/**
 * str_at:
 * @string: a #String object
 * @index: the position of the character to retrieve (0-based)
 *
 * Retrieves the character at the specified index.
 *
 * Returns: the character at @index, or '\0' if @index is out of bounds
 *
 */
char str_at(const String *string, size_t index);

/**
 * str_compare:
 * @string1: first #String object
 * @string2: second #String object
 *
 * Compares two strings lexicographically (case-sensitive).
 *
 * Returns: an integer less than, equal to, or greater than zero if
 *          @string1 is found, respectively, to be less than, to match,
 *          or to be greater than @string2
 *
 */
int str_compare(const String *string1, const String *string2);

/**
 * str_icompare:
 * @string1: first #String object
 * @string2: second #String object
 *
 * Compares two strings lexicographically in a case-insensitive manner.
 *
 * Returns: an integer less than, equal to, or greater than zero if
 *          @string1 is found, respectively, to be less than, to match,
 *          or to be greater than @string2 (ignoring case)
 *
 */
int str_icompare(const String *string1, const String *string2);

/**
 * str_free:
 * @string: a #String object
 *
 * Frees the memory allocated for the String object and sets it to
 * an empty state. After calling this function, the string can be
 * reused with str_init() or other functions.
 *
 */
void str_free(String *string);

#endif
