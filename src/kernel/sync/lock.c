#include <lib/libc/types.h>
#include <kernel/init/hal.h>
#include <kernel/mm/heap.h>
#include <kernel/sync/lock.h>

/* Create a spinlock and initialize its values */
spinlock_t *spinlock_create()
{
	spinlock_t *lock = (spinlock_t*) kmalloc(sizeof(spinlock_t));
	spinlock_init(lock);
	return lock;
}

/* Initialize a spinlock's values */
void spinlock_init(spinlock_t *lock)
{
	atomic_set(&lock->value, 0);
	lock->interrupts;
}

/* Delete a spinlock */
void spinlock_delete(spinlock_t *lock)
{
	kfree(lock);
}

/* Acquire a spinlock */
void spinlock_acquire(spinlock_t *lock)
{
	uint32_t interrupts = get_interrupt_state();

	disable_interrupts();
	while (atomic_cmpxchg(&lock->value, 0, 1) != 0);

	lock->interrupts = interrupts;
}

/* Release a spinlock */
void spinlock_release(spinlock_t *lock)
{
	while (atomic_cmpxchg(&lock->value, 1, 0) == 1);	// Does not seem right: Come back to this
	set_interrupt_state(lock->interrupts);
}