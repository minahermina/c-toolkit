/*
    Copyright (C) 2025  Mina Albert Saeed <mina.albert.saeed@gmail.com>

    A an Arena based memory allocator that uses mmap for efficient arena-based memory management.

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

#ifndef ARENA_LIB
#define ARENA_LIB

#include <unistd.h>
#include <pthread.h>

typedef struct Region Region;

struct Region{
    Region *next;
    size_t capacity;
    size_t count;
    size_t remaining;
    unsigned char *bytes;
};

typedef struct {
    Region *head;
    Region *tail;
    pthread_mutex_t mutex;
} Arena;


#define ARENA_ARR(name, type) \
    typedef struct name { \
        type *items; \
        size_t size; \
        size_t capacity; \
    } name

#define ARENA_REGION_SIZE        (sizeof(Region))
#define ARENA_PAGE_SIZE          (sysconf(_SC_PAGESIZE))
#define ARENA_SIZE_ARR(arr)      (sizeof(arr) / sizeof((arr)[0]))

#define ARENA_REGION_DEFAULT_CAPACITY   (ARENA_PAGE_SIZE * 2)


#define ARENA_ARR_INIT_CAPACITY 256

#define arena_arr_append(arena, arr, item) \
    do{ \
        if((arr)->size >= (arr)->capacity) { \
            size_t new_capacity = (arr)->capacity == 0 ? ARENA_ARR_INIT_CAPACITY : (arr)->capacity*2;  \
            (arr)->items = arena_realloc(arena, \
                                         (arr)->items, \
                                         (arr)->capacity*sizeof(*(arr)->items), \
                                         new_capacity*sizeof(*(arr)->items)); \
            (arr)->capacity = new_capacity; \
        } \
        (arr)->items[(arr)->size] = item;\
        (arr)->size++; \
    } while(0)

/* Functions declarations*/
void arena_init(Arena *arena, size_t size);
void *arena_alloc(Arena *arena, size_t size);
void *arena_realloc(Arena *arena, void *oldptr, size_t oldsz, size_t newsz);
size_t arena_strlen(const char *str); /* this is implemented  instead of including <string.h>*/
void *arena_memcpy(void *dest, const void *src, size_t n); /* just like arena_strlen*/
void arena_dump(Arena *arena);

/* Must be used only when no other threads are using the arena*/
void arena_reset(Arena *arena);
void arena_destroy(Arena *arena);

#endif
