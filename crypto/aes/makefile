CC=gcc
CFLAGS=-Wall
LDFLAGS=-Wall -lm 


all : EXEC clean lancement
EXEC: AES_MODIFIE.o AES_MODIFIE_main.o
	$(CC) -o EXEC AES_MODIFIE.o AES_MODIFIE_main.o $(LDFLAGS) 


AES_MODIFIE.o: AES_MODIFIE.c AES_MODIFIE.h LIBRAIRIES.h
	$(CC) -c AES_MODIFIE.c $(CFLAGS)



AES_MODIFIE_main.o: AES_MODIFIE_main.c AES_MODIFIE.h LIBRAIRIES.h
	$(CC) -c AES_MODIFIE_main.c $(CFLAGS)

clean: 
	rm -rf *.o

lancement:
	./EXEC
