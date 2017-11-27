CC=gcc
LDFLAGS=-Wall


all : STEGANO CLEAN LANCEMENT

STEGANO : STEGANALYSE.c
	$(CC) -o STEGANO STEGANALYSE.c `sdl-config --cflags --libs` $(LDFLAGS)

CLEAN :
	rm -rf *.o

LANCEMENT :
	./STEGANO
