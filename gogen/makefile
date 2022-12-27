gogen: main.o gogen.o
	g++ -g main.o gogen.o -o gogen

main.o: main.cpp gogen.h common.h mask.h
	g++ -g -c -Wall main.cpp -o main.o

gogen.o: gogen.cpp gogen.h common.h mask.h
	g++ -g -c -Wall gogen.cpp -o gogen.o

clean: 
	rm -f *.o gogen