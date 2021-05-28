#include <frame_allocator.h>
#include <page_table.h>
#include <address.h>
#include <max_heap.h>

struct frame_allocator FRAME_ALLOCATOR;

void frame_allocator_init()
{
    FRAME_ALLOCATOR.current = pa_ceil((phys_addr_t)&ekernel + PAGE_SIZE);
    FRAME_ALLOCATOR.end = pa_floor((phys_addr_t)MEMORY_END);
    uint64_t page_nums = FRAME_ALLOCATOR.end - FRAME_ALLOCATOR.current;

    heap_init(&FRAME_ALLOCATOR.recycled, NULL , page_nums);
}

phys_page_num_t frame_alloc_(struct frame_allocator* self)
{
    phys_page_num_t re = 0;
    if(!heap_empty(&self->recycled)) {
        phys_page_num_t t = (phys_page_num_t)self->recycled.heapArray[self->recycled.CurrentSize-1];
        self->recycled.CurrentSize--;
        re = t;
    } else {
        if(self->current == self->end) {
            panic("no frame to alloc!\n");
        } else {
            self->current++;
            re = self->current - 1;
        }
    }
    return re;
}