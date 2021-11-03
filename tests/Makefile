CXX = g++
TARGET = mutexgurd_test
CXXFLAGS = -Wall --std=c++11 -pthread 
SRCS = main.cpp
OBJS := $(SRCS:.cpp=.o) 

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)

check: $(TARGET)
	./$(TARGET)
