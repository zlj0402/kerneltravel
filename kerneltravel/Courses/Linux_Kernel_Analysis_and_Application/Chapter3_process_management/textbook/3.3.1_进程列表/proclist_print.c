#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/list.h>

#include "load.h"

static int print_pid(void);

static int __init print_proc_init(void)
{
	printk("proc print module loaded.\n");
	print_pid();
	return 0;
}

static void __exit print_proc_exit(void)
{
	printk("proc print module unloaded.\n");
}

static int print_pid(void)
{
	struct task_struct * task, * p;
	struct list_head * pos;
	int count = 0;
	
	printk("print_pid enter begin:\n");
	task = &init_task;
	list_for_each(pos, &(task->tasks))
	{
		p = list_entry(pos, struct task_struct, tasks);
		count++;
		printk(" %d ---> %s\n", p->pid, p->comm);
	}
	printk("the number of process is: %d\n", count);

	return 0;
}

module_init(print_proc_init);
module_exit(print_proc_exit);

MODULE_LICENSE("GPL");

