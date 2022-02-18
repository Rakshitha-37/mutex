#include <linux/module.h> 
#include <linux/kthread.h>	//thread
#include <linux/delay.h> 
#include<linux/mutex.h>

MODULE_LICENSE("GPL");

struct mutex my_mutex;

static struct task_struct *char_thread; 
static struct task_struct *char_thread1;

int count = 0;

int my_thread_fun(void *p);

int my_thread_fun(void *p)
{
        int lock = 0;
	while(!kthread_should_stop())        
	{
	  lock = mutex_lock_interruptible(&my_mutex);
	  if(lock == 0)
	  {
	      printk(KERN_INFO"my thread count is %d\n", count++);
	      mutex_unlock(&my_mutex);
	      msleep(1000);
	  }
	  else
	  {
	      printk(KERN_INFO "Thread is interuppted by signal\n");
	  }
	}	
	return 0;
}

static int __init my_simpledriver_init(void)
{

	printk(KERN_INFO"my sample driver start");
	mutex_init(&my_mutex);	
	char_thread = kthread_run(my_thread_fun,NULL,"my char thread");
	if(char_thread)
	{
	
		printk(KERN_INFO"kthread created successfully\n");
		
	}
	else{
		printk(KERN_INFO"COuldnt create thread\n");
	}
	
	char_thread1 = kthread_run(my_thread_fun,NULL,"my char thread 1");   // same name in thread 1 and 2
	if(char_thread1)
	{
		
		printk(KERN_INFO"kthread created successfully\n");
		
	}
	else
	{	
		
		printk(KERN_INFO"COuldnt create thread\n");
	}
	
	return 0;

}

module_init(my_simpledriver_init);

static void  __exit my_simpledriver_exit(void){

	printk(KERN_INFO"my sample driver exit");
	kthread_stop(char_thread);
	kthread_stop(char_thread1);

}

module_exit(my_simpledriver_exit);
