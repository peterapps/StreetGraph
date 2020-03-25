EXECUTABLE = navsim
CXX = g++
SOURCES     = $(wildcard *.cpp)
CXXFLAGS = `pkg-config --cflags --libs opencv`

release: CXXFLAGS += -O3 -DNDEBUG
release:
	$(CXX) $(SOURCES) -o $(EXECUTABLE) $(CXXFLAGS)

debug: CXXFLAGS += -g3 -DDEBUG
debug:
	$(CXX) $(SOURCES) -o $(EXECUTABLE)_debug $(CXXFLAGS) 

clean:
	rm -f $(EXECUTABLE) $(EXECUTABLE)_debug