#include <exception.h>
#include <init.h>
#include <stddef.h>
#include <kernel-stdio.h>
#include <string.h>
#include <timer.h>
#include <assert.h>
#include <sbi.h>
#include <frame_allocator.h>

int i;
extern uintptr_t skernel;
extern uintptr_t stext;
extern uintptr_t etext;
extern uintptr_t srodata;
extern uintptr_t erodata;
extern uintptr_t sdata;
extern uintptr_t edata;
extern uintptr_t sbss;
extern uintptr_t ebss;
extern uintptr_t boot_stack;
extern uintptr_t boot_stack_top;
extern uintptr_t ekernel;

void main()
{
	memset(&sbss, 0, &ebss - &sbss); //清空bss段
	init_all();

	printk("%s", "hello, world!\n");
	
	printk("kernel_start : 		[%p]\n", &skernel);
	printk(".text : 		[%p ~ %p]\n", &stext, &etext);
	printk(".rodata : 		[%p ~ %p]\n", &srodata, &erodata);
	printk(".data : 		[%p ~ %p]\n", &sdata, &edata);
	printk("boot_stack : 		[%p ~ %p]\n", &boot_stack, &boot_stack_top);
	printk(".bss : 			[%p ~ %p]\n", &sbss, &ebss);
	printk("kernel_end: 		[%p]\n", &ekernel);

	mm_init();
	frame_test();

	idt_init();
	irq_enable();
	asm volatile ("ebreak");
	printk(" ? \n");  //

	panic("goodbye %d\n", 1);


	//timer_init();
	//printf(" ? ");
	
	while(1) {}
}