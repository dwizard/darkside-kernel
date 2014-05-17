#include <types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iterator.h>
#include <list.h>
#include <init/loader.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <storage/storage.h>
#include <fs/fs.h>
#include <executable/executable.h>

/* Boot Application main function */
void ba_main(loader_block_t *loader_block)
{
	/* Mount the boot device */
	int status = fs_mount(storage_get_boot_device(), "/", "ext2");
	if (status != 0)
	{
		panic("Failed to mount boot device, error %d\n", status);
	}

	/* Read and parse the configuration file */

	/* Initialize graphics
	framebuffer_t *fb = graphics_init(0, 0, 0);
	loader_block->fb = fb; */

	/* Load the kernel into virtual memory */
	executable_t *kernel = elf_executable_load_executable("/boot/kernel-i386.elf");

	/* Load the Hardware Abstraction Layer into memory */

	/* Load bootvid into virtual memory */

	/* Read and parse the module registry */

	/* Load the boot modules into memory */

	/* Make a temporary copy of the memory map */
	list_t phys_mem_map_copy = list_create();
	iterator_t iter = list_head(loader_block->phys_mem_map);

	mem_map_entry_t *entry = (mem_map_entry_t*) iter.now(&iter);
	while (entry)
	{
		/* Create a new entry */
		mem_map_entry_t *new = (mem_map_entry_t*) malloc(sizeof(mem_map_entry_t));
		new->base = entry->base;
		new->length = entry->length;
		new->flags = entry->flags;
		new->numa_domain = entry->numa_domain;

		/* Insert it into the list */
		list_insert_tail(&phys_mem_map_copy, new);

		/* Go to the next entry */
		entry = (mem_map_entry_t*) iter.next(&iter);
	}

	/* Allocate space for the PFN database */
	vaddr_t pfn_database = kernel->end;
	loader_block->pfn_database = pfn_database;

	iter = list_head(&phys_mem_map_copy);

	entry = (mem_map_entry_t*) iter.now(&iter);
	mem_map_entry_t *next = entry;
	while (entry)
	{
		/* How much space is needed? */
		uint64_t needed_space = (entry->length / 0x1000) * /*sizeof(page_t)*/ 24;

		/* If the block refers to actual memory, allocate PFN structures for it */
		if (entry->flags)
		{
			/* Try to get to a page boundary */
			uint32_t to_next_page = 0;
			if (pfn_database & 0xFFF)
			{
				to_next_page = 0x1000 - (pfn_database & 0xFFF);
			}
			pfn_database += to_next_page;
			needed_space -= to_next_page;

			/* If needed, allocate the page before */
			if (to_next_page && get_mapping(pfn_database - to_next_page) == -1)
			{
				map_page(pfn_database - to_next_page, pmm_alloc_page(), PAGE_READ | PAGE_WRITE);
			}

			/* Allocate the space */
			for (vaddr_t i = pfn_database; i < pfn_database + needed_space; i += 0x1000)
			{
				map_page(i, pmm_alloc_page(), PAGE_READ | PAGE_WRITE);
			}

			/* Clear the rest */
			uint32_t space_remaining = 0;
			if ((pfn_database + needed_space) & 0xFFF)
			{
				space_remaining = 0x1000 - ((pfn_database + needed_space) & 0xFFF);
			}
			memset((void*) pfn_database + needed_space, 0, space_remaining);
		}

		/* Increment the PFN database pointer by the needed space */
		pfn_database += needed_space;

		/* Go to the next entry */
		next = (mem_map_entry_t*) iter.next(&iter);

		if (!next)
		{
			break;
		}
		else
		{
			entry = next;
		}
	}

	loader_block->phys_mem_size = (paddr_t) entry->base + entry->length;

	/* Call the kernel, passing it the loader block */
	bal_enter_kernel(kernel->entry_point, loader_block);

	while(1);
}
