#include <interrupt.h>
#include <riscv_asm.h>
#include <console.h>
#include <stdio.h>
#include <timer.h>
#include <syscall.h>
#include <assert.h>

#define SYN_INTR_CNT 0x20   //支持的同步中断个数
#define ASYN_INTR_CNT 0x20   //支持的异步中断个数

char* syn_intr_name[SYN_INTR_CNT];   //同步中断名
char* asyn_intr_name[ASYN_INTR_CNT];   //异步中断名
// intr_handler syn_intr_table[SYN_INTR_CNT];   //同步中断处理函数表
void (*syn_intr_table[SYN_INTR_CNT])(struct context*f);
intr_handler asyn_intr_table[ASYN_INTR_CNT];   //异步中断处理函数表

extern void __saveall(void);
extern void __restore(void);
void breakpoint(struct context *f);

/**
 * general_intr_handler - 通用中断处理函数
 * **/
static void general_intr_handler(struct context* f)
{
    printf("\n!!!!!!   exception message begin   !!!!!!\n");
    printf("\n%s\n", syn_intr_name[(intptr_t)f->cause]);
    //若为pagefault，将确实的地址打印出来并悬停
    printf("\n!!!!!!   exception message end   !!!!!!\n");
    //能进入中断处理程序就表示已经处在关中断情况下，不会出现调度进程的情况。故下面的死循环不会再被中断
    panic("\n!!!!!!   exception   !!!!!\n");
}

static void syn_exception_init()
{
    int i;
    for(i = 0; i < SYN_INTR_CNT; i++) {
        syn_intr_name[i] = "unknown";
        syn_intr_table[i] = general_intr_handler;
    }
    syn_intr_name[0] = "Instruction address misaligned";
    syn_intr_name[1] = "Instruction access fault";
    syn_intr_name[2] = "Illegal instruction";
    syn_intr_name[3] = "Breakpoint";
    syn_intr_name[4] = "Load address misaligned";
    syn_intr_name[5] = "Load access fault";
    syn_intr_name[6] = "Store address misaligned";
    syn_intr_name[7] = "Store access fault";
    syn_intr_name[8] = "Environment call from U-mode";
    syn_intr_name[9] = "Environment call from S-mode";
    syn_intr_name[11] = "Environment call from M-mode";
    syn_intr_name[12] = "Instruction page fault";
    syn_intr_name[13] = "Load page fault";
    syn_intr_name[15] = "Store page fault";
}

static void asyn_exception_init()
{
    int i;
    for(i = 0; i < ASYN_INTR_CNT; i++) {
        asyn_intr_name[i] = "unknown";
        asyn_intr_table[i] = general_intr_handler;
    }
    asyn_intr_name[1] = "Supervisor software interrupt";
    asyn_intr_name[3] = "Machine software interrupt";
    asyn_intr_name[5] = "Supervisor timer interrupt";
    asyn_intr_name[7] = "Machine timer interrupt";
    asyn_intr_name[9] = "Supervisor external interrupt";
    asyn_intr_name[11] = "Machine external interrupt";
}

void idt_init(void)
{
    syn_exception_init();
    asyn_exception_init();
    register_handler(syn_interrupt_flag, EXC_BREAKPOINT, breakpoint);
    csr_write(CSR_SSCRATCH, 0);
    csr_write(CSR_STVEC, &__saveall);   // 由于函数地址四字节对其，所以设置后模式为Direct
    printf("set stvec : [%p]\n", &__saveall);
    printf("set restore : [%p]\n", &__restore);
}

// intr_status_t intr_get_status()
// {
    // uint64_t sstatus_value;
    // sstatus_value = csr_read(sstatus);
    // !!!!!!----------------------------------
// }

void irq_enable(void)
{
    csr_set(CSR_SSTATUS, SSTATUS_SIE);
}

void irq_disable(void)
{
    csr_clear(CSR_SSTATUS, SSTATUS_SIE);
}

void breakpoint(struct context *f)
{
    printf("Breakpoint at [%p]!\n", f->epc);
    f->epc += 2; //?
}

static inline void exception_dispatch(struct context *f)
{
    if ((intptr_t)f->cause < 0) {
        cons_puts("interrupt\n");
        printf("I code :[%p]\n", f->cause);
        irq_handler(f);
    } else {
        cons_puts("sync_exception\n");
        printf("E code :[%p]\n", f->cause);
        exc_handler(f);
    }
}

struct context *e_dispatch(struct context *f)
{
    exception_dispatch(f);
    return f;                   // 用于__restore 的 mov sp,a0
}

void irq_handler(struct context *f)
{
    intptr_t cause = (f->cause << 1) >> 1;     //去掉最高位
    switch (cause) {
        case IRQ_S_TIMER:
            set_next_trigger();     //设置下一次时钟中断
            if (++TICKS % TICKS_PER_SEC == 0) {
                printf("it's time to sleep\n");
            }
            break;
        default:
            break;
    }
    //printf("done! \n");
    //f->epc = (uintptr_t)&__restore;
    //printf("sepc: [%p]\n", f->epc);
}

void exc_handler(struct context *f)
{
    syn_intr_table[f->cause](f);
    // switch (f->cause) {
    //     case EXC_BREAKPOINT:
    //         // breakpoint(f);
    //         syn_intr_table[EXC_BREAKPOINT](f);
    //         break;
    //     case EXC_SYSCALL:
    //         syscall_handler(f);
    //         break;
    //     default:
    //         break;
    // }
}

/*在中断处理程序数组第vector_no个元素中注册安装中断处理程序function*/
void register_handler(enum intr_flag flag, uint64_t vector_no, intr_handler function)
{
    if(flag == syn_interrupt_flag) {
        //idt_table数组中的函数是在进入中断后根据中断向量号调用的
        syn_intr_table[vector_no] = function;
    } else {
        asyn_intr_table[vector_no] = function;
    }
}
