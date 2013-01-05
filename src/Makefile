CFLAGS=-Wall `pkg-config --cflags gtk+-3.0 gtkmm-3.0` -fpermissive
LDFLAGS=`pkg-config --libs gtk+-3.0 gtkmm-3.0`
OBJS=main.o Application.o GraphicalBoard.o Configuration.o Board.o 
OBJS+=State.o
EXE=main.out

all: $(EXE)

main.out: $(OBJS)
	g++ $(OBJS) -o main.out $(LDFLAGS)

.SUFFIXES:
.SUFFIXES: .o .cpp

.cpp.o:
	g++ $(CFLAGS) -c $<

clean:
	rm -f *~ $(OBJS) $(EXE)

.PHONY: clean
