#ifndef BUTTON_CORE_H
#define BUTTON_CORE_H

#include <linux/wait.h>
#include <linux/spinlock.h>

#define KEY_BUF_SIZE 128

struct button_core {
    wait_queue_head_t waitq;
    spinlock_t lock;
    int buf[KEY_BUF_SIZE];
    int front;
    int rear;
};

extern struct button_core g_button_core;

void button_core_init(struct button_core *core);
void button_core_put_key(struct button_core *core, int key);
int  button_core_get_key(struct button_core *core);
bool button_core_empty(struct button_core *core);

int button_chrdev_init(void);
void button_chrdev_exit(void);

#endif

