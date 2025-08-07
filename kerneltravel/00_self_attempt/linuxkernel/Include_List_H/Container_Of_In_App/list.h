#ifndef ZLJ_LINUX_LIST_H
#define ZLJ_LINUX_LIST_H

struct list_head {
	struct list_head *next, *prev;
};

typedef struct list_head list_t;

/**
 * container_of - cast a member of a structure out to the containing structure
 *
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - offsetof(type,member) );})


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD_SET(ptr) do { \
	(ptr)->next = (ptr);	\
	(ptr)->prev = (ptr);	\
} while (0)

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &list_t pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
/*
#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
*/

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(list_t *new, list_t *prev, list_t *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(list_t *new, list_t *head)
{
	__list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(list_t *new, list_t *head)
{
	__list_add(new, head->prev, head);
	// 对于 __list_add 来看，不分头插尾插，new 都是要插在，第二个参数和第三个参数之间
}


#endif 	//ZLJ_LINUX_LIST_H
