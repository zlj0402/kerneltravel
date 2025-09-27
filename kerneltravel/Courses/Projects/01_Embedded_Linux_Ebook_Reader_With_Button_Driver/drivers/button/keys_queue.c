#include <keys_queue.h>

#include <linux/decompress/mm.h>

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Circular Queue <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static bool kq_empty(struct keys_queue *q) { return q->front == q->rear; }

static bool kq_full(struct keys_queue *q)  { return q->front == (q->rear + 1) % q->size; }

static void kq_put(struct keys_queue *q, int key) {

    if (!kq_full(q)) {
		
        q->buf[q->rear] = key;
        q->rear = (q->rear + 1) % q->size;
    }
}

static int kq_get(struct keys_queue *q) {

    int key = 0;
	
    if (!kq_empty(q)) {
		
        key = q->buf[q->front];
        q->front = (q->front + 1) % q->size;
    }
	
    return key;
}

struct keys_queue *keys_queue_create(int size) {

    struct keys_queue *q = malloc(sizeof(*q));
    q->buf = malloc(sizeof(int) * size);
    q->size = size;
    q->front = q->rear = 0;
    q->empty = kq_empty;
    q->full  = kq_full;
    q->put   = kq_put;
    q->get   = kq_get;
    return q;
}
EXPORT_SYMBOL(keys_queue_create);

void keys_queue_destroy(struct keys_queue *q) {

    if (!q)
        return;

    if (q->buf) {
        free(q->buf);
        q->buf = NULL;
    }

    free(q);
}
EXPORT_SYMBOL(keys_queue_destroy);

