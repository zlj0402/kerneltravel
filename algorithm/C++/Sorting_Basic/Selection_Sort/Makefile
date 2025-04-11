CC := g++
CFLAGS := -x c++ -std=c++11
OBJ := selection_sort.o
TARGET := selection_sort

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $<

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm $(TARGET) $(OBJ)
