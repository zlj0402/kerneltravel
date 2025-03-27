CC := g++
CFLAGS := -x c++ -std=c++11
OBJ := NamedObject.o
TARGET := NamedObject

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $<

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm $(TARGET) $(OBJ)
