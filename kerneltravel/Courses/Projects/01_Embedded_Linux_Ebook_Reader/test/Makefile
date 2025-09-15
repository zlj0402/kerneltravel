CC := gcc
OBJ := getopt1.o
CFLAGS := 
TARGET := getopt1

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
