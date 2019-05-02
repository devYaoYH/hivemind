CXX=g++
CXXFLAGS=-std=c++0x -Wall -Werror

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
bin = gameManager

$(bin): $(obj)
	$(CXX) -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj)
