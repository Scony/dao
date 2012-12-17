CFLAGS=-Wall `pkg-config --cflags --libs gtk+-3.0`
LDFLAGS=`pkg-config --libs gtk+-3.0`
OBJS=main.o Application.o GraphicalBoard.o Configuration.o
EXE=main.out

all: $(EXE)

main.out: $(OBJS)
	g++ $(OBJS) -o main.out $(LDFLAGS)

.SUFFIXES:
.SUFFIXES: .o .cpp

.cpp.o:
	g++ $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(EXE)

.PHONY: clean
