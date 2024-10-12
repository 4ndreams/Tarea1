TARGET = tarea

SRCS = Tarea1.cpp

CXX = g++
CXXFLAGS = -std=c++11 -pthread

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

