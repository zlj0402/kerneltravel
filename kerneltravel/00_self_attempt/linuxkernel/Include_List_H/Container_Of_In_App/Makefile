CC := gcc
CFLAGS :=
OBJ := main.o
TARGET := main

all: $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $@ $<

%.o: %.c
	gcc -o $(CFLAGS) $@ -c $<

clean:
	rm -f $(OBJ) $(TARGET)
