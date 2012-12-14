all: main.out
main.out: main.o application.o
	g++ main.o application.o -o main.out `pkg-config --cflags --libs gtk+-3.0`
main.o: main.cpp
	g++ -Wall -c main.cpp -o main.o `pkg-config --cflags --libs gtk+-3.0`
application.o: Application.cpp
	g++ -Wall -c Application.cpp -o application.o `pkg-config --cflags --libs gtk+-3.0`
clean:
	rm -f *.o *~
