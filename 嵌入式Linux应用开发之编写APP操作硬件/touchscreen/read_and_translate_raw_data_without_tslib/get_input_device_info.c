#include <linux/input.h>
// struct dirent
#include <dirent.h>
// printf perror
#include <stdio.h>
// free
#include <stdlib.h>
// oepn\fcntl\select
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// strcmp
#include <string.h>
// struct input_event
//#include <linux/input.h>
// read\fcntl
#include <unistd.h>
// poll
#include <poll.h>
// select
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
//#include <sys/types.h>
//#include <unistd.h>
// signal
#include <signal.h>
// div_t
#include <stdlib.h>
// bool
#include <stdbool.h>

#include "input_events.h"


#define INPUT_DEVICES_PATH "/dev/input/"

// all function can use it seprately; I set this, mainly for async read device
static int global_fd;

static char fname[512];


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< get input devices name >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// 指针遍历方式，输出 输入设备名
static void scan_devices(int* ndev, struct dirent ***namelist) 
{
    *ndev = scandir(INPUT_DEVICES_PATH, namelist, NULL, alphasort);

    if (*ndev <= 0)
    {
        printf((*ndev == 0 ? "No devices scanned in %s\n" : "scandir %s error.\n"), INPUT_DEVICES_PATH);
        return;
    }

    // 使用指针遍历 namelist
    struct dirent **p = *namelist;
    for (int i = 0; i < *ndev; i++, p++) 
    {
        printf("device %d: %s\n", i, (*p)->d_name);
    }
}

