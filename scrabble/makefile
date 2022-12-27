scrabble: main.o scrabble.o
	g++ -g main.o scrabble.o -o scrabble

main.o: main.cpp scrabble.h
	g++ -Wall -g -c main.cpp -o main.o

scrabble.o: scrabble.cpp scrabble.h
	g++ -Wall -g -c scrabble.cpp -o scrabble.o

clean:
	rm -f *.o scrabble