#ifndef __PAGE_H
#define __PAGE_H

#include <stddef.h>
#include <address.h>
#include <assert.h>

/* 页表项 抽象数据结构 */
typedef uintptr_t pte_t;
typedef uint8_t pte_flags_t;

#define PTE_V 0x001 // Valid
#define PTE_R 0x002 // Read
#define PTE_W 0x004 // Write
#define PTE_X 0x008 // Execute
#define PTE_U 0x010 // User
#define PTE_G 0x020 // Global
#define PTE_A 0x040 // Accessed
#define PTE_D 0x080 // Dirty

// 生成一个新的pte（物理页号和标志位结合）
pte_t pte_new(phys_page_num_t, pte_flags_t);
//获取页号、地址、标志位
phys_page_num_t pte_get_ppn(pte_t pte);
phys_addr_t pte_get_pa(pte_t pte);
pte_flags_t pte_get_flags(pte_t pte);
//清空
void pte_empty(pte_t* pte);
// 判断V、R、W、X
int pte_is_valid(pte_t pte);
int pte_readable(pte_t pte);
int pte_writable(pte_t pte);
int pte_excutable(pte_t pte);
/* 通过pnn返回页表项数组 , 用于根据当前页索引找到页表项 */
pte_t *get_pte_array(phys_page_num_t ppn);
uint8_t *get_byte_array(phys_page_num_t ppn);

#endif