minesweeper: main.o minesweeper.o
	g++ -g main.o minesweeper.o -o minesweeper

main.o: main.cpp minesweeper.h
	g++ -Wall -g -c main.cpp -o main.o

minesweeper.o: minesweeper.cpp minesweeper.h
	g++ -Wall -g -c minesweeper.cpp -o minesweeper.o

clean:
	rm -f *.o minesweeper