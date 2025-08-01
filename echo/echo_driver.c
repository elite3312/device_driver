#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "echo_driver"
#define BUF_LEN 256

static int major;
static char msg_buffer[BUF_LEN];
static int msg_size;

static ssize_t device_read(struct file *filp, char *buffer, size_t len, loff_t *offset) {
    if (*offset >= msg_size) return 0;  // EOF
    
    int bytes_to_read = msg_size - *offset;
    if (bytes_to_read > len) bytes_to_read = len;
    
    if (copy_to_user(buffer, msg_buffer + *offset, bytes_to_read))
        return -EFAULT;
    
    *offset += bytes_to_read;
    return bytes_to_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t *offset) {
    msg_size = (len > BUF_LEN) ? BUF_LEN : len;
    return copy_from_user(msg_buffer, buffer, msg_size) ? -EFAULT : msg_size;
}

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
};

static int __init echo_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) return major;
    printk(KERN_INFO "Echo driver loaded with major %d\n", major);
    return 0;
}

static void __exit echo_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Echo driver unloaded\n");
}

module_init(echo_init);
module_exit(echo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("永璿 & Copilot");
MODULE_DESCRIPTION("Simple Echo Character Driver");