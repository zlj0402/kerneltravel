#include <linux/sched.h>
#include <linux/module.h>

#include "button_core.h"

#define NEXT_POS(x) ((x+1)%KEY_BUF_SIZE)

struct button_core g_button_core;

void button_core_init(struct button_core *core)
{
    init_waitqueue_head(&core->waitq);
    spin_lock_init(&core->lock);
    core->front = core->rear = 0;
}

bool button_core_empty(struct button_core *core)
{
    return core->front == core->rear;
}

// front -> 写指针; reart -> 读指针;
static bool button_core_full(struct button_core *core)
{
    return NEXT_POS(core->front) == core->rear;
}

void button_core_put_key(struct button_core *core, int key)
{
    unsigned long flags;
    spin_lock_irqsave(&core->lock, flags);
    if (!button_core_full(core)) {
        core->buf[core->front] = key;
        core->front = NEXT_POS(core->front);
		printk("\tput key: %d\n", key);
        wake_up_interruptible(&core->waitq);
    }
    spin_unlock_irqrestore(&core->lock, flags);
}

int button_core_get_key(struct button_core *core)
{
    int key = 0;
    unsigned long flags;
    spin_lock_irqsave(&core->lock, flags);
    if (!button_core_empty(core)) {
        key = core->buf[core->rear];
        core->rear = NEXT_POS(core->rear);
		printk("\tget key: %d\n", key);
    }
    spin_unlock_irqrestore(&core->lock, flags);
    return key;
}
