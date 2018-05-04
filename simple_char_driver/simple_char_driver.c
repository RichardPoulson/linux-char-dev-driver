/*
Richard Poulson
Programming Assignment 2

@info:
  https://www.linuxjournal.com/article/6930
  http://www.c4learn.com/c-programming/c-file-structure-and-file-pointer/

  unsigned long __copy_to_user(void __user * to, const void * from, unsigned long n)
    to: Destination address, in user space.
    from: Source address, in kernel space.
    n: Number of bytes to copy.
  unsigned long __copy_from_user(void * to, const void __user * from, unsigned long n)
    to: Destination address, in kernel space.
    from: Source address, in user space.
    n: Number of bytes to copy.
*/

#include<linux/module.h>
#include<linux/init.h>
//#include <linux/device.h>         // Header to support the kernel Driver Model
//#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include<linux/fs.h> // functions that are related to device driver coding
#include<linux/slab.h> // functions that are related to device driver coding
#include<linux/uaccess.h> // copy_to_user, copy_from_user

MODULE_LICENSE("GPL"); // license type
MODULE_AUTHOR("Richard Poulson");
MODULE_DESCRIPTION("CSCI 3753, Linux device driver programming assignment");
MODULE_VERSION("0.1");
MODULE_INFO(intree, "Y");

#define BUFFER_SIZE 1024


/* Define device_buffer and other global data structures you will need here */
static char * device_buffer;
static char * message_pointer;
static ssize_t size_of_message;
static int number_times_opened = 0; // number of times the device has been opened
static int number_times_closed = 0; // number of times the device has been closed

ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
    if(length > (strlen(device_buffer) - (*offset))) {
        length = (strlen(device_buffer) - (*offset));
    }
    int f_pos = (int) pfile->f_pos;
    //printk(KERN_INFO "simple_char_driver: reading, read %d bytes from the device", (int)length);
	printk(KERN_INFO "simple_char_driver: f_pos(%d), offset(%d), buffer(%lu)", f_pos, (int)(*offset), strlen(device_buffer));
    copy_to_user(buffer, device_buffer, length);

    return length;
}

ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "fileAddress(%p), length(%d), offset(%d)", pfile, (int) length, (int) (*offset));
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
    copy_from_user(device_buffer + *offset, buffer, length);
    *offset += length;
	printk(KERN_INFO "simple_char_driver: writing, wrote %d bytes to the device", (int)length);
	return length;
}

int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
    // keep track of how many times the file has been opened, and print the
    // number of times it's been opened
	number_times_opened++; // device is opened, so increment the count
	printk(KERN_INFO "simple_char_driver: device opened, has been opened %d time(s)", number_times_opened);
	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	number_times_closed++; // device closed, so increment the count
	printk(KERN_INFO "simple_char_driver: device closed, has been closed %d time(s)", number_times_closed);
	return 0;
}

//== was loff_t* before, checked in fs.h and it should just be loff_t
loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	printk(KERN_INFO "simple_char_driver: seeking");
    // current position is set to the offset
	if(whence == SEEK_SET) { pfile->f_pos = offset; }
    // current position is incremented by offset bytes
	if(whence == SEEK_CUR) { pfile->f_pos += offset; }
    // current position is set to offset bytes before the end of the file
	if(whence == SEEK_END) { pfile->f_pos = strlen(device_buffer) - abs(offset); }
	return pfile->f_pos;
}

struct file_operations simple_char_driver_file_operations = {
	.owner = THIS_MODULE,
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
	.open	 = simple_char_driver_open,
	.release = simple_char_driver_close,
	.read	 = simple_char_driver_read,
	.write = simple_char_driver_write,
	.llseek = simple_char_driver_seek,
};

static int simple_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	/* register the device */
	// register_chrdev(unsigned int major, const char * name, const struct file_operations * fops););
	printk(KERN_INFO "simple_char_driver: allocating memory space for device_buffer");
    // GFP_KERNEL: This is a normal allocation and might block. This is the flag
    // to use in process context code when it is safe to sleep.
	device_buffer = (char *) kmalloc(BUFFER_SIZE, GFP_KERNEL);
	printk(KERN_INFO "simple_char_driver: registering major number 240 under name \"simple_char_driver\"");
	register_chrdev(240, "simple_char_driver", &simple_char_driver_file_operations);
    printk(KERN_INFO "simple_char_driver: init: buffer(%lu)", strlen(device_buffer));

	return 0;
}

static void simple_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
	printk(KERN_INFO "simple_char_driver: freeing memory allocated for device buffer");
	kfree(device_buffer);
	printk(KERN_INFO "simple_char_driver: un-registering major number 240 under name \"simple_char_driver\"");
	unregister_chrdev(240, "simple_char_driver");
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);
