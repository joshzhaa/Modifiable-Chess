EXECUTABLE  = test.exe

PROJECTFILE = test.cpp

SOURCES     = $(wildcard *.cpp)
OBJECTS     = $(SOURCES:%.cpp=%.o)

CXX         = clang++
CXXFLAGS = -std=c++2a -Wall -Wextra -pedantic

debug: CXXFLAGS += -g3 -DDEBUG
debug:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

release: CXXFLAGS += -O3
release:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

# make clean - remove .o files, executables, tarball
clean:
	rm -f $(EXECUTABLE) $(EXECUTABLE)_debug $(EXECUTABLE)_profile
	rm -Rf *.dSYM
