CFLAGS=-Wall `pkg-config --cflags gtk+-3.0 gtkmm-3.0`
LDFLAGS=`pkg-config --libs gtk+-3.0 gtkmm-3.0` -lrt
OBJS=main.o Application.o GraphicalBoard.o Configuration.o Board.o
OBJS+=State.o Move.o Player.o Human.o Game.o Computer.o Heuristic.o
OBJS+=Algorithms.o ConfigurationUI.o
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
