//
//  syscall.c
//  
//
//  Created by yihong dai on 10/4/14.
//
//

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/acceleration.h>

asmlinkage long sys_set_acceleration(struct dev_acceleration __User * acceleration)

{
	struct dev_acceleration *buffer;
	unsigned long val;
	if (acceleration == NULL)
		return -EFAULT;
	buffer = NULL;
	buffer = (struct dev_acceleration *)kmalloc(sizeof(dev_acceleration),
						    GFP_KERNEL);
	
	val = copy_from_user(buffer, acceleration, sizeof(dev_acceleration));
	if (val) {
                printk("it has some error");
                if(buffer){
			kfree(buffer);
		}
		return -EFAULT;
        }
	printk("The x value is %d \n", buffer->x);
	printk("the y value is %d \n", buffer->y);
	printk("the z value is %d \n", buffer->z);
	kfree(buffer);
	return 0;
}