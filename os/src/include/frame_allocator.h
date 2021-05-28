#ifndef __FRAME_ALLOCATOR_H
#define __FRAME_ALLOCATOR_H

#include <address.h>
#include <page_table.h>
#include <max_heap.h>
#include <stddef.h>
#include <bitmap_buddy.h>

#define KERNEL_HEAP_SIZE (4096 *32)

extern uintptr_t ekernel;

struct frame_allocator
{
    phys_page_num_t current;
    phys_page_num_t end;
    struct maxHeap recycled;
};

extern struct bitmap_buddy* HEAP_ALLOCATOR;

phys_page_num_t frame_alloc(struct frame_allocator* self);
void frame_dealloc(struct frame_allocator* self, phys_page_num_t ppn);
uint64_t frame_remain_size(struct frame_allocator* self);
void frame_test();

void heap_allocator_init();
void frame_allocator_init();

static inline void mm_init()
{
    heap_allocator_init();
    frame_allocator_init();
    // page_activate();
}

#endif