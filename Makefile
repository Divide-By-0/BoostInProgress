all: detectObstacles

detectObstacles: xv11_laser.o obstacleDetection.o main.o
	g++ -o detectObstacles xv11_laser.o obstacleDetection.o main.o -L/usr/local/lib -lboost_system -lglut

main.o: main.cpp
	g++ -c main.cpp -I. -I/usr/local/include

obstacleDetection.o: obstacleDetection.cpp
	g++ -c obstacleDetection.cpp -I. -I/usr/local/include

xv11_laser.o: xv11_laser.cpp
	g++ -c xv11_laser.cpp -I. -I/usr/local/include

clean: 
	rm *.o *~ detectObstacles
