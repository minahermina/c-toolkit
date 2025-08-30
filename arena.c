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

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "arena.h"

static Region*
arena_new_region(size_t size)
{
    Region *region;
    void *ptr;

    ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assert(ptr != MAP_FAILED);

    region             = (Region*) ptr;
    region->next       = NULL;
    region->capacity   = size - ARENA_REGION_SIZE;
    region->remaining  = size - ARENA_REGION_SIZE;
    region->count      = 0;
    region->bytes      = ((unsigned char*)ptr) + ARENA_REGION_SIZE;

    return region;
}

static size_t
arena_align_size(size_t size)
{
    size_t size_page_aligned, page_size, region_size, size_bytes;
    region_size = ARENA_REGION_SIZE;
    page_size = sysconf(_SC_PAGESIZE);
    size_bytes = region_size + size;
    size_page_aligned = (size_bytes + page_size - 1) & ~(page_size - 1);
    return size_page_aligned;
}

static void
arena_append_region(Arena *arena, size_t size)
{
    Region *region;
    if(size < (size_t)ARENA_REGION_DEFAULT_CAPACITY){
        size = ARENA_REGION_DEFAULT_CAPACITY;
    } else{
        size = arena_align_size(size);
    }
    region = arena_new_region(size);
    arena->tail->next = region;
    arena->tail = region;
}

/* This must be called at the beginning of the lifetime to initialize the arena*/
void
arena_init(Arena *arena, size_t size)
{
    Region *region;
    int ret;
    size = arena_align_size(size);
    region = arena_new_region(size);

    arena->head = region;
    arena->tail = region;

    /* Init the mutex */
    ret = pthread_mutex_init(&arena->mutex, NULL);
    assert(ret == 0);
}

static void*
arena_alloc_unlocked(Arena *arena, size_t size)
{
    Region *curr;
    void *ptr;

    assert(arena != NULL);
    assert(arena->head != NULL);

    for(curr = arena->head; curr != NULL; curr = curr->next ){
        if(size <= curr->remaining) {
            ptr = (void*)(curr->bytes + curr->count);
            curr->count      += size;
            curr->remaining  -= size;

            return ptr;
        }
    }

    // Allocate new region as no space available
    arena_append_region(arena, size);
    curr = arena->tail;
    ptr = (void*)(curr->bytes + curr->count);

    return ptr;
}


void*
arena_alloc(Arena *arena, size_t size)
{
    void *ptr;
    int ret;

    assert(arena != NULL);
    assert(arena->head != NULL);

    /* Locking the mutex */
    ret = pthread_mutex_lock(&arena->mutex);
    assert(ret == 0);

    ptr = arena_alloc_unlocked(arena, size);

    /* Unlocking the mutex */
    ret = pthread_mutex_unlock(&arena->mutex);
    assert(ret == 0);

    return ptr;
}

size_t
arena_strlen(const char *str)
{
    size_t sz = 0;
    while(*str){
        sz++;
        str++;
    }
    return sz;
}

void *
arena_memcpy(void *dest, const void *src, size_t n)
{
    const char *_src = src;
    char *_dest = dest;
    for( ; n !=0; n--){
        *_dest = *_src;
        _dest++;
        _src++;
    }
    return _dest;
}


/*
    Memory in the arena allocator is managed in a linear fashion,
    meaning previously allocated blocks cannot be individually freed or reused.
    When reallocating, a new block is allocated, and the old block remains unused,
    effectively making it "orphaned." This can lead to increased memory usage over time.

    TODO: Find a better approach to handle reallocations
*/
void *
arena_realloc(Arena *arena, void *old_ptr, size_t old_size, size_t new_size)
{
    unsigned char *new_ptr;
    size_t i;
    int ret;
    assert(arena != NULL);

    if(new_size < old_size)
        return old_ptr;

    /* Locking the mutex */
    ret = pthread_mutex_lock(&arena->mutex);
    assert(ret == 0);

    new_ptr = (unsigned char*)arena_alloc_unlocked(arena, new_size);

    if(old_ptr != NULL){
        unsigned char * old_ptr_char = (unsigned char*)old_ptr;
        for(i = 0; i < old_size; ++i){ /*Assuming no overlap happens*/
            new_ptr[i] = old_ptr_char[i];
        }
    }

    /* Unlocking the mutex */
    ret = pthread_mutex_unlock(&arena->mutex);
    assert(ret == 0);
    return (void*) new_ptr;
}

static void
arena_region_dump(Region* region)
{
    assert(region != NULL);
    printf("Address:    %p\n", (void*)region);
    printf("Starts at:  %p\n", (void*)region->bytes);
    printf("Next:       %p\n", (void*)region->next);
    printf("Capacity:   %zu bytes\n", region->capacity);
    printf("Used:       %zu bytes\n", region->count);
    printf("Free:       %zu bytes\n", region->remaining);
    printf("\n");
}

void
arena_dump(Arena *arena)
{
    Region *curr;
    size_t cnt = 0;

    assert(arena != NULL);

    printf("=============================\n");
    for(curr = arena->head; curr != NULL; curr = curr->next ){
        printf("===> Region %zu:\n", cnt);
        arena_region_dump(curr);
        cnt++;
    }
    printf("=============================\n");
    printf("\n");
}

void
arena_reset(Arena *arena){
    Region *curr;
    int ret;
    assert(arena != NULL);

    for(curr = arena->head; curr != NULL; curr = curr->next){
        curr->count = 0;
        curr->remaining = curr->capacity;
    }

    /* Safe to destroy - no other threads should be using it */
    ret = pthread_mutex_destroy(&arena->mutex);
    assert(ret == 0);

    /* Re-initialize the mutex for future use */
    ret = pthread_mutex_init(&arena->mutex, NULL);
    assert(ret == 0);
}

static void
arena_free_region(Region* region)
{
    assert(region != NULL);
    size_t size_bytes = ARENA_REGION_SIZE + region->capacity;
    int ret = munmap(region, size_bytes);
    assert(ret == 0);
}

void
arena_destroy(Arena *arena)
{
    Region* curr, *temp;
    int ret;

    for(curr = arena->head; curr != NULL;){
        temp = curr;
        curr = curr->next;
        arena_free_region(temp);
    }
    arena->head = NULL;
    arena->tail = NULL;

    /* Safe to destroy - no other threads should be using it */
    ret = pthread_mutex_destroy(&arena->mutex);
    assert(ret == 0);
}
