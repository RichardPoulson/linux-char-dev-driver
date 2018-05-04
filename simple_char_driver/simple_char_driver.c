/*
Richard Poulson
Programming Assignment 2

@info:
  https://www.linuxjournal.com/article/6930
  http://www.c4learn.com/c-programming/c-file-structure-and-file-pointer/
*/

#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h> // functions that are related to device driver coding
#include<linux/slab.h> // kfree, kmalloc
#include<linux/uaccess.h> // copy_to_user, copy_from_user

MODULE_LICENSE("GPL"); // license type
MODULE_AUTHOR("Richard Poulson");
MODULE_DESCRIPTION("CSCI 3753, Linux device driver programming assignment");
MODULE_VERSION("0.1");
MODULE_INFO(intree, "Y");

#define BUFFER_SIZE 1024

/* Define device_buffer and other global data structures you will need here */
static char * device_buffer;
static int number_times_opened = 0; // number of times the device has been opened
static int number_times_closed = 0; // number of times the device has been closed

ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
    // if requesting to read more characters than available, adjust length
    if( (int)length > ((int)BUFFER_SIZE - (int)(*offset)) ) {
        length = (int)BUFFER_SIZE - (int)(*offset);
    }
    copy_to_user(buffer, device_buffer + *offset, length);
    printk(KERN_INFO "simple_char_driver: read %d bytes from device", (int)length);
    // just read some characters, so need to adjust the offset
    *offset += length;
    return length;
}

ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
    copy_from_user(device_buffer + *offset, buffer, length);
    // just wrote some characters, so need to adjust the offset
    *offset += length;
	printk(KERN_INFO "simple_char_driver: wrote %d bytes to device", (int)length);
	return length;
}

int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	number_times_opened++; // device is opened, so increment the count
	printk(KERN_INFO "simple_char_driver: has been opened %d time(s)", number_times_opened);
	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	number_times_closed++; // device closed, so increment the count
	printk(KERN_INFO "simple_char_driver:has been closed %d time(s)", number_times_closed);
	return 0;
}

//== was loff_t* before, checked in fs.h and it should just be loff_t
loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	printk(KERN_INFO "simple_char_driver: seeking: offset(%d), whence(%d)", (int)offset, (int)whence);
    // for following control structures, make sure offset will be in bounds
	if(whence == SEEK_SET) {
        if( (offset<0) || (offset>strlen(device_buffer))) {
            printk(KERN_ERR "simple_char_driver: invalid position: \"%d\"", (int)offset);
        }
        else { pfile->f_pos = offset; }
    }
    // current position is incremented by offset bytes
	if(whence == SEEK_CUR) {
        int new_offset = pfile->f_pos + offset;
        if( (new_offset < 0) || (new_offset > strlen(device_buffer)) ) {
            printk(KERN_ERR "simple_char_driver: invalid position: \"%d\"", new_offset);
        }
        else { pfile->f_pos = new_offset; }
    }
    // current position is set to offset bytes before the end of the file
	if(whence == SEEK_END) { pfile->f_pos = strlen(device_buffer) - abs(offset); }
    // seek() expects new offset to be returned (I believe)
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
	printk(KERN_INFO "simple_char_driver: allocating memory and registering device");
    // GFP_KERNEL: This is a normal allocation and might block. This is the flag
    // to use in process context code when it is safe to sleep.
	device_buffer = (char *) kmalloc(BUFFER_SIZE, GFP_KERNEL);
	register_chrdev(240, "simple_char_driver", &simple_char_driver_file_operations);
	return 0;
}

static void simple_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
	printk(KERN_INFO "simple_char_driver: exit: freeing memory and unregistering device");
	kfree(device_buffer);
	unregister_chrdev(240, "simple_char_driver");
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);
