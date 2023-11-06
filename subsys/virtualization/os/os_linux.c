/*
 * Copyright 2021-2022 HNU
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sys/mem_manage.h>

#include <virtualization/zvm.h>
#include <virtualization/vm_manager.h>
#include <virtualization/os/os_linux.h>



static atomic_t zvm_linux_image_map_init = ATOMIC_INIT(0);
static uint64_t zvm_linux_image_map_phys = 0;

/**
 * @brief Establish a mapping between the zephyr image addresses 
 *      and virtual addresses
 */
static uint64_t zvm_mapped_linux_image()
{
    uint8_t *ptr;
    uintptr_t phys;
    size_t size;
    uint32_t flags;
    if(likely(!atomic_cas(&zvm_linux_image_map_init,0,1))){
        return zvm_linux_image_map_phys;
    }

    phys = LINUX_VM_MEM_BASE;
    size = LINUX_VM_IMG_SIZE;
    flags = K_MEM_CACHE_NONE | K_MEM_PERM_RW | K_MEM_PERM_EXEC;
    z_phys_map(&ptr,phys,size,flags);
    zvm_linux_image_map_phys = (uint64_t)ptr;
    return zvm_linux_image_map_phys;
}


/**
 * @brief load linux image from other memory address to allocated address
 */
int load_linux_image(struct vm_mem_domain *vmem_domain)
{
    char *dbuf, *sbuf;
    ARG_UNUSED(dbuf);
    ARG_UNUSED(sbuf);
    int ret = 0;
    uint64_t base_addr,base_size,image_base,image_size,phys,entry;
    struct _dnode *d_node, *ds_node;
    struct vm_mem_partition *vpart;
    struct vm_mem_block *blk;
    struct vm *this_vm = vmem_domain->vm;
    ARG_UNUSED(blk);
    ARG_UNUSED(this_vm);

#ifndef  CONFIG_VM_DYNAMIC_MEMORY
    ARG_UNUSED(this_vm);
    return ret;
#endif /* CONFIG_VM_DYNAMIC_MEMORY */

    /*Find the zephyr image base_addr and it size here */
    base_addr = LINUX_VM_MEM_BASE;
    base_size =  LINUX_VM_MEM_SIZE;
    image_base = zvm_mapped_linux_image();
    image_size = LINUX_VM_IMG_SIZE;
    entry = LINUX_VMSYS_ENTRY;
    SYS_DLIST_FOR_EACH_NODE_SAFE(&vmem_domain->mapped_vpart_list,d_node,ds_node){
        vpart = CONTAINER_OF(d_node,struct vm_mem_partition,vpart_node);
        if(vpart->part_hpa_base == base_addr){
            vpart->part_kpa_base = k_malloc(image_size+CONFIG_MMU_PAGE_SIZE);
            phys = ROUND_UP(vpart->part_kpa_base, CONFIG_MMU_PAGE_SIZE);
            phys = z_mem_phys_addr(phys);
            memcpy(phys, image_base, image_size);
            arch_mmap_vpart_to_block(vmem_domain->vm_mm_domain, phys,
                                    entry, image_size, MT_VM_NORMAL_MEM, false, this_vm->vmid);
            break;
        }
    }

    return ret;

}
