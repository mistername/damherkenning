SOURCES = src/AverageColor.cpp src/blur.cpp src/CheckerBoard.cpp src/Checkers_detection.cpp src/Constants.cpp src/DominantColor.cpp src/FunctionKernel.cpp src/Image.cpp src/ImageFunction.cpp src/Kernel.cpp src/Threshold.cpp
CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread `pkg-config --cflags opencv4`
LDLIBS = `pkg-config --libs opencv4`

OBJECTS = $(SOURCES:.cpp=.o)

prog: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDLIBS) -o $@ $^

clean::
	$(RM) prog

.cpp.o:
	$(CXX) -MD -MP $(CXXFLAGS) $(LDLIBS) -o $@ -c $<

clean::
	$(RM) src/*.o

DEPENDS = $(SOURCES:.cpp=.d)

-include $(DEPENDS)

%.d:
	@touch $@

clean::
	$(RM) src/*.d
