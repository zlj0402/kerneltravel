#include "list.h"

#include <stdio.h>
#include <stddef.h>

#include "load.h"

typedef struct {
	int iVal;
	struct list_head tList;
} T_List, *PT_List;

int main(void)
{
	T_List a;
	LIST_HEAD_SET(&a.tList);

	printf("a addr: %p\n", &a);
	printf("a.tList addr: %p\n", &a.tList);
	printf("list_entry(&a.tList, T_List, tList): %p\n", list_entry(&a.tList, T_List, tList));
}
