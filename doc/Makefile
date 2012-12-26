
PDFS=dao_expert.pdf

all: $(PDFS)

.SUFFIXES:
.SUFFIXES: .tex .pdf

.tex.pdf:
	pdflatex $<

clean:
	rm -f $(PDFS) *.aux *.log

.PHONY: all clean
