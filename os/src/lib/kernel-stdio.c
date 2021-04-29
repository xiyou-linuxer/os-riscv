#include <kernel-stdio.h>
#include <stdio.h>

void printk(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    
    vprintf(fmt, ap);

    va_end(ap);
}
