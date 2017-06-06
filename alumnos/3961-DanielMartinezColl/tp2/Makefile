CC=gcc
CFLAGS=-g -Wall
TARGETS=tp2

all: $(TARGETS)

tp2: tp2.o  contador.o  convertidor.o

format:
	fold -w 80 -s noticia > noticia

clean:
	rm -f *~ .o $(TARGETS) 
