#ifndef __DEVICE_H
#define __DEVICE_H

/* Device type */
#define DEVICE_STORAGE		0
#define DEVICE_NETWORK		1
#define DEVICE_OTHER		0xFFFFFFFF

/* Enumeration statuses */
#define ENUMERATE_LEAF		0
#define ENUMERATE_SUCCESS	1
#define ENUMERATE_DONE		2

/* Device operations */
struct device;
typedef struct device_ops
{
	/* Enumerate the device's children */
	uint32_t (*enumerate)(struct device *device, struct device **out);
} device_ops_t;

/* Device structure */
typedef struct device
{
	/* Device operations */
	device_ops_t *ops;

	/* Type of device */
	uint32_t type;

	/* Child devices */
	list_t children;
} device_t;

/* Enumerate a device's children */
uint32_t device_enumerate(device_t *device, device_t **out);

#endif
