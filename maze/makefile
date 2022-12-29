maze: maze.o main.o
	g++ -g maze.o main.o -o maze

main.o: main.cpp maze.h
	g++ -Wall -g -c main.cpp -o main.o

maze.o: maze.cpp maze.h
	g++ -Wall -g -c maze.cpp -o maze.o

clean:
	rm -f *.o maze