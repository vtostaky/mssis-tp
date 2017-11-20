CC=gcc
CFLAGS=-I.
DEPS= vehicule.h liste_vehicules.h
OBJ= vehicule.o liste_vehicules.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
tp4exe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
