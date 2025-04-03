all: clean compile

clean:
	@rm -f *.o output.ppm main

compile:
	@g++ -c tinyxml2.cpp
	@g++ -c main.cpp
	@g++ -c util.cpp
	@g++ -c point.cpp
	@g++ -c vector.cpp
	@g++ -c ray.cpp
	@g++ -c imagePlane.cpp
	@g++ -c camera.cpp
	@g++ -c triangle.cpp
	@g++ -c matrix_3x3.cpp
	@g++ -c scene.cpp
	@g++ -c ambientLight.cpp
	@g++ -c pointLight.cpp
	@g++ -c triangularLight.cpp
	@g++ -c mesh.cpp
	@g++ -c material.cpp
	@g++ -c face.cpp
	@g++ -o main tinyxml2.o main.o util.o imagePlane.o camera.o triangle.o point.o vector.o ray.o matrix_3x3.o scene.o ambientLight.o pointLight.o triangularLight.o mesh.o material.o face.o
