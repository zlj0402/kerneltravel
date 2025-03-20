CC = $(BUILDROOT_CROSS_COMPILE)gcc
CFLAGS = -I ../include -c
OBJ = accessing_EEPROM_at24c02.o i2cbusses.o smbus.o
TARGET = accessing_EEPROM_at24c02

all: $(TARGET)

accessing_EEPROM_at24c02: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	rm $(OBJ) $(TARGET)
