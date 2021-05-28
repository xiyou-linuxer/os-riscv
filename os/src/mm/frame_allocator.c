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

phys_page_num_t frame_alloc(struct frame_allocator* self)
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

void frame_dealloc(struct frame_allocator *self, phys_page_num_t ppn)
{
    //printk("free page :[%p]\n", ppn);
    if ((uint64_t)ppn >= self->current || is_some(&self->recycled, (uint64_t)ppn))
        panic("Frame ppn={%p} has not been allocated!\n", ppn);
    heap_insert(&self->recycled, (uint64_t)ppn);

    while (heap_top(&self->recycled) == (uint64_t)self->current - 1) {
        heap_removeMax(&self->recycled);
        self->current--;
    }
}

uint64_t frame_remain_size(struct frame_allocator *self)
{
    return self->end - self->current + self->recycled.CurrentSize;
}

void frame_test()
{
    phys_page_num_t p1 = frame_alloc(&FRAME_ALLOCATOR);
    printf("alloc page p1 :[%p]\n", p1);
    phys_page_num_t p2 = frame_alloc(&FRAME_ALLOCATOR);
    printf("alloc page p2 :[%p]\n", p2);
    phys_page_num_t p3 = frame_alloc(&FRAME_ALLOCATOR);
    printf("alloc page p3 :[%p]\n", p3);
    phys_page_num_t p4 = frame_alloc(&FRAME_ALLOCATOR);
    printf("alloc page p4 :[%p]\n", p4);
    phys_page_num_t p5 = frame_alloc(&FRAME_ALLOCATOR);
    printf("alloc page p5 :[%p]\n", p5);
    phys_page_num_t p6 = frame_alloc(&FRAME_ALLOCATOR);
    printf("alloc page p6 :[%p]\n", p6);

    frame_dealloc(&FRAME_ALLOCATOR, p4);    
    frame_dealloc(&FRAME_ALLOCATOR, p3);    
    // frame_dealloc(&FRAME_ALLOCATOR, p6);    
    frame_dealloc(&FRAME_ALLOCATOR, p5);    

    phys_page_num_t p7 = frame_alloc(&FRAME_ALLOCATOR);
    printf("alloc page :[%p]\n", p7);
    phys_page_num_t p8 = frame_alloc(&FRAME_ALLOCATOR);
    printf("alloc page :[%p]\n", p8);

    // frame_dealloc(&FRAME_ALLOCATOR, p4);    


}
