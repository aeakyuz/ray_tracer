all: clean compile

clean:
	@rm *.o main

compile:
	@g++ -c tinyxml2.cpp
	@g++ -c main.cpp
	@g++ -c util.cpp
	@g++ -c imagePlane.cpp
	@g++ -c camera.cpp
	@g++ -c triangle.cpp
	@g++ -std=c++11 -Wall -Wextra -pedantic -Werror -Wshadow -Wconversion -Wsign-conversion -Wunused -o main tinyxml2.o main.o util.o imagePlane.o camera.o triangle.o
