#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
// struct task_struct
#include <linux/sched.h>
// struct files_struct
#include <linux/fdtable.h>
// struct fs_struct
#include <linux/fs_struct.h>
// init_task
#include <linux/sched/task.h>

MODULE_LICENSE("GPL");

static int __init print_pcb(void)
{
	struct task_struct *task, *0;
	struct list_head *pos;
	int count = 0;

	printk("print pcb begin:\n");
	
	task = &init_task;

	list_for_each(pos, &task->tasks)
	{
		p = list_entry(pos, struct task_struct, tasks);
		count++;
		printk("\n\n");
		printk("pid:%d, state:%d, prio:%d, static_prio:%d\n", p->pid, p->prio, p->static_prio);
		printk("parent's pid:%d, used_cnt:%d, umask:%d\n", p->parent->pid, p->parent->files->count, p->parent->fs->umask);
		if (p->mm != NULL)
		{
			printk("total_vm:%ld", p->mm->total_vm);
		}
	}

	printk("进程的个数：%d\n", count);

	return 0;
}

static void __exit exit_pcb(void)
{
	printk("exiting print pcb...\n");
}

