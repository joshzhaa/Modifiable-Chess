EXECUTABLE  = test.exe

PROJECTFILE = test.cpp

SOURCES     = $(wildcard *.cpp)
OBJECTS     = $(SOURCES:%.cpp=%.o)

CXX         = g++
CXXFLAGS = -std=c++2a -Wconversion -Wall -Wextra -pedantic 

opt: CXXFLAGS += -O3 -g3 -DDEBUG
opt:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

release: CXXFLAGS += -O3
release:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

debug: CXXFLAGS += -g3 -DDEBUG
debug:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

# make clean - remove .o files, executables, tarball
clean:
	rm -f $(EXECUTABLE) $(EXECUTABLE)_debug $(EXECUTABLE)_profile
	rm -Rf *.dSYM
