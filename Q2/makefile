#makefile

all: compile run

setup:
	cp books/* .

compile:
	gcc txt2epub.c -o txt2epub

run:
	./txt2epub *.txt 

clean:
	rm txt2epub
	rm *.epub
	rm ebooks.zip
