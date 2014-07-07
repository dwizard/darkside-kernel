#include <types.h>
#include <microkernel/interrupt.h>
#include <microkernel/i686/idt.h>
#include <mm/slab.h>

/* Registered interrupt object list */
static interrupt_t *interrupts[224];

/* ASM interrupt code template */
void asm_interrupt_code_template();

/* Interrupt object slab cache */
static slab_cache_t *interrupt_cache;

/* ASM interrupt stub slab cache */
static slab_cache_t *asm_interrupt_stub_cache;

/* Create an interrupt object */
interrupt_t *interrupt_create()
{
	interrupt_t *interrupt = slab_cache_alloc(interrupt_cache);
}

/* Destroy an interrupt object */
void interrupt_destroy(interrupt_t *interrupt)
{
	slab_cache_free(interrupt_cache, interrupt);
}

/* Register an interrupt object and its handler */
void interrupt_register_handler(interrupt_t *interrupt, interrupt_handler_t handler)
{
	/* Fill in the handler */
	interrupt->handler = handler;

	/* Insert the interrupt object into the registered interrupt list */
	int offset = interrupt->vector - 32;
	if (interrupts[offset])
	{
		/* Iterate through the linked list of interrupt objects */
		interrupt_t *head = interrupts[offset];
		while (head->next)
		{
			head = head->next;
		}

		/* Place the interrupt object at the end of the linked list */
		head->next = interrupt;
	}
	else
	{
		interrupts[offset] = interrupt;
	}

	/* Allocate an ASM interrupt stub, copy in the template, and modify it */

	/* Register the handler with the interrupt controller */
	(*interrupt->controller)->irq_register_handler(interrupt->controller, interrupt);
}

/* Initialize the interrupt manager */
void interrupts_init()
{
	/* Calculate the size of the ASM interrupt stub template */

	/* Create the interrupt object and ASM interrupt code slab caches */
	interrupt_cache = slab_cache_create(sizeof(interrupt_t), PAGE_READ | PAGE_WRITE);
	asm_interrupt_stub_cache = slab_cache_create(32, PAGE_READ | PAGE_WRITE | PAGE_EXECUTE);
}
