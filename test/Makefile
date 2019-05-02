main : main_pipe dummy_child timed_child

main_pipe : file_pipes_demo.o
	g++ -o main_pipe file_pipes_demo.o

file_pipes_demo.o : file_pipes_demo.cpp
	g++ -c file_pipes_demo.cpp

dummy_child : ex_receiver.o
	g++ -o dummy_child ex_receiver.o

ex_receiver.o : ex_receiver.cpp
	g++ -c ex_receiver.cpp

timed_child: timed_child.o
	g++ -std=c++0x -o timed_child timed_child.o

timed_child.o : timed_child.cpp
	g++ -std=c++0x -c timed_child.cpp

.PHONY: clean
clean:
	rm file_pipes_demo.o ex_receiver.o timed_child.o
