EXECUTABLE  = run.exe

PROJECTFILE = driver.cpp
ATH := /usr/um/gcc-6.2.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-6.2.0/lib64
LD_RUN_PATH := /usr/um/gcc-6.2.0/lib64

SOURCES     = $(wildcard *.cpp)
OBJECTS     = $(SOURCES:%.cpp=%.o)
# designate which compiler to use
CXX         = g++

#Default Flags (we prefer -std=c++17 but Mac/Xcode/Clang doesn't support)
CXXFLAGS = -std=c++1z -Wconversion -Wall -Wextra -pedantic 

# make debug - will compile sources with $(CXXFLAGS) and the -g3 flag
#              also defines DEBUG, so "#ifdef DEBUG /*...*/ #endif" works
debug: CXXFLAGS += -g3 -DDEBUG
debug:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

# make clean - remove .o files, executables, tarball
clean:
	rm -f $(EXECUTABLE) $(EXECUTABLE)_debug $(EXECUTABLE)_profile
	rm -Rf *.dSYM
