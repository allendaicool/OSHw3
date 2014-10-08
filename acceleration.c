#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/acceleration.h>
#include <linux/spinlock.h>

/*Define the noise*/
#define NOISE 10

/*Define the window*/
#define WINDOW 20

int acceleration_head = 0;
int acceleration_end = 0;
int buffer_Count_acceleration = 0;
struct dev_acceleration  *our_buffer;
event_num = 0;
DEFINE_SPINLOCK(mr_lock);

/* Create an event based on motion.
 * If frq exceeds WINDOW, cap frq at WINDOW.
 * Return an event_id on success and the appropriate error on failure.
 * system call number 379
 */
static LIST_HEAD(collection_head);

asmlinkage int accevt_create(struct acc_motion __user *acceleration)
{
	spin_lock(&mr_lock);
	struct collection *bundle = (struct collection *)kmalloc(sizeof(struct collection)
						,GFP_KERNEL);
	bundle->event_id = event_num;
	event_num++;
	unsigned long val;
	struct acc_motion *buffer;
	buffer = NULL;
	buffer = (struct acc_motion *)kmalloc(sizeof(struct acc_motion),
						    GFP_KERNEL);
	val = copy_from_user(buffer, acceleration,
			     sizeof(struct acc_motion));
	if (val) {
                printk("it has some error");
                if(buffer){
			kfree(buffer);
		}
		return -EFAULT;
        }
	bundle->motionType = buffer;
	init_waitqueue_head(&bundle->heading);
	list_add(&bundle->Node,&collection_head);
	spin_unlock(&mr_lock);
	return event_num-1;
}




asmlinkage int sys_accevt_signal(struct dev_acceleration __user * acceleration)
{
	
	if(our_buffer == NULL)
		our_buffer = (struct dev_acceleration *)kmalloc(
								WINDOW*sizeof(struct dev_acceleration ),GFP_KERNEL);
	struct dev_acceleration *buffer;
	unsigned long val;
	if (acceleration == NULL)
		return -EFAULT;
	buffer = NULL;
	buffer = (struct dev_acceleration *)kmalloc(sizeof(struct dev_acceleration),
						    GFP_KERNEL);
	
	val = copy_from_user(buffer, acceleration, sizeof(struct dev_acceleration));
	if (val) {
                printk("it has some error");
                if(buffer){
			kfree(buffer);
		}
		return -EFAULT;
        }
	
 		if (buffer_Count_acceleration < WINDOW){
		our_buffer[acceleration_end].x = buffer->x;
		our_buffer[acceleration_end].y = buffer->y;
		our_buffer[acceleration_end].z = buffer->z;
		buffer_Count_acceleration++;
		if (acceleration_end < WINDOW -1)
			acceleration_end++;
	}
	else {
		acceleration_end = (acceleration_end + 1) % WINDOW;
		acceleration_head = (acceleration_head + 1) % WINDOW;
		our_buffer[acceleration_end].x = buffer->x;
		our_buffer[acceleration_end].y = buffer->y;
		our_buffer[acceleration_end].z = buffer->z;
	}
	
	
	
	spin_lock(&mr_lock);

	
	
	
	
	spin_unlock(&mr_lock);

		kfree(buffer);
	return 0;

}


asmlinkage long sys_set_acceleration(struct dev_acceleration __user * acceleration)

{
	if(our_buffer == NULL)
		our_buffer = (struct dev_acceleration *)kmalloc(
                                                          WINDOW*sizeof(struct dev_acceleration ),GFP_KERNEL);
	struct dev_acceleration *buffer;
	unsigned long val;
	if (acceleration == NULL)
		return -EFAULT;
	buffer = NULL;
	buffer = (struct dev_acceleration *)kmalloc(sizeof(struct dev_acceleration),
						    GFP_KERNEL);
	
	val = copy_from_user(buffer, acceleration, sizeof(struct dev_acceleration));
	if (val) {
                printk("it has some error");
                if(buffer){
			kfree(buffer);
		}
		return -EFAULT;
        }

 	printk("The x value---------  is %d \n", buffer->x);
          printk("the y value ----------- is %d \n", buffer->y);
         printk("the z value------------ is %d \n", buffer->z);
	if (buffer_Count_acceleration < WINDOW){
		printk("it gets in B");
		our_buffer[acceleration_end].x = buffer->x;
		our_buffer[acceleration_end].y = buffer->y;
		our_buffer[acceleration_end].z = buffer->z;
		printk("it gets in A\n");
		 printk("The x value is %d,   x val is %d \n",our_buffer[acceleration_end].x, acceleration_end );
                 printk("the y value is %d \n", our_buffer[acceleration_end].y);
                  printk("the z value is %d \n",our_buffer[acceleration_end].z);
		buffer_Count_acceleration++;
		if( acceleration_end < WINDOW -1)
			acceleration_end++;
	}
	else {
		acceleration_end = (acceleration_end+1)%WINDOW;
		acceleration_head = (acceleration_head+1)%WINDOW;
		our_buffer[acceleration_end].x = buffer->x;
		our_buffer[acceleration_end].y = buffer->y;
		our_buffer[acceleration_end].z = buffer->z;
		printk("The x value is %d \n",our_buffer[acceleration_end].x );
        	 printk("the y value is %d \n", our_buffer[acceleration_end].y);
 	         printk("the z value is %d \n",our_buffer[acceleration_end].z);
	}

printk("The x value +++++++++++  is %d \n", buffer->x);
	printk("the y value+++++++++++ is %d \n", buffer->y);
	printk("the z value++++++++++++++ is %d \n", buffer->z);
	printk("+++++++ the head is %d\n " , acceleration_head);
	printk("+++++++ the end is %d\n " , acceleration_end);
	
	kfree(buffer);
	return 0;
}
