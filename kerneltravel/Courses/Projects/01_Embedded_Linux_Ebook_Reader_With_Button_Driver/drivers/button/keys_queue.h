#ifndef KEYS_QUEUE_H
#define KEYS_QUEUE_H

// bool
#include <linux/types.h>

struct keys_queue {
	
    int front, rear;
    int *buf;
    int size;

    bool (*empty)(struct keys_queue *);
    bool (*full)(struct keys_queue *);
    void (*put)(struct keys_queue *, int key);
    int  (*get)(struct keys_queue *);
};

extern struct keys_queue* kque;

struct keys_queue *keys_queue_create(int size);
void keys_queue_destroy(struct keys_queue *q);

#endif //KEYS_QUEUE_H

