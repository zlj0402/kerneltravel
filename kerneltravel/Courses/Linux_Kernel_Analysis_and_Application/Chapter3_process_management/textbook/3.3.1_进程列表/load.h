/**
 * content: 1. print processes by (struct task_struct)->tasks
 * 			2. execute as kernel module
 *
 * execute: 如果有配置arm开发板的工具链全局变量，先临时的去除CROSS_COMPILE和ARCH:
 * 			unset ARCH
 * 			unset CROSS_COMPILE
 * 			1. make
 * 			2. sudo insmod proclist_print.ko
 * 			3. dmesg # 查看模块安装时 init模块的日志
 * 			4. sudo rmmod proclist_print
 * 			5. dmesg # 查看模块卸载时 退出函数的日志
 *
 * output:
 * 			[ 3235.052384] proclist_print: loading out-of-tree module taints kernel.
 *			[ 3235.052523] proclist_print: module verification failed: signature and/or required key missing - tainting kernel
 * 			[ 3235.053140] proc print module loaded.
 * 			[ 3235.053141] print_pid enter begin:
 * 			[ 3235.053142]  1 ---> systemd
 * 			[ 3235.053143]  2 ---> kthreadd
 * 			[ 3235.053144]  3 ---> rcu_gp
 * 			[ 3235.053145]  4 ---> rcu_par_gp
 * 			[ 3235.053146]  5 ---> slub_flushwq
 * 			...
 * 			[ 3235.053414]  3297 ---> sftp-server
 * 			[ 3235.053414]  3299 ---> insmod
 * 			[ 3235.053415] the number of process is: 308
 * 			[ 3345.833827] proc print module unloaded.
 * ls 生成的模块:
 * 			zlj@zlj-vm:~/zljgit/kerneltravel/Courses/Linux_Kernel_Analysis_and_Application/Chapter3_process_management/textbook/3.3.1_进程列表$ ls
 * 			Makefile       Module.symvers    proclist_print.ko   proclist_print.mod.c  proclist_print.o
 * 			modules.order  proclist_print.c  proclist_print.mod  proclist_print.mod.o
 */
