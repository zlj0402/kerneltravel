CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = main
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)