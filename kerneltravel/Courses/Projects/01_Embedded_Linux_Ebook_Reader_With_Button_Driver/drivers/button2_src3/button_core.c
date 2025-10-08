#include <linux/sched.h>

#include "button_core.h"

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

static bool button_core_full(struct button_core *core)
{
    return ((core->rear + 1) % KEY_BUF_SIZE) == core->front;
}

void button_core_put_key(struct button_core *core, int key)
{
    unsigned long flags;
    spin_lock_irqsave(&core->lock, flags);
    if (!button_core_full(core)) {
        core->buf[core->rear] = key;
        core->rear = (core->rear + 1) % KEY_BUF_SIZE;
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
        key = core->buf[core->front];
        core->front = (core->front + 1) % KEY_BUF_SIZE;
    }
    spin_unlock_irqrestore(&core->lock, flags);
    return key;
}

