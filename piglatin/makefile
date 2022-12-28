piglatin: main.o piglatin.o
	g++ -g main.o piglatin.o -o piglatin

main.o: main.cpp piglatin.h
	g++ -g -c -Wall main.cpp -o main.o

piglatin.o: piglatin.cpp piglatin.h
	g++ -g -c -Wall piglatin.cpp -o piglatin.o

clean: 
	rm -f *.o piglatin