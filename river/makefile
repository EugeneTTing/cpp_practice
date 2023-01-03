river: main.o river.o
	g++ -g main.o river.o -o river

main.o: main.cpp river.h
	g++ -Wall -g -c main.cpp -o main.o

river.o: river.cpp river.h
	g++ -Wall -g -c river.cpp -o river.o

clean:
	rm -f *.o river