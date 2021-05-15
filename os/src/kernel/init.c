#include <init.h>
#include <string.h>

void init_all()
{
    // memset(&sbss, 0, &ebss - &sbss); //清空bss段
    //开启页模式
    //page_init();
    //初始化中断
    // idt_init();
    return;
}