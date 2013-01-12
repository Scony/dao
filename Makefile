EXE=dao_expert

all: $(EXE)

$(EXE): src
	make -C src
	cp src/main.out $(EXE)

clean: 
	make -C src clean
	rm -f $(EXE)

.PHONY: all, clean
