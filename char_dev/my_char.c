#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/ioctl.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

#define MY_CDEB_MAJOR	42
#define MY_CDEV_NAME	"my_char"
#define MY_MAX_MINORS  5

struct my_device_data {
	struct cdev cdev;
};

struct my_device_data devs[MY_MAX_MINORS];

const int arr[5];

static int my_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buffer,
					size_t size, loff_t *offset)
{
	char *message = "Hello from kernel space!\n";
	int message_len = strlen(message);
	int bytes_to_read;
	
	/* Check if we've already read everything */
	if (*offset >= message_len)
		return 0;  /* EOF */
	
	/* Calculate how many bytes to read */
	bytes_to_read = message_len - *offset;
	if (bytes_to_read > size)
		bytes_to_read = size;
	
	/* Copy data from kernel space to user space */
	if (copy_to_user(user_buffer, message + *offset, bytes_to_read))
		return -EFAULT;  /* Bad address */
	
	/* Update the offset */
	*offset += bytes_to_read;
	
	return bytes_to_read;  /* Return number of bytes read */
}

static ssize_t my_write(struct file *file, const char __user *user_buffer,
					size_t size, loff_t * offset)
{
	return 0;
}

static int my_release(struct inode *inode , struct file *filp)
{
	return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	return 0;
}

const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
	.unlocked_ioctl = my_ioctl
};

static __init int my_char_init(void)
{
	int i, err;
	err = register_chrdev_region(MKDEV(MY_CDEB_MAJOR, 0), MY_MAX_MINORS,
								MY_CDEV_NAME);
	if (err != 0) {
		/* report error */
		return err;
	}
	for(i = 0; i < MY_MAX_MINORS; i++) {
		/* initialize devs[i] fields */
		cdev_init(&devs[i].cdev, &my_fops);
		cdev_add(&devs[i].cdev, MKDEV(MY_CDEB_MAJOR, i), 1);
	}
	printk(KERN_INFO "Hello kernel\n");
	return 0;
}

static void __exit my_char_exit(void)
{
	int i;
	printk(KERN_INFO "Goodbye\n");

	for(i = 0; i < MY_MAX_MINORS; i++) {
		/* release devs[i] fields */
		cdev_del(&devs[i].cdev);
	}
	unregister_chrdev_region(MKDEV(MY_CDEB_MAJOR, 0), MY_MAX_MINORS);
}

module_init(my_char_init);
module_exit(my_char_exit);
