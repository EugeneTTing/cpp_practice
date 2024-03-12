correct: main.o correct.o
	g++ -g main.o correct.o -o correct

main.o: main.cpp correct.h
	g++ -g -Wall -c main.cpp -o main.o

correct.o: correct.cpp correct.h
	g++ -g -Wall -c correct.cpp -o correct.o

clean:
	rm -f *.o correct