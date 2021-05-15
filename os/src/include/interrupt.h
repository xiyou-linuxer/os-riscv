#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <stddef.h>

#define STIMER_INTR_NUM 5
#define SSOFTWARE_INTR_NUM 1
#define SEXTERN_INTR_NUM 9

typedef void* intr_handler;

typedef enum intr_status {
    INTR_OFF,
    INTR_ON
}intr_status_t;

enum intr_flag {
    syn_interrupt_flag,
    asyn_interrupt_flag
};

void register_handler(enum intr_flag flag, uint64_t vector_no, intr_handler function);
void idt_init();
intr_status_t intr_get_status();
intr_status_t intr_set_status(intr_status_t);
intr_status_t intr_enable(void);
intr_status_t intr_disable(void);

#endif
