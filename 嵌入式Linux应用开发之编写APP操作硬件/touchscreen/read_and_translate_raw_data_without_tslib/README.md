<<<<<<<<<<<<<<<<<<<<<<<<< Usage in Linux >>>>>>>>>>>>>>>>>>

./get_input_device_info [evdev_name under /dev/input]
  + `./get_input_device_info` get all evdev info under /dev/input dir;
  + `./get_input_device_info <evdev_name>` to read specific touchscreen device

<<<<<<<<<<<<<<<<<<<<<<<<<< What implement >>>>>>>>>>>>>>>>>>>>>

> `./get_input_device_info <evdev_name>`

四种读取touchscreen方式 -- [read/poll-read/select-read/async signal - read]
```

  a. while read[nonblock] ----- side effect: high cpu
  b. while read[block] ----- side effect: when block, main process can not do other things
  c. poll[block] -----               
                      | --- side effect: timeout will not work once first data come, and then block 
  d. select[block] ---
  e. async signal[nonblock] ----- the better way to read data
```

> `./get_input_device_info`
get all evdev device name under directory `/dev/input`
```
1. get devices name -- by scandir
2. if evdev, get device id struct -- `ioctl(int fd, EVIOCGID, &struct input_id id)`
3. xxx, get device name -- `int name_len = ioctl(fd, EVIOCGNAME(sizeof(d_name)), d_name);`
4. xxx, get device support event types -- `ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit)`
5. xxx, judge whether device is ts devcie -- `int prop_len = ioctl(fd, EVIOCGPROP(sizeof(properties)), &properties);`
6. xxx && is ts device, get max slots -- `ioctl(fd, EVIOCGABS(ABS_MT_SLOT), &struct input_absinfo slot);`
```
