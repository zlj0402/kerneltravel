/*
 * reference by Linux Courses baoyou.xie
 */

#include <linux/kernel.h>
//#include <linux/init.h>
#include <linux/module.h>
// kallsyms_lookup_name
#include <linux/kallsyms.h>
// struct task_struct
#include <linux/sched.h>
// struct hrtimer/hrtimer_init/hrtimer_cancel/...
#include <linux/hrtimer.h>
// struct stacktrace
#include <linux/stacktrace.h>
#include <linux/trace_events.h>

#define BACKTRACE_DEPTH 20

#define FSHIFT 11	/* bits count of fraction */
#define FIXED_1 (1 << FSHIFT)
#define LOAD_INT(x) ((x) >> FSHIFT)	/* load integer part */
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)

unsigned long* ptr_avenrun;

struct hrtimer timer;

static void print_all_task_stack(void)
{
	struct task_struct *g, *p;
	unsigned long backtrace[BACKTRACE_DEPTH];
	struct stack_trace trace;

	memset(&trace, 0, sizeof(trace));
	memset(backtrace, 0, sizeof(unsigned long) * BACKTRACE_DEPTH);
	trace.max_entries = BACKTRACE_DEPTH;
	trace.entries = backtrace;

	printk("!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	printk("\tLoad: %lu.%02lu, %lu.%02lu, %lu.%02lu",
		LOAD_INT(ptr_avenrun[0]), LOAD_FRAC(ptr_avenrun[0]),
		LOAD_INT(ptr_avenrun[1]), LOAD_FRAC(ptr_avenrun[1]),
		LOAD_INT(ptr_avenrun[2]), LOAD_FRAC(ptr_avenrun[2]));
	printk("dump all tasks: --------\n");

	rcu_read_lock();

	printk("dump running task. \n");

	do_each_thread(g, p) {
		if (p->__state == TASK_RUNNING) {
			printk("running task, comm: %s, pid: %d\n", p->comm, p->pid);
			memset(&trace, 0, sizeof(trace));
			memset(backtrace, 0, BACKTRACE_DEPTH * sizeof(unsigned long));
			trace.max_entries = BACKTRACE_DEPTH;
			trace.entries = backtrace;
			save_stack_trace_tsk(p, &trace);
			print_stack_trace(&trace, 0);
		}
	} while_each_thread(g, p);

	printk("dump uninterrupted task. \n");

	do_each_thread(g, p) {
		if (p->__state & TASK_UNINTERRUPTIBLE) {
			printk("uninterrupted task, comm: %s, pid: %d\n", p->comm, p->pid);
			memset(&trace, 0, sizeof(trace));
			memset(backtrace, 0, BACKTRACE_DEPTH * sizeof(unsigned long));
			trace.max_entries = BACKTRACE_DEPTH;
			trace.entries = backtrace;
			save_stack_trace_tsk(p, &trace);
			print_stack_trace(&trace, 0);
		}
	} while_each_thread(g, p);

	rcu_read_unlock();
}

static void check_load(void)
{
	static ktime_t last;
	u64 ms;
	int load = LOAD_INT(ptr_avenrun[0]);	/* 1min 5min 15min 的 load average, 取1min的 */

	if (load < 3)
		return;

	// 与上次打印时间，间隔<20 ms，退出这次check load
	ms = ktime_to_ms(ktime_sub(ktime_get(), last));
	if (ms < 20 * 1000)
		return;

	last = ktime_get();
	print_all_task_stack();
}

static enum hrtimer_restart monitor_handler(struct hrtimer* hrtimer)
{
	enum hrtimer_restart ret = HRTIMER_RESTART;
	check_load();
	hrtimer_forward_now(hrtimer, ms_to_ktime(10));
	return ret;
}

static void start_timer(void)
{
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_PINNED);
	timer.function = monitor_handler;
	hrtimer_start_range_ns(&timer, ms_to_ktime(10), 0, HRTIMER_MODE_REL_PINNED);
}

static int __init load_monitor_init(void)
{
	ptr_avenrun = (void*)kallsyms_lookup_name("avenrun");
	if (!ptr_avenrun)
	{
		return -EINVAL;
	}

	start_timer();

	printk("load monitor loaded.\n");
	return 0;
}

static void __exit load_monitor_exit(void)
{
	printk("load monitor unloaded.\n");
}


module_init(load_monitor_init);
module_exit(load_monitor_exit);

MODULE_LICENSE("GPL");
