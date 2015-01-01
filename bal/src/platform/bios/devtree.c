#include <types.h>
#include <list.h>
#include <dict.h>
#include <device/device.h>
#include <device/devtree.h>

/* Device tree root operations */
static device_ops_t root_ops =
{
	.init = NULL;
	.shutdown = NULL;
	.enumerate = &root_enumerate;
};

/* Initialize the device tree */
void devtree_init()
{
	/* Create the root of the device tree and enumerate it */
	device_t *root = devtree_root();
	root->ops = &root_ops;
	root->type = DEVICE_OTHER;
	root->children = list_create();
	root->num_children = 0;
	device_enumerate(root);
}