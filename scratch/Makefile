all: main
main: main.o
	g++ main.o -o main `pkg-config --cflags --libs gtk+-3.0`
main.o: main.cpp
	g++ -Wall -c main.cpp -o main.o `pkg-config --cflags --libs gtk+-3.0`
clean:
	rm -f *.o *~
