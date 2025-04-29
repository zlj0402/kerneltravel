CC := g++
CFLAGS := -x c++ -std=c++11
OBJ := main.o
TARGET := main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $<

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm $(TARGET) $(OBJ)
