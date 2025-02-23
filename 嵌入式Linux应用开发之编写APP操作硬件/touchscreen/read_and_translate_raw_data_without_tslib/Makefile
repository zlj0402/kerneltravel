CC = ${BUILDROOT_CROSS_COMPILE}gcc
CFLAGS = -I./ -c
LDFLAGS = -L./
SHARED_FLAGS = -shared -fPIC

#SRC = get_input_devices_info.c input_events.c
OBJ = get_input_devices_info.o
LIBS = libinput_events.so

all: get_input_devices_info 

get_input_devices_info: $(OBJ) $(LIBS)
	$(CC) -o $@ $(LDFLAGS) $(LIBS) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

lib%.so: %.c
	$(CC) $(SHARED_FLAGS) $< -o $@

clean:
	rm -f ${OBJ} get_input_devices_info $(LIBS)
