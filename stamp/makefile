stamp: main.o stamp.o
	g++ -g main.o stamp.o -o stamp -lcrypto

stamp.o: stamp.cpp stamp.h
	g++ -Wall -g -c stamp.cpp -o stamp.o 

main.o: main.cpp stamp.h
	g++ -Wall -g -c main.cpp -o main.o 

clean: 
	rm -f *.o stamp
