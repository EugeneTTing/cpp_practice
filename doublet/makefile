doublets: doublets.o main.o
	g++ -g main.o doublets.o -o doublets -O3

main.o: main.cpp doublets.h
	g++ -Wall -g -c main.cpp -o main.o -O3

doublets.o: doublets.cpp doublets.h
	g++ -Wall -g -c doublets.cpp -o doublets.o -O3

clean:
	rm -f *.o doublets