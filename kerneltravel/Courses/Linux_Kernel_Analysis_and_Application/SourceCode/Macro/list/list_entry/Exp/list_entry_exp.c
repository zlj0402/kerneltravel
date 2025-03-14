#include <stdio.h>

// last whole define: v2.5.27
#define list_entry(ptr, type, member) \
		((type*)((char*)ptr - (unsigned long)(&((type*)0)->member)))

struct test {
	int a;
	char bff;
	double c;
};

int main() {
	struct test example = {1, 42, 3};
	printf("a = %d, b = %c, c = %.2f\n", example.a, example.bff, example.c);
	printf("example address: 0x%lx\n", (unsigned long)(&example));
	printf("list entry -- example address: 0x%lx\n", (unsigned long)list_entry(&example.bff, struct test, bff));

	return 0;
}

/* output:
zlj@zlj-vm:~/zljgit/kerneltravel/Courses/Linux_Kernel_Analysis_and_Application/SourceCode/Macro/list/list_entry/Exp$ ./list_entry_exp
a = 1, b = *, c = 3.00
example address: 0x7ffed35b9030
list entry -- example address: 0x7ffed35b9030
*/

/* expalin:
 the third argument in list_entry macro, is the name what we define in the struct
*/