// 非指针方式，输出 输入设备名
static void scan_devices2(int* ndev, struct dirent ***namelist) 
{
	*ndev = scandir(INPUT_DEVICES_PATH, namelist, NULL, alphasort);

	if (*ndev <= 0)
	{
		printf((*ndev == 0 ? "No devices scaned in %s\n" : "scandir %s error.\n"), INPUT_DEVICES_PATH);
		return;
	}

	for (int i = 0; i < *ndev; i++)
	{
  		int len  = snprintf(fname, sizeof(fname), "%s%s", INPUT_DEVICES_PATH, (*namelist)[i]->d_name);
		printf("dev_no: %d, name len: %d, fname: %s\n",i ,len, fname);
	}
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< read device info >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

static void print_value_in_binary_by_pointer(unsigned int bytes_len, unsigned char *bits)
{
	// the length of data in bits >= bytes_len * 8
	unsigned char byte;
	for (int i = 0; i < bytes_len; i++)
	{
		byte = bits[i];
		for (int bit = 0; bit < 8; bit++)
		{
			printf("%d", (byte >> bit) & 1);
		}
		printf(" ");
	}
	printf("\n");
}

static void explain_binary_meaning(unsigned int bytes_len, unsigned char *bits, const char* meanings[])
{
	unsigned char byte;
	for (int i = 0; i < bytes_len; i++)
	{
		byte = bits[i];
		for (int bit = 0; bit < 8; bit++)
		{
			if ((byte >> bit) & 1)
			{
				int pos = i * 8 + bit;
				printf("%s(%d) ", meanings[pos], pos);
			}
		}
	}
	printf("\n");
}

static void print_value_in_binary_by_value(unsigned int bytes_len, unsigned char* bits)
{
	// the length of data in bits >= bytes_len * 8
	unsigned long value = 0;
	for (int i = 0; i < bytes_len; i++) {
	    value |= ((unsigned long)bits[i]) << (8 * i);  // 逐字节拼接
	}
	
	for (int bit = bytes_len * 8 - 1; bit >= 0; bit--)
	{
		printf((value & (1UL << bit)) ? "1" : "0");
		if (bit % 8 == 0)
			printf(" ");
	}
	printf("\n");
}

static void concat_device_name_in_fname(char* f_name)
{
	snprintf(fname, sizeof(fname), "%s%s", INPUT_DEVICES_PATH, f_name);
	printf("concat device name: %s\n", fname);
}

/*
 * content: judge whether one device is touchscreen device or not 
 * reference: tslib1.23 version -- ts_setup.c
 * return: True or False
 */
static bool is_direct_input_devices(unsigned char* properties)
{
	return *properties & (1UL << (INPUT_PROP_DIRECT));
}

static void ts_max_slots(int fd)
{
	if (fd < 0)
		return;
	
	struct input_absinfo slot;
	if (ioctl(fd, EVIOCGABS(ABS_MT_SLOT), &slot) < 0)
	{
		perror("ioctl EVIOCGABS");
		// ts_close(ts);
	}

	// slot.minimum 是 最小槽编号（通常是 0
	// slot.maximum 是 最大槽编号（通常是 N-1）
	int max_slots  = slot.maximum - slot.minimum + 1;
	printf("\tmax_slots = %d \n", max_slots);
}

/*
 * params:
 * f_name: event0, without dir
 */
static void read_device_info(char* f_name)
{
	int fd = 0;
	int err = 0;
	int len = 0;
	struct input_id id;
	unsigned char d_name[64] = "";
	// unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
	
	// EV_CNT => EV_MAX + 1 => 0x1f + 1 => 32
	
	// #define BITS_PER_BYTE		8
	// #define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
	// BITS_TO_LONGS(nr) => DIV_ROUND_UP(32, 8 * 8) => DIV_ROUND_UP(32, 64)
	// #define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
	// DIV_ROUND_UP(32, 64) => (32 + 64 - 1) / 64 => 1

	// unsigned long evbit[BITS_TO_LONGS(EV_CNT)] => unsigned long evbit[1]
	unsigned long evbit = 0UL;

	// unsigned long evbit[BITS_TO_LONGS(INPUT_PROP_CNT)];
	// INPUT_PROP_CNT => INPUT_PROP_MAX + 1 => 0x1f + 1 => 32
	unsigned long properties = 0UL;

	// return: open error -> return -1
	//-----------------------------------------------------------------
	concat_device_name_in_fname(f_name);
	fd = open(fname, O_RDONLY);
	if (fd < 0)
	{
		printf("Open %s error\n", fname);
		return;
	}

	// content: get device id
	// ioctl error -> return -1
	// return: usually, success -> return 0
	//-----------------------------
	err = ioctl(fd, EVIOCGID, &id);
	if (err < 0)
	{
		printf("ioctl EVIOCGID error, device: %s\n", fname);
		return;
	}

	// content: printf input_id info
	//--------------------------------------
	printf("-----device: %s-----\n", fname);
	printf("bustype = 0x%x\n", id.bustype);
	printf("product = 0x%x\n", id.product);
	printf("vendor = 0x%x\n", id.vendor);
	printf("version = 0x%x\n", id.version);

	// content: get input device (struct input_dev) name/evbit attribute
	// return: len -- the length of bytes read
 	// ---------------------------------------------------
	len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
	if (len > 0 && len <= sizeof(evbit))	// 有读到数据
	{	
		// get device name
		// ------------------------------------------------------------
		int name_len = ioctl(fd, EVIOCGNAME(sizeof(d_name)), d_name);
		printf("device_name(%d char[s]): %s\n", name_len, d_name);

		int prop_len = ioctl(fd, EVIOCGPROP(sizeof(properties)), &properties);
		printf("read ev properties bytes len: %d\n", prop_len);
		print_value_in_binary_by_pointer((unsigned int)prop_len, (unsigned char *)&properties);
		printf("support ev properties: ");
		explain_binary_meaning((unsigned int)prop_len, (unsigned char *)&properties, ev_properties);
		bool is_direct_input_dev = is_direct_input_devices((unsigned char *)&properties);
		printf("Is direct input devices: %s\n", is_direct_input_dev ? "True" : "False");
		if (is_direct_input_dev)
		{
			ts_max_slots(fd);
		}

		
		// get evbit attribute
		// ---------------------------
		printf("read evbit bytes len: %d\n", len);
		// printf evbit as binary --- method 1
		printf("support ev type --- method1: ");
		print_value_in_binary_by_pointer((unsigned int)len, (unsigned char *)&evbit);

		// printf evbit as binary --- method 2
		printf("support ev type --- method2: ");
		print_value_in_binary_by_value((unsigned int)len, (unsigned char *)&evbit);

		// support ev type --- method1: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000111
		// support ev type --- method2: 11100000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
		// 从结果可以看出来，数据是以 小端字节序（Little Endian） 存储的

		printf("support ev type: ");
		explain_binary_meaning((unsigned int)len, (unsigned char *)&evbit, ev_names);

	}
	printf("\n");
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< get device input >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

struct run_time {
	unsigned int days;
	unsigned int hours;
	unsigned int minutes;
	unsigned int seconds;

	void (*set_attributes)(struct run_time* rt, unsigned int sec);
};

/*
typedef struct {
    int quot;  // 商 (quotient)
    int rem;   // 余数 (remainder)
} div_t;
*/
void set_attributes_with_seconds(struct run_time* rt, unsigned int sec)
{
	div_t d1 = div(sec, 60);
	div_t d2 = div(d1.quot, 60);
	div_t d3 = div(d2.quot, 24);

	rt->seconds = d1.rem;
	rt->minutes = d2.rem;
	rt->hours = d3.rem;
	rt->days = d3.quot;
}

void input_event_translation(struct input_event* event)
{
	/*printf("\ttime: %ds, %dμs. type = %s(0x%x), code = %s(0x%x), value = 0x%x(%d);\n", 
		(int)event->time.tv_sec, (int)event->time.tv_usec, 
		ev_names[event->type], event->type,
		ev_type_values[event->type][event->code], event->code,
		event->value, event->value);
	*/

	struct run_time rt;
	rt.set_attributes = set_attributes_with_seconds;
	rt.set_attributes(&rt, (unsigned int)event->time.tv_sec);
	printf("\ttime: %dd %dh %dm %ds.%dμs; type = %s(0x%x), code = %s(0x%x), value = 0x%x(%d);\n", 
		rt.days, rt.hours, rt.minutes, rt.seconds, (int)event->time.tv_usec, 
		ev_names[event->type], event->type,
		ev_type_values[event->type][event->code], event->code,
		event->value, event->value);
}

static void read_device_input(int fd, struct input_event* event)
{
	int bytes_len = 0;
	while(1) 
	{
		bytes_len = read(fd, event, sizeof(struct input_event));
		if (bytes_len == sizeof(struct input_event))
		{
			input_event_translation(event);
			// printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event->type, event->code, event->value);
		}
		else
		{
			// O_NONBLOCK
			// PID  PPID USER     STAT   VSZ %VSZ %CPU COMMAND
			// 2020   303 root     R     1724   0%  86% ./get_input_devices_info event1
			printf("read error %d\n", bytes_len);
		}
	}
}

void read_device_input_poll(int fd, struct input_event* event) 
{
	nfds_t nfds = 1;
	struct pollfd fds[nfds];
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	// timeout: 5000ms
	int ava_nfds = poll(fds, nfds, 5000);
	if (ava_nfds < 0)
	{
		perror("poll");
	}
	else if(ava_nfds == 0)
	{
		// 没有数据就会timeout退出；
		printf("poll time out\n");
	}
	else
	{
		if (fds[0].revents & POLLIN)
		{
			// 只要没什么意外的情况，读的数据不会出错，就不会结束while; 没有数据传入，就阻塞，也不退出while;
			// 有了一次数据，后面就会一直不会timeout;
			while(sizeof(struct input_event) == read(fds[0].fd, event, sizeof(struct input_event)))
			{
				input_event_translation(event);
				//printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event->type, event->code, event->value);
			}
			// printf("end while get event +++");
		}
	}
}


void read_device_input_select(int fd, struct input_event* event)
{
	struct timeval tv;
	fd_set readfds;
	int nfds = fd + 1; 	  /* readfds/writefds/exceptfds当中最大的file descriptor + 1 */

	/* 想监测哪些文件? */
	FD_ZERO(&readfds);    /* 先全部清零 */	
	FD_SET(fd, &readfds); /* 想监测fd */ 

	/* Wait up to five seconds. */
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	int ready_fds = select(nfds, &readfds, NULL, NULL, &tv);
	printf("ready_fds: %d\n", ready_fds);
	if (ready_fds == -1)	/* -1: error */
		perror("select()");
	else if (ready_fds == 0)	/* 超时 */
		printf("No data within %d second(s) %d ms\n", (int)tv.tv_sec, (int)tv.tv_usec);
	else
	{
		printf("wait time left: %d second(s) %d ms\n", (int)tv.tv_sec, (int)tv.tv_usec);
		if (FD_ISSET(fd, &readfds))	/* 再次确认fd有数据 */
		{
			while (read(fd, event, sizeof(struct input_event)))
			{
				// poll - select 都是一样的，一旦接收到一次数据之后，就从此以后不会timeout;
				// printf("wait time left: %d second(s) %d ms\n", tv.tv_sec, tv.tv_usec);
				//printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event->type, event->code, event->value);
				input_event_translation(event);
			}
		}
	}

	exit(EXIT_SUCCESS);
}

void async_sig_handler(int sig)
{
	struct input_event event;
	int r = 0;
	while (r = read(global_fd, &event, sizeof(struct input_event)) == sizeof(struct input_event))
	{
		input_event_translation(&event);
		// printf("r = %d, get event: type = 0x%x, code = 0x%x, value = 0x%x\n", r, event.type, event.code, event.value);
	}
}

void read_device_input_async(char* f_name)
{
	/* 注册信号处理函数 */
	signal(SIGIO, async_sig_handler);
	/* 打开驱动程序 */
	// 以非阻塞的方式打开设备
	global_fd = open(f_name, O_NONBLOCK | O_RDWR);
	if (global_fd < 0)
	{
		perror(f_name);
		return;
	}

	/* 把APP的进程号告诉驱动程序 */
	fcntl(global_fd, F_SETOWN, getpid());

	/* 使能"异步通知" */
	int flags = fcntl(global_fd, F_GETFL);
	fcntl(global_fd, F_SETFL, flags | FASYNC);

	int count = 0;
	while (1)
	{
		printf("do other things - count: %d\n", count++);
		sleep(2);
	}
	
}


void usage_help(char** argv)
{
	printf("Usage: %s [Device Node]\n", argv[0]);
	printf("\t%s: to get all devices' info under %s\n", argv[0], INPUT_DEVICES_PATH);
	printf("\t%s <Device Node>: to get specific device's info\n", argv[0]);
}

int main(int argc, char** argv) 
{
	// 处理 "--help" 选项
    if (argc >= 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) 
    {
        usage_help(argv);
        return 0; // 结束程序
    }

	// 如果参数超过2个，也显示帮助信息
	if (argc >= 3)
	{
		usage_help(argv);
		return 1;
	}

	char command[512] = ""; // 初始为空字符串
    char temp[128];         // 用于存放当前拼接的字符串
    struct input_event event;

    for (int i = 0; i < argc; i++) {
        snprintf(temp, sizeof(temp), " argv[%d]:%s", i, argv[i]);
        strncat(command, temp, sizeof(command) - strlen(command) - 1);
    }
	printf("command: %s\n", command);

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< main process >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if (argc == 1)
	{
		int ndev = 0;
	    struct dirent **namelist = NULL;  // 初始化 namelist 为 NULL

	    scan_devices(&ndev, &namelist);  // 传递 namelist
	    printf("\n");
	    scan_devices2(&ndev, &namelist);  // 传递 namelist

		// --------------------------- read device info for each --------------------------
		for (int i = 0; i < ndev; i++)
		{
			read_device_info(namelist[i]->d_name);
		}

		// --------------------------- free -----------------------------------------------
	    // 释放资源
	    if (namelist) 
	    {
	        for (int i = 0; i < ndev; i++) 
	        {
	            free(namelist[i]);  // 释放每个目录项
	        }
	        free(namelist);  // 释放 namelist 数组
	    }
	}
	else
	{
		// get device info
		// -----------------------
		read_device_info(argv[1]);
		
		// query device to get input event
		// ----------------------------------
		
		concat_device_name_in_fname(argv[1]);
		printf("----- open and read device: %s -----\n", fname);

		// --- block way to read ---
		printf("--- block way to read ---\n");
		// default block
		int open_flags = O_RDWR;
		// nonblock
		//int open_flags = O_RDWR | O_NONBLOCK;
		int fd = open(fname, open_flags);
		
		if (fd < 0)
		{
			perror(fname);
			return -1;
		}
		/// read_device_input(fd, &event);

		// --- poll way to read ---
		printf("--- poll way to read ---\n");
		read_device_input_poll(fd, &event);
		
		// --- select way to read ---
		printf("--- select way to read ---\n");
		///read_device_input_select(fd, &event);

		// --- async way to read ---
		printf("--- async way to read ---\n");
		///read_device_input_async(fname);
		
	}

    return 0;
}

