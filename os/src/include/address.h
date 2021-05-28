#ifndef __MM_H
#define __MM_H

#include <stddef.h>
#include <assert.h>

#define PAGE_SIZE 4096L
#define PAGE_OFFSET_BITS 12L
#define MEMORY_END 0x80800000L   // 可用内存的结束位置

/* 物理和虚拟 地址和页号 */
typedef uintptr_t phys_addr_t;
typedef uintptr_t phys_page_num_t;
typedef uintptr_t virt_addr_t;
typedef uintptr_t virt_page_num_t;

/* 根据地址得到页内偏移 */
static inline uintptr_t get_offset(uintptr_t addr)
{
    return addr & (PAGE_SIZE - 1);
}
/* 判断是否4K对齐 */
static inline bool is_aligned(uintptr_t addr)
{
    return get_offset(addr) == 0;
}

/* floor向下 ceil向上 */
static inline phys_page_num_t pa_floor(phys_addr_t phy_addr)
{
    return phy_addr / PAGE_SIZE;
}
static inline phys_page_num_t pa_ceil(phys_addr_t phy_addr)
{
    return (phy_addr + PAGE_SIZE - 1) / PAGE_SIZE;
}
static inline virt_page_num_t va_floor(virt_addr_t virt_addr)
{
    return pa_floor((phys_addr_t)virt_addr);
}
static inline virt_page_num_t va_ceil(virt_addr_t virt_addr)
{
    return pa_ceil((phys_addr_t)virt_addr);
}

/* 页号和地址的相互转换 */
static inline phys_page_num_t pa2ppn(phys_addr_t pa)
{
    //assert(get_offset(pa) == 0);
    return pa_floor(pa);
}
static inline phys_addr_t ppn2pa(phys_page_num_t ppn)
{
    return ppn << PAGE_OFFSET_BITS;
}
static inline virt_page_num_t va2vpn(virt_addr_t va)
{
    return pa2ppn((phys_addr_t)va);
}
static inline virt_addr_t vpn2va(virt_page_num_t vpn)
{
    return ppn2pa((phys_page_num_t)vpn);
}

#endif
