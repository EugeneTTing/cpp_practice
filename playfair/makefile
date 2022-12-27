playfair: playfair.o main.o
	g++ -g playfair.o main.o -o playfair

playfair.o: playfair.cpp playfair.h
	g++ -Wall -g -c playfair.cpp -o playfair.o

main.o: main.cpp playfair.h
	g++ -Wall -g -c main.cpp -o main.o

clean: 
	rm -f *.o playfair
