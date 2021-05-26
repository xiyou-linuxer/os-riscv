#include <page_table.h>
#include <string.h>

pte_t pte_new(phys_page_num_t ppn, pte_flags_t flags)
{
    uint64_t pte = (ppn << 10) | (uint64_t)flags;
    return pte;
}

// 获取页号、地址、标志位
phys_page_num_t pte_get_ppn(pte_t pte)
{
    uint64_t t = ((uint64_t)1 << 44) - 1;
    return (pte >> 10) & t;
}

phys_addr_t pte_get_pa(pte_t pte)
{
    return ppn2pa(pte_get_ppn(pte));
}

pte_flags_t pte_get_flags(pte_t pte)
{
    return (uint8_t)(pte & 0x0FF);
}

// 清空
void pte_empty(pte_t* pte)
{
    *pte = 0;
}

// 判断V R W X
bool pte_is_valid(pte_t pte)
{
    return (pte_get_flags(pte) & PTE_V) != 0;
}
bool pte_readable(pte_t pte)
{
    return (pte_get_flags(pte) & PTE_R) != 0;
}
bool pte_writable(pte_t pte)
{
    return (pte_get_flags(pte) & PTE_W) != 0;
}
bool pte_excutable(pte_t pte)
{
    return (pte_get_flags(pte) & PTE_X) != 0;
}

/* 访问一个特定物理页帧方法：先把物理页号转为物理地址 PhysAddr ，然后再转成 usize 形式的物理地址 */
/* 通过ppn返回页表项数组 , 用于根据当前页索引找到页表项 */
pte_t *get_pte_array(phys_page_num_t ppn) // 8字节粒度
{
    return (void *)ppn2pa(ppn);
}
uint8_t *get_byte_array(phys_page_num_t ppn) // 1字节粒度
{
    return (void *)ppn2pa(ppn);
}